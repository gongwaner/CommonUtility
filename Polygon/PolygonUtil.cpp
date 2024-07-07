#include "PolygonUtil.h"

#include <vtkTriangle.h>
#include <vtkVectorOperators.h>


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
}
