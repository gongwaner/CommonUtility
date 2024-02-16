#include <iostream>
#include <filesystem>

#include <vtkPolyData.h>

#include "IO/IOUtil.h"
#include "Test/MeshFeatureTest.h"

int main()
{
    auto cwd = std::filesystem::current_path();
    printf("current working dir: %s\n", cwd.string().c_str());

    std::filesystem::path dataDir;

#ifdef _WIN32
    auto projectDir = cwd.parent_path();
    printf("projectDir dir: %s\n", projectDir.string().c_str());
#elif __APPLE__
    auto projectDir = cwd.parent_path().parent_path().parent_path().parent_path();
    printf("projectDir dir: %s\n", projectDir.string().c_str());
#else
    printf("Operating system not supported!\n");
#endif

    dataDir = projectDir.append("Data");
    printf("dataDir dir: %s\n", dataDir.string().c_str());
    if(!std::filesystem::is_directory(dataDir))
    {
        printf("data directory does not exist!\n");
        return 1;
    }

    auto inputFile = dataDir;
    inputFile = inputFile.append("lowerJaw.stl");
    printf("reading %s\n", inputFile.string().c_str());


    auto polyData = IOUtil::ReadMesh(inputFile.c_str());
    auto outDir = dataDir;
    outDir = outDir.append("output");
    const auto outFile = outDir.append("testResult.ply");
    std::cout << "outFile: " << outFile << std::endl;

    MeshFeatureTest::WriteMeshMeanCurvatureFeature(polyData, outFile.c_str());

    return 0;
}