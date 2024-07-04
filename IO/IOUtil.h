#pragma once

#include <vtkVector.h>

#include <vector>


class vtkPolyData;
class vtkImageData;

namespace IOUtil
{
    //poly data
    vtkSmartPointer<vtkPolyData> ReadMesh(const char* fileDir);
    void WriteMesh(const char* fileDir, vtkPolyData* polyData);
    void WriteColorMesh(const char* fileDir, vtkPolyData* polyData);
    void WriteFeatureVectorToFile(const char* dir, const std::vector<double>& dataVector, bool append = false);

    //image data
    vtkSmartPointer<vtkImageData> ReadImageData(const char* folder);
    void WritePng(const char* fileDir, vtkImageData* imageData, bool isUnsignedShort = true);
}
