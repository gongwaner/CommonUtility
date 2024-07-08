#include "TestUtil.h"

#include <vtkAppendPolyData.h>
#include <vtkMatrix4x4.h>

#include "../Mesh/GeometricObjectUtil.h"
#include "../Mesh/MeshUtil.h"
#include "../Transformation/TransformUtil.h"


namespace TestUtil
{
    std::filesystem::path GetDataDir()
    {
        const auto cwd = std::filesystem::current_path();
        printf("current working dir: %s\n", cwd.string().c_str());

        std::filesystem::path dataDir;

#ifdef _WIN32
        auto projectDir = cwd.parent_path();
        printf("projectDir dir: %s\n", projectDir.string().c_str());
#elif __APPLE__
        auto projectDir = cwd.parent_path().parent_path().parent_path().parent_path();
        printf("projectDir dir: %s\n", projectDir.string().c_str());
#else
        printf("Operating system not supported!\n");
#endif

        return projectDir.append("Data");
    }

    std::filesystem::path GetOutputDir()
    {
        return GetDataDir() / "output";
    }

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
