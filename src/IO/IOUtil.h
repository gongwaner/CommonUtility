#pragma once

#include <vtkSmartPointer.h>

#include <vector>
#include <string>


class vtkPolyData;
class vtkImageData;

namespace IOUtil
{
    //extensions for polydata
    const std::string PlyExtension = ".ply";
    const std::string ObjExtension = ".obj";
    const std::string StlExtension = ".stl";
    const std::string VtkExtension = ".vtk";

    //extension for imagedata
    const std::string JpegExtension = ".jpg";
    const std::string PngExtension = ".png";

    //poly data
    vtkSmartPointer<vtkPolyData> ReadMesh(const std::string& fileDir);
    void WriteMesh(const std::string& fileDir, vtkPolyData* polyData);
    void WriteColorMesh(const std::string& fileDir, vtkPolyData* polyData);
    void WriteFeatureVectorToFile(const std::string& dir, const std::vector<double>& dataVector, bool append = false);

    //image data
    vtkSmartPointer<vtkImageData> ReadImageData(const std::string& fileDir);
    vtkSmartPointer<vtkImageData> ReadDicomImageData(const std::string& dicomFolder);
    void WritePng(const std::string& fileDir, vtkImageData* imageData, bool isUnsignedShort = true);
    void WriteJpeg(const std::string& fileDir, vtkImageData* imageData);
}
