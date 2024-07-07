#include "MeshUtil.h"

#include <vtkPolyData.h>
#include <vtkOBBTree.h>
#include <vtkVectorOperators.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkPointData.h>
#include <vtkTriangle.h>
#include <vtkIdFilter.h>
#include <vtkFeatureEdges.h>

#include <queue>
#include <stack>


namespace MeshUtil
{
    void PrintMeshInfo(vtkPolyData* polyData)
    {
        std::cout << "mesh vertices count: " << polyData->GetNumberOfPoints() << ", " << polyData->GetNumberOfCells() << std::endl;
    }

    void PrintMeshInfo(const std::string& meshName, vtkPolyData* polyData)
    {
        std::cout << meshName << " vertices count: " << polyData->GetNumberOfPoints() << ", " << polyData->GetNumberOfCells() << std::endl;
    }

    vtkVector3d GetMeshDimension(vtkPolyData* polyData)
    {
        auto polyBounds = polyData->GetBounds();
        const auto dimensionX = polyBounds[1] - polyBounds[0];
        const auto dimensionY = polyBounds[3] - polyBounds[2];
        const auto dimensionZ = polyBounds[5] - polyBounds[4];

        return {dimensionX, dimensionY, dimensionZ};
    }

    vtkSmartPointer<vtkOBBTree> GetOBBTree(vtkPolyData* polyData)
    {
        auto obbTree = vtkSmartPointer<vtkOBBTree>::New();
        obbTree = vtkSmartPointer<vtkOBBTree>::New();
        obbTree->SetDataSet(polyData);
        obbTree->BuildLocator();

        return obbTree;
    }

    vtkVector3d GetCellCenter(vtkPolyData* polyData, const vtkIdType cellID)
    {
        auto pointIdList = vtkSmartPointer<vtkIdList>::New();
        polyData->GetCellPoints(cellID, pointIdList);;

        vtkVector3d cellCenter(0, 0, 0);
        for(vtkIdType i = 0; i < pointIdList->GetNumberOfIds(); i++)
            cellCenter += vtkVector3d(polyData->GetPoint(pointIdList->GetId(i)));

        for(int i = 0; i < 3; ++i)
            cellCenter[i] /= pointIdList->GetNumberOfIds();

        return cellCenter;
    }

