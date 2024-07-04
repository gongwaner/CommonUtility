#pragma once

#include <vtkVector.h>


class vtkPolyData;
class vtkPoints;

namespace GeometricObjectUtil
{
    vtkSmartPointer<vtkPolyData> GetPointsPolyData(vtkPoints* points);
    vtkSmartPointer<vtkPolyData> GetLinePolyData(const vtkVector3d& start, const vtkVector3d& end);
    vtkSmartPointer<vtkPolyData> GetLinePolyData(const vtkVector3d& start, const vtkVector3d& dir, double length);
    vtkSmartPointer<vtkPolyData> GetCubePolyData(const vtkVector3d& pos, double size);

    /**
     * given plane center and normal, return its axis X and Y
     */
    void GetPlaneAxes(double center[3], double normal[3], double axisX[3], double axisY[3]);
}

