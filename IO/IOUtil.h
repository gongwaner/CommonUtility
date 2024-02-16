#pragma once

#include <vtkVector.h>

#include <vector>


class vtkPolyData;

namespace IOUtil
{
    vtkSmartPointer<vtkPolyData> ReadMesh(const char* fileDir);
    void WriteMesh(const char* fileDir, vtkPolyData* polyData);
    void WriteColorMesh(const char* fileDir, vtkPolyData* polyData);
}
