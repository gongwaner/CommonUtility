#include "IOUtil.h"

#include <vtkPolyData.h>
#include <vtkPLYReader.h>
#include <vtkPLYWriter.h>
#include <vtkOBJReader.h>
#include <vtkOBJWriter.h>
#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>
#include <vtkPolyDataReader.h>

#include <vtkImageData.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageCast.h>
#include <vtkPNGWriter.h>

#include <iostream>
#include <filesystem>


namespace IOUtil
{
    std::string GetFileExtension(const char* fileDir)
    {
        const auto path = std::filesystem::path(fileDir);
        return path.extension().string();
    }

    bool PathExist(const char* fileDir)
    {
        const auto path = std::filesystem::path(fileDir);
        if(!std::filesystem::exists(path))
        {
            std::cerr << "ERROR: Directory " << path.string() << " does not exist!" << std::endl;
            return false;
        }

        return true;
    }

    bool IsValidDirectory(const char* fileDir)
    {
        const auto path = std::filesystem::path(fileDir);
        const auto parentDir = path.parent_path();
        if(!std::filesystem::is_directory(parentDir))
        {
            std::cerr << "ERROR: Directory " << parentDir.string() << " does not exist!" << std::endl;
            return false;
        }

        return true;
    }

    void CreateParentDirectory(const char* fileDir)
    {
        const auto path = std::filesystem::path(fileDir);
        const auto parentDir = path.parent_path();

        std::cout << "Creating Directory: " << parentDir << std::endl;
        std::filesystem::create_directories(parentDir);
    }

    vtkSmartPointer<vtkPolyData> ReadMesh(const char* fileDir)
    {
        std::cout << "Reading " << fileDir << std::endl;

        if(!PathExist(fileDir))
            throw std::runtime_error("ReadMesh(). Path does not exist!");

        const auto extension = GetFileExtension(fileDir);
        vtkSmartPointer<vtkPolyData> polyData;
        if(extension == ".ply")
        {
            auto reader = vtkSmartPointer<vtkPLYReader>::New();
            reader->SetFileName(fileDir);
            reader->Update();
            polyData = reader->GetOutput();
        }
        else if(extension == ".obj")
        {
            auto reader = vtkSmartPointer<vtkOBJReader>::New();
            reader->SetFileName(fileDir);
            reader->Update();
            polyData = reader->GetOutput();
        }
        else if(extension == ".stl")
        {
            auto reader = vtkSmartPointer<vtkSTLReader>::New();
            reader->SetFileName(fileDir);
            reader->Update();
            polyData = reader->GetOutput();
        }
        else if(extension == ".vtk")
        {
            auto reader = vtkSmartPointer<vtkPolyDataReader>::New();
            reader->SetFileName(fileDir);
            reader->Update();
            polyData = reader->GetOutput();
        }
        else
        {
            throw std::runtime_error("ReadMesh(). ERROR: unsupported file extension!");
        }

        printf("Poly data vertices cnt: %lld, cells cnt: %lld\n", polyData->GetNumberOfPoints(), polyData->GetNumberOfCells());

        return polyData;
    }

    void WriteMesh(const char* fileDir, vtkPolyData* polyData)
    {
        if(!IsValidDirectory(fileDir))
            CreateParentDirectory(fileDir);

        const auto extension = GetFileExtension(fileDir);
        if(extension == ".ply")
        {
            auto writer = vtkSmartPointer<vtkPLYWriter>::New();
            writer->SetFileName(fileDir);
            writer->SetInputData(polyData);
            writer->Write();
        }
        else if(extension == ".obj")
        {
            auto writer = vtkSmartPointer<vtkOBJWriter>::New();
            writer->SetFileName(fileDir);
            writer->SetInputData(polyData);
            writer->Write();
        }
        else if(extension == ".stl")
        {
            auto writer = vtkSmartPointer<vtkSTLWriter>::New();
            writer->SetFileName(fileDir);
            writer->SetInputData(polyData);
            writer->Write();
        }
        else
        {
            std::cerr << "ERROR: unsupported file extension" << std::endl;
            return;
        }

        std::cout << "Poly data is written to " << fileDir << std::endl;
    }

    void WriteColorMesh(const char* fileDir, vtkPolyData* polyData)
    {
        if(GetFileExtension(fileDir) != ".ply")
        {
            std::cerr << "ERROR: wrong extension. Should be .ply" << std::endl;
            return;
        }

        if(!IsValidDirectory(fileDir))
            CreateParentDirectory(fileDir);

        auto plyWriter = vtkSmartPointer<vtkPLYWriter>::New();
        plyWriter->SetFileName(fileDir);
        plyWriter->SetInputData(polyData);
        plyWriter->SetArrayName("Colors"); //set colors
        plyWriter->Write();

        std::cout << "Saved colored polydata to " << fileDir << std::endl;
    }

    void WriteFeatureVectorToFile(const char* dir, const std::vector<double>& dataVector, const bool append)
    {
        if(!IsValidDirectory(dir))
            CreateParentDirectory(dir);

        std::ofstream outfile;

        if(append)
            outfile.open(dir, std::ios_base::app);
        else
            outfile.open(dir, std::ios_base::trunc);

        for(const auto& value: dataVector)
            outfile << value << std::endl;

        outfile.close();
        std::cout << "Saved feature file to " << dir << std::endl;
    }

    vtkSmartPointer<vtkImageData> ReadImageData(const char* folder)
    {
        if(!PathExist(folder))
        {
            throw std::runtime_error("ReadImageData(). Folder does not exist!");
        }

        const auto path = std::filesystem::path(folder);
        std::cout << "reading " << path << std::endl;

        auto reader = vtkSmartPointer<vtkDICOMImageReader>::New();
        reader->SetDirectoryName(folder);
        reader->Update();

        return reader->GetOutput();
    }

    void WritePng(const char* fileDir, vtkImageData* imageData, const bool isUnsignedShort)
    {
        if(!IsValidDirectory(fileDir))
        {
            std::cout << "Creating Directory..." << std::endl;

            const auto path = std::filesystem::path(fileDir);
            const auto parentDir = path.parent_path();
            std::filesystem::create_directories(parentDir);
        }

        //png only supports unsigned char/short so cast it before export
        auto castFilter = vtkSmartPointer<vtkImageCast>::New();
        castFilter->SetInputData(imageData);
        isUnsignedShort ? castFilter->SetOutputScalarTypeToUnsignedShort() : castFilter->SetOutputScalarTypeToUnsignedChar();
        castFilter->Update();
        auto output = castFilter->GetOutput();

        auto writer = vtkSmartPointer<vtkPNGWriter>::New();
        writer->SetFileName(fileDir);
        writer->SetInputData(output);
        writer->Write();
    }
}
