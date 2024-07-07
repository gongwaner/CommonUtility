#pragma once

#include <vtkVector.h>


class vtkSphere;
class vtkPolyData;
class vtkPoints;

namespace GeometricObjectUtil
{
    vtkSmartPointer<vtkSphere> GetSphereImplicitFunction(const vtkVector3d& sphereCenter, double radius);

    vtkSmartPointer<vtkPolyData> GetSpherePolyData(const vtkVector3d& center, double radius, int phiResolution, int thetaResolution);

    vtkSmartPointer<vtkPolyData> GetPointPolyData(const vtkVector3d& point);
    vtkSmartPointer<vtkPolyData> GetPointsPolyData(vtkPoints* points);

    /**
     * Return line polydata for visualization. If you want the mesh for export, use GetLineMesh() instead
     */
    vtkSmartPointer<vtkPolyData> GetLinePolyData(const vtkVector3d& start, const vtkVector3d& end);
    vtkSmartPointer<vtkPolyData> GetLinePolyData(const vtkVector3d& start, const vtkVector3d& dir, double length);

    vtkSmartPointer<vtkPolyData> GetLineMesh(const vtkVector3d& lineStart, const vtkVector3d& lineEnd, double radius);

    vtkSmartPointer<vtkPolyData> GetPlanePolyData(double center[3], double normal[3], int xResolution, int yResolution, int width, int height);

    vtkSmartPointer<vtkPolyData> GetCubePolyData(const vtkVector3d& pos, double size);

    /**
     * Return a polygonal cylinder centered at given center.
     * The axis of the cylinder is aligned along the global y-axis.
     */
    vtkSmartPointer<vtkPolyData> GetCylinderPolyData(const vtkVector3d& center, double radius, double height, int resolution);

    /**
     * Given plane center and normal, return its axis X and Y
     */
    void GetPlaneAxes(double center[3], double normal[3], double axisX[3], double axisY[3]);
}

