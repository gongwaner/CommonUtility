#pragma once

#include <vtkVector.h>

#include <optional>


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

    /**
     * Check if given point is within triangle ABC.
     */
    bool PointInTriangle(const vtkVector3d& point, const vtkVector3d& A, const vtkVector3d& B, const vtkVector3d& C, double epsilon = 1e-6);

    /**
     * Check whether 2 lines intersect or not.
     * Note: this function does not calculate intersection point.
     * By default, cases where intersection point is on either line or shared by both lines(common end point)
     * are counted as has intersection. If the latter is not desired, set  'commonEndpointAsIntersection' to false.
     */
    bool LinesIntersect(const vtkVector3d& line1Start, const vtkVector3d& line1End,
                        const vtkVector3d& line2Start, const vtkVector3d& line2End,
                        bool commonEndpointAsIntersection = true, double epsilon = 1e-6);

    bool LinesIntersect(const std::pair<vtkVector3d, vtkVector3d>& line1, const std::pair<vtkVector3d, vtkVector3d>& line2,
                        bool commonEndpointAsIntersection = true, double epsilon = 1e-6);

    /**
     * Calculate the intersection points of 2 lines
     */
    std::optional<vtkVector3d> GetLinesIntersection(const vtkVector3d& line1Start, const vtkVector3d& line1End,
                                                    const vtkVector3d& line2Start, const vtkVector3d& line2End, double epsilon = 1e-6);
}
