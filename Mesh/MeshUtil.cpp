#include "MeshUtil.h"

#include <vtkPolyData.h>
#include <vtkOBBTree.h>

namespace MeshUtil
{
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
}
