#include "TopologyUtil.h"

#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkExtractEdges.h>
#include <vtkGenerateIds.h>
#include <vtkFeatureEdges.h>
#include <vtkLine.h>

#include <queue>
#include <stack>


namespace TopologyUtil
{
    struct PairHash
    {
        size_t operator()(const std::pair<int, int>& p) const
        {
            size_t h1 = std::hash<int>{}(p.first);
            size_t h2 = std::hash<int>{}(p.second);

            return h1 ^ (h2 << 1);
        }
    };

    std::vector<std::pair<int, int>> GetEdgeVids(vtkPolyData* polyData)
    {
        const auto arrayName = "OriginalIDs";

        auto originalIDs = vtkSmartPointer<vtkIntArray>::New();
        originalIDs->SetName(arrayName);
        originalIDs->SetNumberOfComponents(1);
        originalIDs->SetNumberOfTuples(polyData->GetNumberOfPoints());

        for(vtkIdType i = 0; i < polyData->GetNumberOfPoints(); ++i)
        {
            originalIDs->SetValue(i, i);
        }

        polyData->GetPointData()->AddArray(originalIDs);

        auto extractEdges = vtkSmartPointer<vtkExtractEdges>::New();
        extractEdges->SetInputData(polyData);
        extractEdges->Update();

        auto output = extractEdges->GetOutput();
        vtkCellArray* lines = output->GetLines();
        vtkDataArray* originalIDsArray = output->GetPointData()->GetArray(arrayName);

        std::vector<std::pair<int, int>> edgeVidsVector;
        if(!originalIDsArray)
        {
            std::cerr << "Error: original ids array not found in output." << std::endl;
            return edgeVidsVector;
        }

        auto pointIds = vtkSmartPointer<vtkIdList>::New();
        lines->InitTraversal();
        while(lines->GetNextCell(pointIds))
        {
            if(pointIds->GetNumberOfIds() == 2)//each line has 2 points
            {
                const auto startId = pointIds->GetId(0);
                const auto endId = pointIds->GetId(1);

                //look up the original IDs using the local point IDs from the output
                const auto startVid = originalIDsArray->GetTuple1(startId);
                const auto endVid = originalIDsArray->GetTuple1(endId);

                edgeVidsVector.push_back({static_cast<int>(startVid), static_cast<int>(endVid)});
            }
        }

        return edgeVidsVector;
    }

    std::vector<std::vector<int>> GetTriangleVids(vtkPolyData* polyData)
    {
        const auto count = polyData->GetNumberOfCells();
        std::vector<std::vector<int>> triangles(count);

        for(auto i = 0; i < count; i++)
        {
            auto pointIdList = vtkSmartPointer<vtkIdList>::New();
            polyData->GetCellPoints(i, pointIdList);;

            const auto vidsCount = pointIdList->GetNumberOfIds();
            std::vector<int> vids(vidsCount);

            for(int j = 0; j < vidsCount; j++)
            {
                vids[j] = pointIdList->GetId(j);
            }

            triangles[i] = vids;
        }

        return triangles;
    }

    std::unordered_set<int> GetNeighborVids(vtkPolyData* polyData, const int vid)
    {
        //get all cells that vertex 'id' is a part of
        auto cellIdList = vtkSmartPointer<vtkIdList>::New();
        polyData->GetPointCells(vid, cellIdList);

        std::unordered_set<int> neighborsSet;

        for(size_t i = 0; i < cellIdList->GetNumberOfIds(); i++)
        {
            //get all vids in cell
            auto cellVids = vtkSmartPointer<vtkIdList>::New();
            polyData->GetCellPoints(cellIdList->GetId(i), cellVids);;

            for(size_t j = 0; j < cellVids->GetNumberOfIds(); ++j)
            {
                auto neighborVid = cellVids->GetId(j);
                if(neighborVid != vid)
                    neighborsSet.insert(neighborVid);
            }
        }

        return neighborsSet;
    }

    std::vector<int> GetNRingNeighbors(vtkPolyData* polyData, const int vid, const int nring)
    {
        std::vector<bool> visited(polyData->GetNumberOfPoints(), false);
        std::queue<std::pair<int, int>> queue;//<vid, ringID> pair
        std::vector<int> nringNeighbors;

        queue.push(std::make_pair(vid, 0));
        visited[vid] = true;

        while(!queue.empty())
        {
            auto pair = queue.front();
            auto vertexID = pair.first;
            auto ringID = pair.second;
            queue.pop();

            nringNeighbors.push_back(vertexID);

            if(ringID >= nring)
                continue;

            auto neighbors = GetNeighborVids(polyData, vertexID);
            for(const auto& neighborVid: neighbors)
            {
                if(!visited[neighborVid])
                {
                    visited[neighborVid] = true;
                    queue.push(std::make_pair(neighborVid, ringID + 1));
                }
            }
        }

        return nringNeighbors;
    }

