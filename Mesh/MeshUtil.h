#pragma  once

#include <vtkVector.h>

#include <unordered_set>

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

    /**
     * Return {length, width, height} of the given mesh
     */
    vtkVector3d GetMeshDimension(vtkPolyData* polyData);

    vtkSmartPointer<vtkOBBTree> GetOBBTree(vtkPolyData* polyData);

    vtkVector3d GetCellCenter(vtkPolyData* polyData, vtkIdType cellID);

    /**
     * Extract the topological neighbors of given vid
     */
    std::unordered_set<int> GetNeighborVids(vtkPolyData* polyData, int vid);

    /**
   * Extract the topological n ring neighbors vids of given vid
   * @return vector of n ring neighbor vids. Note: the input vertex itself is included in result.
   */
    std::vector<int> GetNRingNeighbors(vtkPolyData* polyData, int vid, int nring);

    vtkSmartPointer<vtkPolyData> GetLargestComponent(vtkPolyData* polyData);

    /**
     * Get all connected components of a given input polydata
     * @param minCellsCnt for optimization purpose, skip components whose cell cnt < specified minimum cells cnt
     */
    std::vector<vtkSmartPointer<vtkPolyData>> GetAllComponents(vtkSmartPointer<vtkPolyData> polyData, int minCellsCnt);

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
