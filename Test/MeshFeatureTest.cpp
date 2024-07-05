#include "MeshFeatureTest.h"

#include <vtkPolyData.h>

#include "../MeshFeature/MeshFeature.h"
#include "../MeshFeature/MeshFeatureUtil.h"
#include "../IO/IOUtil.h"


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

        auto outFile = outputDir;
        outFile.append("mean_curvature_feature.ply");
        IOUtil::WriteColorMesh(outFile.string().c_str(), mesh);
    }

    void MeshFeatureTest::TestGaussianCurvatureFeature()
    {
        auto meanCurvature = MeshFeature::GetGaussianCurvature(mesh);
        MeshFeatureUtil::ClampVector(meanCurvature, -1.0, 0.5);

        MeshFeatureUtil::ColorMapping(mesh, meanCurvature);

        auto outFile = outputDir;
        outFile.append("gaussian_curvature_feature.ply");
        IOUtil::WriteColorMesh(outFile.string().c_str(), mesh);
    }

    void MeshFeatureTest::TestPCAFeature()
    {
        auto meanCurvature = MeshFeature::GetPCAFeature(mesh, 3);
        MeshFeatureUtil::ClampVector(meanCurvature, 50.0, 300.0);

        MeshFeatureUtil::ColorMapping(mesh, meanCurvature);

        auto outFile = outputDir;
        outFile.append("pca_feature.ply");
        IOUtil::WriteColorMesh(outFile.string().c_str(), mesh);
    }
}
