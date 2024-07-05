#pragma once

#include <vtkSmartPointer.h>
#include <filesystem>


class vtkPolyData;

namespace UnitTest
{
    class MeshFeatureTest
    {
    public:
        /**
         * Setup of the mesh feature test, given input file directory and output folder directory
         */
        void SetUp(const char* inputFile, const std::filesystem::path& outputDir);

        void TestMeanCurvatureFeature();
        void TestGaussianCurvatureFeature();
        void TestPCAFeature();


    private:
        vtkSmartPointer<vtkPolyData> mesh;
        std::filesystem::path outputDir;
    };
}
