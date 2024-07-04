#pragma  once

#include <vtkVector.h>


class vtkPolyData;
class vtkOBBTree;

namespace MeshUtil
{
    vtkVector3d GetMeshDimension(vtkPolyData* polyData);
    vtkSmartPointer<vtkOBBTree> GetOBBTree(vtkPolyData* polyData);
}
