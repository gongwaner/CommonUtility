#include "TopologyUtil.h"

#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkGenerateIds.h>
#include <vtkFeatureEdges.h>
#include <vtkLine.h>

#include <queue>
#include <stack>


namespace TopologyUtil
{
    std::vector<std::vector<int>> GetTriangleVidsVector(vtkPolyData* polyData)
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
        //add a point data array with the original point IDs.
        auto idFilter = vtkSmartPointer<vtkGenerateIds>::New();
        idFilter->SetInputData(polyData);
        idFilter->SetPointIds(true);
        idFilter->SetCellIds(false);
        idFilter->SetPointIdsArrayName("OriginalIDs");
        idFilter->Update();

        auto idFilterOutput = idFilter->GetOutput();

        //use vtkFeatureEdges on the output of the idFilter. The "OriginalIDs" array will be preserved in the output.
        auto featureEdges = vtkSmartPointer<vtkFeatureEdges>::New();
        featureEdges->SetInputData(idFilterOutput);
        featureEdges->BoundaryEdgesOn();
        featureEdges->ManifoldEdgesOff();
        featureEdges->NonManifoldEdgesOff();
        featureEdges->FeatureEdgesOff();
        featureEdges->Update();

        auto boundaryEdges = featureEdges->GetOutput();
        vtkCellArray* lines = boundaryEdges->GetLines();

        const auto edgesCnt = lines->GetNumberOfCells();
        std::vector<std::pair<int, int>> edges;

        //retrieve the point data array containing the original IDs
        vtkDataArray* originalIDs = boundaryEdges->GetPointData()->GetArray("OriginalIDs");
        if(!originalIDs)
        {
            std::cerr << "Error: OriginalIDs array not found in output." << std::endl;
            return edges;
        }

        //traverse all  edges
        for(auto i = 0; i < edgesCnt; i++)
        {
            const std::string className = boundaryEdges->GetCell(i)->GetClassName();
            if(className == "vtkLine")
            {
                auto line = dynamic_cast<vtkLine*>(boundaryEdges->GetCell(i));;

                const auto startVid = originalIDs->GetTuple1(line->GetPointIds()->GetId(0));
                const auto endVid = originalIDs->GetTuple1(line->GetPointIds()->GetId(1));

                edges.push_back({startVid, endVid});
            }
        }

        return edges;
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
