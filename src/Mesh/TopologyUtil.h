#pragma once

#include <vtkVector.h>

#include <unordered_set>


class vtkPolyData;
class vtkCellArray;

namespace TopologyUtil
{
    std::vector<std::vector<int>> GetTriangleVidsVector(vtkPolyData* polyData);

    /**
     * Extract the topological neighbors of given vid
     */
    std::unordered_set<int> GetNeighborVids(vtkPolyData* polyData, int vid);

    /**
     * Extract the topological n ring neighbors vids of given vid
     * @return vector of n ring neighbor vids. Note: the input vertex itself is included in result.
     */
    std::vector<int> GetNRingNeighbors(vtkPolyData* polyData, int vid, int nring);

    /**
     * Return boundary vertices ids of the given mesh.
     * Note: the returned result is a list of UNSORTED vertices ids.
     */
    std::vector<int> GetUnsortedBoundaryVids(vtkPolyData* polyData);

    /**
     *
     * Return boundary vertices ids in a loop of the given mesh.
     * The returned result is sorted.
     */
    std::vector<int> GetBoundaryLoopVids(vtkPolyData* polyData);

    /**
     *
     * Return boundary edges of the given mesh as a list of {startVid, endVid}
     */
    std::vector<std::pair<int, int>> GetBoundaryEdgeVids(vtkPolyData* polyData);

    /**
     *
     * Given a list of triangle vids, return the topology as vtkCellArray
     */
    vtkSmartPointer<vtkCellArray> GetTriangleTopologyAsCellArray(const std::vector<std::vector<int>>& triangleVids);
}
