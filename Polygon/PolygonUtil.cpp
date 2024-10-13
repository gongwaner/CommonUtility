#include "PolygonUtil.h"

#include <vtkTriangle.h>
#include <vtkVectorOperators.h>
#include <vtkLine.h>


namespace PolygonUtil
{
    vtkVector3d GetTriangleNormal(const vtkVector3d& A, const vtkVector3d& B, const vtkVector3d& C)
    {
        const auto AB = B - A;
        const auto AC = C - A;

        const auto normal = AB.Cross(AC);

        return normal.Normalized();
    }

    vtkVector3d GetPolygonNormal(const std::vector<vtkVector3d>& polygonPoints)
    {
        vtkVector3d normal{0, 0, 0};

        vtkVector3d pt0 = polygonPoints[0];
        vtkVector3d pt1;
        const auto numOfPoints = polygonPoints.size();

        for(unsigned int i = 0; i < numOfPoints; i++)
        {
            pt1 = polygonPoints[(i + 1) % numOfPoints];

            normal[0] += (pt0[1] - pt1[1]) * (pt0[2] + pt1[2]);
            normal[1] += (pt0[2] - pt1[2]) * (pt0[0] + pt1[0]);
            normal[2] += (pt0[0] - pt1[0]) * (pt0[1] + pt1[1]);

            pt0[0] = pt1[0];
            pt0[1] = pt1[1];
            pt0[2] = pt1[2];
        }

        return normal.Normalized();
    }

    vtkSmartPointer<vtkTriangle> GetTriangle(const int vid0, const int vid1, const int vid2)
    {
        auto triangle = vtkSmartPointer<vtkTriangle>::New();

        triangle->GetPointIds()->SetId(0, vid0);
        triangle->GetPointIds()->SetId(1, vid1);
        triangle->GetPointIds()->SetId(2, vid2);

        return triangle;
    }

    vtkSmartPointer<vtkTriangle> GetTriangle(const std::vector<vtkVector3d>& polygonPoints, const int vid0, const int vid1, const int vid2,
                                             const vtkVector3d& polygonNormal)
    {
        if(polygonPoints.size() < 3)
            throw std::runtime_error("PolygonUtil::GetTriangle(). Error: polygonPoints.size() should be > 3!");

        const auto triNormal = GetTriangleNormal(polygonPoints[vid0], polygonPoints[vid1], polygonPoints[vid2]);

        vtkSmartPointer<vtkTriangle> triangle;
        if(triNormal.Dot(polygonNormal) < 0)
            triangle = GetTriangle(vid0, vid1, vid2);
        else
            triangle = GetTriangle(vid2, vid1, vid0);

        return triangle;
    }

    bool PointInTriangle(const vtkVector3d& point, const vtkVector3d& A, const vtkVector3d& B, const vtkVector3d& C, const double epsilon)
    {
        const auto v0 = B - A;
        const auto v1 = C - A;
        const auto v2 = point - A;

        // Compute dot products
        const auto dot00 = v0.Dot(v0);
        const auto dot01 = v0.Dot(v1);
        const auto dot02 = v0.Dot(v2);
        const auto dot11 = v1.Dot(v1);
        const auto dot12 = v1.Dot(v2);

        //compute barycentric coordinates
        const auto invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
        const auto u = (dot11 * dot02 - dot01 * dot12) * invDenom;
        const auto v = (dot00 * dot12 - dot01 * dot02) * invDenom;

        return (u >= -epsilon) && (v >= -epsilon) && (u + v < 1);
    }

    bool LinesIntersect(const vtkVector3d& line1Start, const vtkVector3d& line1End,
                        const vtkVector3d& line2Start, const vtkVector3d& line2End,
                        const bool commonEndpointAsIntersection, const double epsilon)
    {
        double u, v;
        int result = vtkLine::Intersection(line1Start.GetData(), line1End.GetData(), line2Start.GetData(), line2End.GetData(), u, v);

        if(result == vtkLine::IntersectionType::NoIntersect)
            return false;

        if(commonEndpointAsIntersection)
            return true;

        if(abs(u) < epsilon || abs(v) < epsilon) //u or v == 0
            return false;

        if(abs(u - 1) < epsilon || abs(v - 1) < epsilon) //u or v == 1
            return false;

        return true;
    }

    bool LinesIntersect(const std::pair<vtkVector3d, vtkVector3d>& line1, const std::pair<vtkVector3d, vtkVector3d>& line2,
                        const bool commonEndpointAsIntersection, const double epsilon)
    {
        return LinesIntersect(line1.first, line1.second, line2.first, line2.second, commonEndpointAsIntersection, epsilon);
    }

    std::optional<vtkVector3d> GetLineIntersection(const vtkVector3d& line1Start, const vtkVector3d& line1End,
                                                   const vtkVector3d& line2Start, const vtkVector3d& line2End, const double epsilon)
    {
        double u, v;
        int result = vtkLine::Intersection(line1Start.GetData(), line1End.GetData(), line2Start.GetData(), line2End.GetData(), u, v);

        if(result == vtkLine::IntersectionType::NoIntersect)
            return std::nullopt;

        //either intersect or on line
        return line1Start + u * (line1End - line1Start);
    }
}
