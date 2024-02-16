class vtkPolyData;

namespace MeshFeatureTest
{
    void WriteMeshMeanCurvatureFeature(vtkPolyData* polydata, const char* outDir);
    void WriteMeshPCAFeature(vtkPolyData* polydata, const char* outDir);
}


