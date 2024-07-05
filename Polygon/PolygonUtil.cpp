#include "PolygonUtil.h"

#include <vtkTriangle.h>


namespace PolygonUtil
{
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

    vtkSmartPointer<vtkTriangle> GetTriangle(const std::vector<vtkVector3d>& points, const int vid0, const int vid1, const int vid2,
                                             const vtkVector3d& planeNormal)
    {
        double triNormal[3];
        vtkTriangle::ComputeNormal(points[vid0].GetData(), points[vid1].GetData(), points[vid2].GetData(), triNormal);

        auto triangle = vtkSmartPointer<vtkTriangle>::New();
        if(vtkVector3d(triNormal).Dot(planeNormal) < 0)
        {
            triangle->GetPointIds()->SetId(0, vid0);
            triangle->GetPointIds()->SetId(1, vid1);
            triangle->GetPointIds()->SetId(2, vid2);
        }
        else
        {
            triangle->GetPointIds()->SetId(0, vid2);
            triangle->GetPointIds()->SetId(1, vid1);
            triangle->GetPointIds()->SetId(2, vid0);
        }

        return triangle;
    }
}
