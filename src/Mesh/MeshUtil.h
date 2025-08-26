#pragma  once

#include <vtkVector.h>

#include "Color.h"


class vtkPolyData;
class vtkOBBTree;
class vtkBoundingBox;
class vtkPoints;
class vtkCellArray;
class vtkTriangle;

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

    std::vector<vtkVector3d> GetTriPoints(vtkPolyData* mesh, int tid);

    vtkBoundingBox GetTriBounds(vtkSmartPointer<vtkPolyData> mesh, int tid);
    vtkBoundingBox GetTrisBounds(vtkPolyData* mesh, const std::vector<int>& tids);

    vtkVector3d GetCellCenter(vtkPolyData* polyData, size_t cellID);

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

    /**
     * Given points and cells topology, return the mesh
     */
    vtkSmartPointer<vtkPolyData> GetPolyData(vtkPoints* points, vtkCellArray* cells);
    vtkSmartPointer<vtkPolyData> GetPolyData(const std::vector<vtkVector3d>& points, vtkSmartPointer<vtkCellArray> cells);
    vtkSmartPointer<vtkPolyData> GetPolyData(const std::vector<vtkVector3d>& points, const std::vector<vtkSmartPointer<vtkTriangle>>& triangles);
}
