#pragma once

#include <vtkVector.h>


namespace CommonUtil
{
    template<typename T>
    void RemoveIndex(std::vector<T>& vector, int index)
    {
        if(index >= 0 && index < vector.size())
            vector.erase(vector.begin() + index);
    }

    template<typename T>
    bool Contains(const std::vector<T>& vec, const T& value)
    {
        return std::find(vec.begin(), vec.end(), value) != vec.end();
    }

    bool EpsilonEqual(const vtkVector3d& p0, const vtkVector3d& p1, double epsilon = 1e-6);
    bool EpsilonContains(const std::vector<vtkVector3d>& points, const double point[3], double epsilon = 1e-6);
    bool EpsilonContains(const std::vector<vtkVector3d>& points, const vtkVector3d& point, double epsilon = 1e-6);
    vtkVector3d GetAverage(const std::vector<vtkVector3d>& data);
}
