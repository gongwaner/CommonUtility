#pragma once

#include <vtkVector.h>


class vtkPolyData;

namespace GeometricObjectUtil
{
    vtkSmartPointer<vtkPolyData> GetLinePolyData(const vtkVector3d& start, const vtkVector3d& end);
    vtkSmartPointer<vtkPolyData> GetLinePolyData(const vtkVector3d& start, const vtkVector3d& dir, double length);
    vtkSmartPointer<vtkPolyData> GetCubePolyData(const vtkVector3d& pos, double size);
}

