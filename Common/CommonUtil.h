#pragma once

#include <vtkVector.h>


namespace CommonUtil
{
    bool EpsilonEqual(const vtkVector3d& p0, const vtkVector3d& p1, double epsilon = 1e-6);
    bool EpsilonContains(const std::vector<vtkVector3d>& points, const double point[3], double epsilon = 1e-6);
    bool EpsilonContains(const std::vector<vtkVector3d>& points, const vtkVector3d& point, double epsilon = 1e-6);
    vtkVector3d GetAverage(const std::vector<vtkVector3d>& data);
}
