#include "MeshFeatureTest.h"

#include <vtkPolyData.h>

#include "MeshFeature.h"
#include "MeshFeatureUtil.h"
#include "IOUtil.h"


namespace UnitTest
{
    void MeshFeatureTest::SetUp(const char* inputFile, const std::filesystem::path& outputDir)
    {
        mesh = IOUtil::ReadMesh(inputFile);
        this->outputDir = outputDir;
    }

    void MeshFeatureTest::TestMeanCurvatureFeature()
    {
        auto meanCurvature = MeshFeature::GetMeanCurvature(mesh);
        MeshFeatureUtil::ClampVector(meanCurvature, -1.0, 0.5);

        MeshFeatureUtil::ColorMapping(mesh, meanCurvature);

        const auto outFile = outputDir / ("mean_curvature_feature" + IOUtil::PlyExtension);
        IOUtil::WriteColorMesh(outFile.string().c_str(), mesh);
    }

    void MeshFeatureTest::TestGaussianCurvatureFeature()
    {
        auto meanCurvature = MeshFeature::GetGaussianCurvature(mesh);
        MeshFeatureUtil::ClampVector(meanCurvature, -1.0, 0.5);

        MeshFeatureUtil::ColorMapping(mesh, meanCurvature);

        const auto outFile = outputDir / ("gaussian_curvature_feature" + IOUtil::PlyExtension);
        IOUtil::WriteColorMesh(outFile.string().c_str(), mesh);
    }

    void MeshFeatureTest::TestPCAFeature()
    {
        auto meanCurvature = MeshFeature::GetPCAFeature(mesh, 3);
        MeshFeatureUtil::ClampVector(meanCurvature, 50.0, 300.0);

        MeshFeatureUtil::ColorMapping(mesh, meanCurvature);

        const auto outFile = outputDir / ("pca_feature" + IOUtil::PlyExtension);
        IOUtil::WriteColorMesh(outFile.string().c_str(), mesh);
    }
}
