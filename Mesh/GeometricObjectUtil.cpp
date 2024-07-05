#include "GeometricObjectUtil.h"

#include <vtkSphere.h>
#include <vtkSphereSource.h>
#include <vtkLineSource.h>
#include <vtkVectorOperators.h>
#include <vtkPlaneSource.h>
#include <vtkCubeSource.h>
#include <vtkVertexGlyphFilter.h>


namespace GeometricObjectUtil
{
    vtkSmartPointer<vtkSphere> GetSphereImplicitFunction(const vtkVector3d& sphereCenter, const double radius)
    {
        auto implicitFunction = vtkSmartPointer<vtkSphere>::New();
        implicitFunction->SetCenter(sphereCenter.GetData());
        implicitFunction->SetRadius(radius);

        return implicitFunction;
    }

    vtkSmartPointer<vtkPolyData> GetSpherePolyData(const vtkVector3d& center, const double radius, const int phiResolution, const int thetaResolution)
    {
        auto sphereSource = vtkSmartPointer<vtkSphereSource>::New();
        sphereSource->SetCenter(center.GetData());
        sphereSource->SetRadius(radius);
        sphereSource->SetPhiResolution(phiResolution);
        sphereSource->SetThetaResolution(thetaResolution);
        sphereSource->Update();

        return sphereSource->GetOutput();
    }

    vtkSmartPointer<vtkPolyData> GetPointPolyData(const vtkVector3d& point)
    {
        auto points = vtkSmartPointer<vtkPoints>::New();
        points->InsertNextPoint(point.GetData());

        return GetPointsPolyData(points);
    }

    vtkSmartPointer<vtkPolyData> GetPointsPolyData(vtkPoints* points)
    {
        auto pointsPolyData = vtkSmartPointer<vtkPolyData>::New();
        pointsPolyData->SetPoints(points);

        auto vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
        vertexFilter->SetInputData(pointsPolyData);
        vertexFilter->Update();

        return vertexFilter->GetOutput();
    }

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

    vtkSmartPointer<vtkPolyData> GetPlanePolyData(double center[3], double normal[3], int xResolution, int yResolution, int width, int height)
    {
        auto planeSource = vtkSmartPointer<vtkPlaneSource>::New();
        planeSource->SetCenter(center);
        planeSource->SetNormal(normal);
        planeSource->SetXResolution(xResolution);
        planeSource->SetYResolution(yResolution);
        planeSource->Update();

        double axisX[3];
        double axisY[3];
        planeSource->GetAxis1(axisX);
        planeSource->GetAxis2(axisY);

        //move center to bottom left
        for(int i = 0; i < 3; ++i)
        {
            center[i] -= width * 0.5 * axisX[i];
            center[i] -= height * 0.5 * axisY[i];
        }

        //change size of plane
        double point1[3];
        double point2[3];
        for(int i = 0; i < 3; ++i)
        {
            point1[i] = center[i] + width * axisX[i];
            point2[i] = center[i] + height * axisY[i];
        }

        planeSource->SetOrigin(center);
        planeSource->SetPoint1(point1);
        planeSource->SetPoint2(point2);
        planeSource->Update();

        return planeSource->GetOutput();
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

    void GetPlaneAxes(double center[3], double normal[3], double axisX[3], double axisY[3])
    {
        auto planeSource = vtkSmartPointer<vtkPlaneSource>::New();
        planeSource->SetCenter(center);
        planeSource->SetNormal(normal);
        planeSource->Update();

        planeSource->GetAxis1(axisX);
        planeSource->GetAxis2(axisY);
    }
}
