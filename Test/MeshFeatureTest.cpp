#include "MeshFeatureTest.h"

#include "../MeshFeature/MeshFeature.h"
#include "../MeshFeature/MeshFeatureUtil.h"
#include "../IO/IOUtil.h"

#include <vtkPolyData.h>


namespace MeshFeatureTest
{
    void WriteMeshMeanCurvatureFeature(vtkPolyData* polydata, const char* outDir)
    {
        auto meanCurvature = MeshFeature::GetMeanCurvature(polydata);
        MeshFeatureUtil::ClampVector(meanCurvature, -1.0, 0.5);

        MeshFeatureUtil::ColorMapping(polydata, meanCurvature);

        IOUtil::WriteColorMesh(outDir, polydata);
    }

    void WriteMeshPCAFeature(vtkPolyData* polydata, const char* outDir)
    {
        auto meanCurvature = MeshFeature::GetPCAFeature(polydata, 3);
        MeshFeatureUtil::ClampVector(meanCurvature, 50.0, 300.0);

        MeshFeatureUtil::ColorMapping(polydata, meanCurvature);

        IOUtil::WriteColorMesh(outDir, polydata);
    }
}