    std::unordered_set<int> GetNeighborVids(vtkPolyData* polyData, const int vid)
    {
        //get all cells that vertex 'id' is a part of
        auto cellIdList = vtkSmartPointer<vtkIdList>::New();
        polyData->GetPointCells(vid, cellIdList);

        std::unordered_set<int> neighborsSet;

        for(vtkIdType i = 0; i < cellIdList->GetNumberOfIds(); i++)
        {
            //get all vids in cell
            auto cellVids = vtkSmartPointer<vtkIdList>::New();
            polyData->GetCellPoints(cellIdList->GetId(i), cellVids);;

            for(vtkIdType j = 0; j < cellVids->GetNumberOfIds(); ++j)
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

        auto idFilter = vtkSmartPointer<vtkIdFilter>::New();
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

        auto array = edges->GetOutput()->GetPointData()->GetArray(arrayName);

        std::vector<int> boundaryVids(array->GetNumberOfValues());
        for(int i = 0; i < array->GetNumberOfValues(); ++i)
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

    vtkSmartPointer<vtkPolyData> GetLargestComponent(vtkPolyData* polyData)
    {
        auto connectivityFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
        connectivityFilter->SetInputData(polyData);
        connectivityFilter->SetExtractionModeToLargestRegion();
        connectivityFilter->Update();

        return connectivityFilter->GetOutput();
    }

    std::vector<vtkSmartPointer<vtkPolyData>> GetAllComponents(vtkSmartPointer<vtkPolyData> polyData, const int minCellsCnt)
    {
        auto connectivityFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
        connectivityFilter->SetInputData(polyData);
        connectivityFilter->SetExtractionModeToAllRegions();
        connectivityFilter->Update();

        int componentCnt = connectivityFilter->GetNumberOfExtractedRegions();
        printf("component cnt: %i\n", componentCnt);

        std::vector<vtkSmartPointer<vtkPolyData>> componentVec;
        componentVec.reserve(componentCnt);

        for(int regionID = 0; regionID < componentCnt; ++regionID)
        {
            //select the region to extract
            connectivityFilter->SetExtractionModeToSpecifiedRegions();
            connectivityFilter->AddSpecifiedRegion(regionID);
            connectivityFilter->Update();

            auto component = connectivityFilter->GetOutput();
            if(component->GetNumberOfCells() >= minCellsCnt)
            {
                auto copy = vtkSmartPointer<vtkPolyData>::New();
                copy->DeepCopy(component);
                componentVec.push_back(copy);
            }

            //delete selected component
            connectivityFilter->DeleteSpecifiedRegion(regionID);
            connectivityFilter->Update();
        }

        return componentVec;
    }

    vtkSmartPointer<vtkPolyData> GetCombinedPolyData(const std::vector<vtkSmartPointer<vtkPolyData>>& meshes)
    {
        if(meshes.size() == 1)
            return meshes[0];

        auto appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
        for(const auto& mesh: meshes)
            appendFilter->AddInputData(mesh);
        appendFilter->Update();

        return appendFilter->GetOutput();
    }

    void AppendMesh(vtkSmartPointer<vtkPolyData> appendFrom, vtkSmartPointer<vtkPolyData>& appendTo)
    {
        auto appendMesh = vtkSmartPointer<vtkAppendPolyData>::New();
        appendMesh->AddInputData(appendFrom);
        appendMesh->AddInputData(appendTo);
        appendMesh->Update();

        appendTo = appendMesh->GetOutput();
    }

    bool HasColorInfo(vtkPolyData* polyData)
    {
        return polyData->GetPointData()->GetScalars(Color::ColorArrayName.c_str());
    }

    void EnableMeshColor(vtkSmartPointer<vtkPolyData> polyData, const vtkVector4d& initColor)
    {
        auto colorArray = vtkSmartPointer<vtkUnsignedCharArray>::New();
        colorArray->SetNumberOfComponents(4);
        colorArray->SetName(Color::ColorArrayName.c_str());

        for(int i = 0; i < polyData->GetNumberOfPoints(); ++i)
        {
            colorArray->InsertNextTuple4(initColor[0], initColor[1], initColor[2], initColor[3]);
        }

        polyData->GetPointData()->AddArray(colorArray);
    }

    void SetVertexColor(vtkSmartPointer<vtkPolyData> polyData, const int vid, const vtkVector4d& color)
    {
        if(!HasColorInfo(polyData))
            EnableMeshColor(polyData);

        auto scalarArray = polyData->GetPointData()->GetScalars(Color::ColorArrayName.c_str());
        auto colorPtr = static_cast<unsigned char*>(scalarArray->GetVoidPointer(0));

        const int cnt = 4;
        for(int i = 0; i < cnt; ++i)
            colorPtr[vid * cnt + i] = color[i];

        polyData->GetPointData()->Modified();
    }

    vtkSmartPointer<vtkPolyData> GetPolyData(vtkPoints* points, vtkCellArray* cells)
    {
        auto mesh = vtkSmartPointer<vtkPolyData>::New();
        mesh->SetPoints(points);
        mesh->SetPolys(cells);

        return mesh;
    }

    vtkSmartPointer<vtkPolyData> GetPolyData(const std::vector<vtkVector3d>& points, vtkSmartPointer<vtkCellArray> cells)
    {
        auto pointsArray = vtkSmartPointer<vtkPoints>::New();
        for(const auto& point: points)
            pointsArray->InsertNextPoint(point.GetData());

        return GetPolyData(pointsArray, cells);
    }

    vtkSmartPointer<vtkPolyData> GetPolyData(const std::vector<vtkVector3d>& points, const std::vector<vtkSmartPointer<vtkTriangle>>& triangles)
    {
        auto pointsArray = vtkSmartPointer<vtkPoints>::New();
        for(const auto& point: points)
            pointsArray->InsertNextPoint(point.GetData());

        auto cells = vtkSmartPointer<vtkCellArray>::New();
        for(const auto& tri: triangles)
            cells->InsertNextCell(tri);

        return GetPolyData(pointsArray, cells);
    }
}
