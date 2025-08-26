#include "MeshUtil.h"

#include <vtkPolyData.h>
#include <vtkOBBTree.h>
#include <vtkBoundingBox.h>
#include <vtkVectorOperators.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkPointData.h>
#include <vtkTriangle.h>


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

    std::vector<vtkVector3d> GetTriPoints(vtkPolyData* mesh, const int tid)
    {
        std::vector<vtkVector3d> triPoints;

        auto pointIdList = vtkSmartPointer<vtkIdList>::New();
        mesh->GetCellPoints(tid, pointIdList);

        for(int i = 0; i < pointIdList->GetNumberOfIds(); i++)
            triPoints.push_back(vtkVector3d(mesh->GetPoint(pointIdList->GetId(i))));

        return triPoints;
    }

    vtkBoundingBox GetTriBounds(vtkSmartPointer<vtkPolyData> mesh, const int tid)
    {
        vtkBoundingBox boundingBox;
        auto pointIdList = vtkSmartPointer<vtkIdList>::New();
        mesh->GetCellPoints(tid, pointIdList);

        for(int i = 0; i < pointIdList->GetNumberOfIds(); i++)
            boundingBox.AddPoint(mesh->GetPoint(pointIdList->GetId(i)));

        return boundingBox;
    }

    vtkBoundingBox GetTrisBounds(vtkPolyData* mesh, const std::vector<int>& tids)
    {
        vtkBoundingBox boundingBox;

        for(const auto tid: tids)
        {
            auto pointIdList = vtkSmartPointer<vtkIdList>::New();
            mesh->GetCellPoints(tid, pointIdList);

            for(int i = 0; i < pointIdList->GetNumberOfIds(); i++)
                boundingBox.AddPoint(mesh->GetPoint(pointIdList->GetId(i)));
        }

        return boundingBox;
    }

    vtkVector3d GetCellCenter(vtkPolyData* polyData, const size_t cellID)
    {
        auto pointIdList = vtkSmartPointer<vtkIdList>::New();
        polyData->GetCellPoints(cellID, pointIdList);;

        vtkVector3d cellCenter(0, 0, 0);
        for(size_t i = 0; i < pointIdList->GetNumberOfIds(); i++)
            cellCenter += vtkVector3d(polyData->GetPoint(pointIdList->GetId(i)));

        for(int i = 0; i < 3; ++i)
            cellCenter[i] /= pointIdList->GetNumberOfIds();

        return cellCenter;
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
