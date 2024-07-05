#include "MeshUtil.h"

#include <vtkPolyData.h>
#include <vtkOBBTree.h>
#include <vtkAppendPolyData.h>
#include <vtkPointData.h>


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
        auto scalarArray = polyData->GetPointData()->GetScalars(Color::ColorArrayName.c_str());
        if(!scalarArray)
        {
            EnableMeshColor(polyData);
            scalarArray = polyData->GetPointData()->GetScalars(Color::ColorArrayName.c_str());
        }

        auto colorPtr = static_cast<unsigned char*>(scalarArray->GetVoidPointer(0));

        const int cnt = 4;
        for(int i = 0; i < cnt; ++i)
            colorPtr[vid * cnt + i] = color[i];

        polyData->GetPointData()->Modified();
    }
}
