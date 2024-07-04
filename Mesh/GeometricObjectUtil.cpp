#include "GeometricObjectUtil.h"

#include <vtkLineSource.h>
#include <vtkVectorOperators.h>
#include <vtkCubeSource.h>


namespace GeometricObjectUtil
{
    vtkSmartPointer<vtkPolyData> GetLinePolyData(const vtkVector3d& start, const vtkVector3d& end)
    {
        vtkNew<vtkLineSource> lineSource;
        lineSource->SetPoint1(start.GetData());
        lineSource->SetPoint2(end.GetData());
        lineSource->Update();

        return lineSource->GetOutput();
    }

    vtkSmartPointer<vtkPolyData> GetLinePolyData(const vtkVector3d& start, const vtkVector3d& dir, const double length)
    {
        return GetLinePolyData(start, start + dir * length);
    }

    vtkSmartPointer<vtkPolyData> GetCubePolyData(const vtkVector3d& pos, double size)
    {
        auto cube = vtkSmartPointer<vtkCubeSource>::New();
        cube->SetCenter(pos.GetData());
        cube->SetXLength(size);
        cube->SetYLength(size);
        cube->SetZLength(size);
        cube->Update();

        return cube->GetOutput();
    }
}
