#include <iostream>
#include <filesystem>

#include "Test/MeshFeatureTest.h"


std::string GetDataDir()
{
    const std::filesystem::path cmakeDir(CMAKE_SOURCE_DIR);
    const auto dataDir = cmakeDir / "Data";

    return dataDir.string();
}

std::string GetOutputDir()
{
    const std::filesystem::path dataFolderDir(GetDataDir());
    const auto outputFolderDir = dataFolderDir / "output";

    return outputFolderDir.string();
}

int main()
{
    const auto dataDir = GetDataDir();
    std::cout << "data dir: " << dataDir << std::endl;

    if(!std::filesystem::is_directory(dataDir))
    {
        std::cerr << "data directory does not exist!" << std::endl;
        return 1;
    }

    const std::filesystem::path DataPath(dataDir);
    const auto inputFile = DataPath / "lowerJaw.stl";
    const auto outDir = GetOutputDir();

    //create the output folder if it does not exist
    if(!std::filesystem::exists(outDir))
    {
        std::cerr << "output directory " << outDir << " does not exist!" << std::endl;
        std::filesystem::create_directory(outDir);
    }

    UnitTest::MeshFeatureTest meshFeatureTest;
    meshFeatureTest.SetUp(inputFile.string().c_str(), outDir);
    meshFeatureTest.TestMeanCurvatureFeature();
    meshFeatureTest.TestGaussianCurvatureFeature();
    meshFeatureTest.TestPCAFeature();

    return 0;
}
