#pragma once

#include <vtkVector.h>


class vtkPolyData;

namespace TestUtil
{
    //print functions
    void Print(const std::string& msg, const double vec[3]);
    void Print(const std::string& msg, const vtkVector3d& vec);

    void AppendCube(vtkSmartPointer<vtkPolyData>& mesh, const vtkVector3d& pos, double size);
}


