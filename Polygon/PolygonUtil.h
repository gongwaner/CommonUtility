#pragma once

#include <vtkVector.h>


class vtkTriangle;
class vtkPolyData;
class vtkPoints;
class vtkCellArray;

namespace PolygonUtil
{
    /**
     * Replacement function for  vtkTriangle::ComputeNormal()
     */
    vtkVector3d GetTriangleNormal(const vtkVector3d& A, const vtkVector3d& B, const vtkVector3d& C);

    /**
     * compute normal for any kind of polygon. as vtkPolygon::ComputeNormal only works for convex polygon
     * ref: https://gitlab.kitware.com/vtk/vtk/-/issues/11988
     */
    vtkVector3d GetPolygonNormal(const std::vector<vtkVector3d>& polygonPoints);

    vtkSmartPointer<vtkTriangle> GetTriangle(int vid0, int vid1, int vid2);

    /**
     * Given polygon points and triangle index, return a triangle whose vertex indices is the same direction as polygon normal
     * Note: polygonPoints.size() should be >= 3
     */
    vtkSmartPointer<vtkTriangle> GetTriangle(const std::vector<vtkVector3d>& polygonPoints, int vid0, int vid1, int vid2,
                                             const vtkVector3d& polygonNormal);
}
