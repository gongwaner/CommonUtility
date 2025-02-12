#pragma once

#include <vtkVector.h>


class vtkPolyData;

namespace TestUtil
{
    std::string GetDataDir();
    std::string GetOutputDir();

    //print functions
    void Print(const std::string& msg, const double vec[3]);
    void Print(const std::string& msg, const vtkVector3d& vec);

    /**
     * Append cube at pos onto mesh with given size
     */
    void AppendCube(vtkSmartPointer<vtkPolyData>& mesh, const vtkVector3d& pos, double size);

    /**
     * Append cube at pos onto mesh with given size and color.
     * Note: to see the color, the export file extension should be .ply
     */
    void AppendCube(vtkSmartPointer<vtkPolyData>& mesh, const vtkVector3d& pos, double size, const vtkVector4d& color);

    /**
     * Append line onto mesh
     */
    void AppendLine(vtkSmartPointer<vtkPolyData>& mesh, const vtkVector3d& lineStart, const vtkVector3d& lineEnd, double radius);
    void AppendLine(vtkSmartPointer<vtkPolyData>& mesh, const std::pair<vtkVector3d, vtkVector3d>& line, double radius);

    /**
     * Append line onto mesh with given color.
     * Note: to see the color, the export file extension should be .ply
     */
    void AppendLine(vtkSmartPointer<vtkPolyData>& mesh, const std::pair<vtkVector3d, vtkVector3d>& line, double radius, const vtkVector4d& color);
}
