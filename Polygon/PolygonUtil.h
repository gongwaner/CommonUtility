#pragma once

#include <vtkVector.h>


class vtkTriangle;

namespace PolygonUtil
{
    /**
     * compute normal for any kind of polygon. as vtkPolygon::ComputeNormal only works for convex polygon
     * ref: https://gitlab.kitware.com/vtk/vtk/-/issues/11988
     */
    vtkVector3d GetPolygonNormal(const std::vector<vtkVector3d>& polygonPoints);

    vtkSmartPointer<vtkTriangle> GetTriangle(const std::vector<vtkVector3d>& points, int vid0, int vid1, int vid2, const vtkVector3d& planeNormal);
}
