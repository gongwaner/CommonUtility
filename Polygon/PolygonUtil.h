#pragma once

#include <vtkVector.h>


namespace PolygonUtil
{
    /**
     * compute normal for any kind of polygon. as vtkPolygon::ComputeNormal only works for convex polygon
     * ref: https://gitlab.kitware.com/vtk/vtk/-/issues/11988
     */
    vtkVector3d GetPolygonNormal(const std::vector<vtkVector3d>& polygonPoints);
}

