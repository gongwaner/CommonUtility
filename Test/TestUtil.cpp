#include "TestUtil.h"

#include <vtkAppendPolyData.h>

#include "../Mesh/GeometricObjectUtil.h"


namespace TestUtil
{
    void Print(const std::string& msg, const double vec[3])
    {
        std::cout << msg << vec[0] << ", " << vec[1] << ", " << vec[2] << std::endl;
    }

    void Print(const std::string& msg, const vtkVector3d& vec)
    {
        Print(msg, vec.GetData());
    }

    void AppendCube(vtkSmartPointer<vtkPolyData>& mesh, const vtkVector3d& pos, double size)
    {
        auto cube = GeometricObjectUtil::GetCubePolyData(pos, size);
        auto appendMesh = vtkSmartPointer<vtkAppendPolyData>::New();
        appendMesh->AddInputData(mesh);
        appendMesh->AddInputData(cube);
        appendMesh->Update();

        mesh = appendMesh->GetOutput();
    }
}
