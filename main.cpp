#include <iostream>
#include <filesystem>

#include <vtkPolyData.h>

#include "Test/TestUtil.h"
#include "Test/MeshFeatureTest.h"


int main()
{
    const auto dataDir = TestUtil::GetDataDir();
    printf("dataDir dir: %s\n", dataDir.c_str());

    if(!std::filesystem::is_directory(dataDir))
    {
        std::cerr << "data directory does not exist!" << std::endl;
        return 1;
    }

    const std::filesystem::path DataPath(dataDir);
    const auto inputFile = DataPath / "lowerJaw.stl";
    const auto outDir = TestUtil::GetOutputDir();

    UnitTest::MeshFeatureTest meshFeatureTest;
    meshFeatureTest.SetUp(inputFile.string().c_str(), outDir);
    meshFeatureTest.TestMeanCurvatureFeature();
    meshFeatureTest.TestGaussianCurvatureFeature();
    meshFeatureTest.TestPCAFeature();

    return 0;
}
