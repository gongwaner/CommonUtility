#include "TestUtil.h"

#include <vtkAppendPolyData.h>
#include <vtkVectorOperators.h>
#include <vtkMatrix4x4.h>

#include "../Mesh/GeometricObjectUtil.h"
#include "../Mesh/MeshUtil.h"
#include "../Transformation/TransformUtil.h"


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
        MeshUtil::AppendMesh(cube, mesh);
    }

    void AppendCube(vtkSmartPointer<vtkPolyData>& mesh, const vtkVector3d& pos, double size, const vtkVector4d& color)
    {
        if(!MeshUtil::HasColorInfo(mesh))
            MeshUtil::EnableMeshColor(mesh);

        auto cube = GeometricObjectUtil::GetCubePolyData(pos, size);
        MeshUtil::EnableMeshColor(cube, color);

        MeshUtil::AppendMesh(cube, mesh);
    }

    void AppendLine(vtkSmartPointer<vtkPolyData>& mesh, const vtkVector3d& lineStart, const vtkVector3d& lineEnd, const double radius)
    {
        auto line = GeometricObjectUtil::GetLineMesh(lineStart, lineEnd, radius);
        MeshUtil::AppendMesh(line, mesh);
    }

    void AppendLine(vtkSmartPointer<vtkPolyData>& mesh, const std::pair<vtkVector3d, vtkVector3d>& line, double radius)
    {
        return AppendLine(mesh, line.first, line.second, radius);
    }

    void AppendLine(vtkSmartPointer<vtkPolyData>& mesh, const std::pair<vtkVector3d, vtkVector3d>& line, double radius, const vtkVector4d& color)
    {
        if(!MeshUtil::HasColorInfo(mesh))
            MeshUtil::EnableMeshColor(mesh);

        auto lineMesh = GeometricObjectUtil::GetLineMesh(line.first, line.second, radius);
        MeshUtil::EnableMeshColor(lineMesh, color);

        MeshUtil::AppendMesh(lineMesh, mesh);
    }
}
