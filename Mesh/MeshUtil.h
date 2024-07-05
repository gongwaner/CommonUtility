#pragma  once

#include <vtkVector.h>

#include "../Common/Color.h"


class vtkPolyData;
class vtkOBBTree;

namespace MeshUtil
{
    /**
     * Print mesh vertices and cells count
     */
    void PrintMeshInfo(vtkPolyData* polyData);
    void PrintMeshInfo(const std::string& meshName, vtkPolyData* polyData);

    vtkVector3d GetMeshDimension(vtkPolyData* polyData);
    vtkSmartPointer<vtkOBBTree> GetOBBTree(vtkPolyData* polyData);

    /**
     * Combine a vector of meshes into a single mesh.
     * This function will not change the input meshes
     */
    vtkSmartPointer<vtkPolyData> GetCombinedPolyData(const std::vector<vtkSmartPointer<vtkPolyData>>& meshes);

    /**
     * Append appendFrom mesh to appendTo mesh.
     * This function will change appendTo mesh
     */
    void AppendMesh(vtkSmartPointer<vtkPolyData> appendFrom, vtkSmartPointer<vtkPolyData>& appendTo);

    bool HasColorInfo(vtkPolyData* polyData);
    void EnableMeshColor(vtkSmartPointer<vtkPolyData> polyData, const vtkVector4d& initColor = Color::White);
    void SetVertexColor(vtkSmartPointer<vtkPolyData> polyData, int vid, const vtkVector4d& color);
}
