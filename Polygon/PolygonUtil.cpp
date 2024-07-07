#include "PolygonUtil.h"

#include <vtkTriangle.h>
#include <vtkVectorOperators.h>
#include <vtkLine.h>


namespace PolygonUtil
{
    vtkVector3d GetTriangleNormal(const vtkVector3d& A, const vtkVector3d& B, const vtkVector3d& C)
    {
        vtkVector3d AB = B - A;
        vtkVector3d AC = C - A;

        vtkVector3d normal = AB.Cross(AC);

        return normal.Normalized();
    }

    vtkVector3d GetPolygonNormal(const std::vector<vtkVector3d>& polygonPoints)
    {
        vtkVector3d normal{0, 0, 0};

        vtkVector3d pt0 = polygonPoints[0];
        vtkVector3d pt1;
        auto numOfPoints = polygonPoints.size();

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

    vtkSmartPointer<vtkTriangle> GetTriangle(int vid0, int vid1, int vid2)
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

        if(u > epsilon && u < 1 && v > epsilon && v < 1)//not doing u - 1 < epsilon here because when u==1.0 should return false
            return true;

        return false;
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

        if(u >= (0 - epsilon) && u <= (1 + epsilon) && v >= (0 - epsilon) && v <= (1 + epsilon))//u, v falls into [0,1]
        {
            return line1Start + u * (line1End - line1Start);
        }

        return std::nullopt;
    }
}
