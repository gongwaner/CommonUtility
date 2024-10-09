#include <iostream>
#include <filesystem>

#include <vtkPolyData.h>

#include "Test/TestUtil.h"
#include "Test/MeshFeatureTest.h"


int main()
{
    const auto dataDir = TestUtil::GetDataDir();
    printf("dataDir dir: %s\n", dataDir.string().c_str());

    if(!std::filesystem::is_directory(dataDir))
    {
        printf("data directory does not exist!\n");
        return 1;
    }

    const auto inputFile = dataDir / "lowerJaw.stl";
    const auto outDir = TestUtil::GetOutputDir();

    UnitTest::MeshFeatureTest meshFeatureTest;
    meshFeatureTest.SetUp(inputFile.string().c_str(), outDir);
    meshFeatureTest.TestMeanCurvatureFeature();
    meshFeatureTest.TestGaussianCurvatureFeature();
    meshFeatureTest.TestPCAFeature();

    return 0;
}
