#pragma once

#include <vtkVector.h>


namespace CommonUtil
{
    bool EpsilonEqual(const vtkVector3d& p0, const vtkVector3d& p1, double epsilon = 1e-6);
    vtkVector3d GetAverage(const std::vector<vtkVector3d>& data);
}