    std::vector<int> GetUnsortedBoundaryVids(vtkPolyData* polyData)
    {
        const auto arrayName = "ids";

        auto idFilter = vtkSmartPointer<vtkGenerateIds>::New();
        idFilter->SetInputData(polyData);
        idFilter->SetPointIds(true);
        idFilter->SetCellIds(false);
        idFilter->SetPointIdsArrayName(arrayName);
        idFilter->SetCellIdsArrayName(arrayName);
        idFilter->Update();

        auto edges = vtkSmartPointer<vtkFeatureEdges>::New();
        edges->SetInputData(idFilter->GetOutput());
        edges->BoundaryEdgesOn();
        edges->ManifoldEdgesOff();
        edges->NonManifoldEdgesOff();
        edges->FeatureEdgesOff();
        edges->Update();

        auto output = edges->GetOutput();

        const auto pointsCnt = output->GetNumberOfPoints();
        const auto cellsCnt = output->GetNumberOfCells();
        if(pointsCnt == 0 && cellsCnt == 0)//closed mesh
        {
            return {};
        }

        auto array = output->GetPointData()->GetArray(arrayName);

        const auto count = array->GetNumberOfValues();
        std::vector<int> boundaryVids(count);
        for(int i = 0; i < count; ++i)
        {
            boundaryVids[i] = array->GetTuple1(i);
        }

        return boundaryVids;
    }

    std::vector<int> GetBoundaryLoopVids(vtkPolyData* polyData)
    {
        const auto unsortedBoundaryVids = GetUnsortedBoundaryVids(polyData);

        const auto vertCnt = polyData->GetNumberOfPoints();
        std::vector<bool> visited(vertCnt, false);
        std::vector<bool> isBoundary(vertCnt, false);

        for(const auto vid: unsortedBoundaryVids)
            isBoundary[vid] = true;

        std::vector<int> dfsResult;
        std::stack<int> stack;

        const auto seed = unsortedBoundaryVids[0];
        stack.push(seed);
        visited[seed] = true;

        while(!stack.empty())
        {
            const auto vid = stack.top();
            dfsResult.push_back(vid);
            stack.pop();

            auto neighbors = GetNeighborVids(polyData, vid);
            for(auto neighbor: neighbors)
            {
                if(!visited[neighbor] && isBoundary[neighbor])
                {
                    stack.push(neighbor);
                    visited[neighbor] = true;
                }
            }
        }

        return dfsResult;
    }

    std::vector<std::pair<int, int>> GetBoundaryEdgeVids(vtkPolyData* polyData)
    {
        std::unordered_map<std::pair<int, int>, int, PairHash> edgeVidsCountMap;

        //iterate through all triangles
        vtkCellArray* polys = polyData->GetPolys();
        polys->InitTraversal();

        auto pointIds = vtkSmartPointer<vtkIdList>::New();
        while(polys->GetNextCell(pointIds))
        {
            if(pointIds->GetNumberOfIds() != 3)//a triangle has 3 points
                continue;

            //get the three edges of the current triangle
            for(int i = 0; i < 3; ++i)
            {
                const auto v1 = pointIds->GetId(i);
                const auto v2 = pointIds->GetId((i + 1) % 3);

                //create a canonical edge key
                const auto key = (v1 < v2) ? std::make_pair(v1, v2) : std::make_pair(v2, v1);
                edgeVidsCountMap[key]++;
            }
        }

        //extract boundary edges
        std::vector<std::pair<int, int>> boundaryEdgeVids;
        for(const auto& [edgeVids, count]: edgeVidsCountMap)
        {
            if(count == 1)
            {
                boundaryEdgeVids.push_back(edgeVids);
            }
        }

        return boundaryEdgeVids;
    }

    vtkSmartPointer<vtkCellArray> GetTriangleTopologyAsCellArray(const std::vector<std::vector<int>>& triangleVids)
    {
        auto cells = vtkSmartPointer<vtkCellArray>::New();
        auto connectivity = vtkSmartPointer<vtkIdTypeArray>::New();

        const auto numTriangles = triangleVids.size();

        //each triangle needs 3 vertices and a leading '3' for the count.
        connectivity->SetNumberOfComponents(1);
        connectivity->SetNumberOfValues(numTriangles * 4);

        vtkIdType* connData = connectivity->GetPointer(0);
        vtkIdType connIdx = 0;

        //fill the connectivity array directly
        for(const auto& tri: triangleVids)
        {
            connData[connIdx++] = 3;//points number in the cell
            connData[connIdx++] = tri[0];
            connData[connIdx++] = tri[1];
            connData[connIdx++] = tri[2];
        }

        cells->SetCells(numTriangles, connectivity);

        return cells;
    }
}
