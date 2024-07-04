#pragma once

#include <vtkVector.h>


namespace TestUtil
{
    void Print(const std::string& msg, const double vec[3]);
    void Print(const std::string& msg, const vtkVector3d& vec);
}


