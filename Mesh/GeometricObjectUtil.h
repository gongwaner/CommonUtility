#pragma once

#include <vtkVector.h>


class vtkSphere;
class vtkPolyData;
class vtkPoints;

namespace GeometricObjectUtil
{
    /**
     * Point(s) visualization
     */
    vtkSmartPointer<vtkPolyData> GetPointPolyData(const vtkVector3d& point);

    vtkSmartPointer<vtkPolyData> GetPointsPolyData(vtkPoints* points);

    /**
     * Line visualization. If you want the mesh for export, use GetLineMesh() instead of GetLinePolyData()
     */
    vtkSmartPointer<vtkPolyData> GetLinePolyData(const vtkVector3d& start, const vtkVector3d& end);

    vtkSmartPointer<vtkPolyData> GetLinePolyData(const vtkVector3d& start, const vtkVector3d& dir, double length);

    vtkSmartPointer<vtkPolyData> GetLineMesh(const vtkVector3d& lineStart, const vtkVector3d& lineEnd, double radius);

    vtkSmartPointer<vtkPolyData> GetDashedLinePolyData(const std::vector<vtkVector3d>& points, double dashLength, double gapLength);

    /**
     * Curve visualization
     */
    vtkSmartPointer<vtkPolyData> GetCurvePolyData(vtkPoints* points);

    /**
     * Sphere visualization
     */
    vtkSmartPointer<vtkPolyData> GetSpherePolyData(const vtkVector3d& center, double radius, int phiResolution, int thetaResolution);

    vtkSmartPointer<vtkSphere> GetSphereImplicitFunction(const vtkVector3d& sphereCenter, double radius);

    /**
     * Cube visualization
     */
    vtkSmartPointer<vtkPolyData> GetCubePolyData(const vtkVector3d& pos, double size);

    /**
     * Return a polygonal cylinder centered at given center.
     * The axis of the cylinder is aligned along the global y-axis.
     */
    vtkSmartPointer<vtkPolyData> GetCylinderPolyData(const vtkVector3d& center, double radius, double height, int resolution);

    /**
     * Plane visualization
     */
    vtkSmartPointer<vtkPolyData> GetPlanePolyData(double center[3], double normal[3], int xResolution, int yResolution, int width, int height);

    /** Given plane center and normal, return its axis X and Y*/
    void GetPlaneAxes(double center[3], double normal[3], double axisX[3], double axisY[3]);
}

