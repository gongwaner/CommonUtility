#pragma once

#include <vtkVector.h>


class vtkPolyData;

namespace TestUtil
{
    //print functions
    void Print(const std::string& msg, const double vec[3]);
    void Print(const std::string& msg, const vtkVector3d& vec);

    /**
     * Append cube at pos onto mesh with given size
     */
    void AppendCube(vtkSmartPointer<vtkPolyData>& mesh, const vtkVector3d& pos, double size);

    /**
     * Append cube at pos onto mesh with given size and color
     * Note: to see color the export file extension should be .ply
     */
    void AppendCube(vtkSmartPointer<vtkPolyData>& mesh, const vtkVector3d& pos, double size, const vtkVector4d& color);
}


