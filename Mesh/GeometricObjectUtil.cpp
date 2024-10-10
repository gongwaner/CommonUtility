#include "GeometricObjectUtil.h"

#include <vtkVectorOperators.h>
#include <vtkMatrix4x4.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkLineSource.h>
#include <vtkLine.h>
#include <vtkCardinalSpline.h>
#include <vtkParametricSpline.h>
#include <vtkParametricFunctionSource.h>
#include <vtkSphere.h>
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>
#include <vtkPlaneSource.h>

#include "../Transformation/TransformUtil.h"


namespace GeometricObjectUtil
{
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

    vtkSmartPointer<vtkPolyData> GetLineMesh(const vtkVector3d& lineStart, const vtkVector3d& lineEnd, const double radius)
    {
        const auto center = (lineStart + lineEnd) * 0.5;
        const auto lineLength = (lineEnd - lineStart).Norm();

        auto line = GetCylinderPolyData(center, radius, lineLength, 10);//random resolution

        //the returned cylinder mesh is aligned with global Y. rotate it to align with the line
        auto rotationMat = TransformUtil::GetAlignVectorMatrix({0, 1, 0}, lineEnd - lineStart);
        auto transformMat = TransformUtil::GetTransformationMatrix(center, rotationMat);
        TransformUtil::TransformMesh(line, transformMat);

        return line;
    }

    vtkSmartPointer<vtkPolyData> GetDashedLinePolyData(const std::vector<vtkVector3d>& points, const double dashLength, const double gapLength)
    {
        auto dashedLinePoints = vtkSmartPointer<vtkPoints>::New();
        auto lines = vtkSmartPointer<vtkCellArray>::New();
        double totalLength = 0.0;

        for(size_t i = 1; i < points.size(); ++i)
        {
            const auto start = points[i - 1];
            const auto end = points[i];
            const auto direction = (end - start).Normalized();
            const double segmentLength = (end - start).Norm();

            while(totalLength < segmentLength)
            {
                const auto dashStart = start + direction * totalLength;
                totalLength += dashLength;
                if(totalLength > segmentLength)
                    break;

                const auto dashEnd = start + direction * totalLength;
                const auto idStart = dashedLinePoints->InsertNextPoint(dashStart.GetData());
                const auto idEnd = dashedLinePoints->InsertNextPoint(dashEnd.GetData());

                auto line = vtkSmartPointer<vtkLine>::New();
                line->GetPointIds()->SetId(0, idStart);
                line->GetPointIds()->SetId(1, idEnd);
                lines->InsertNextCell(line);

                totalLength += gapLength;
            }

            totalLength -= segmentLength;
        }

        auto polyData = vtkSmartPointer<vtkPolyData>::New();
        polyData->SetPoints(dashedLinePoints);
        polyData->SetLines(lines);
        return polyData;
    }

    vtkSmartPointer<vtkPolyData> GetCurvePolyData(vtkPoints* points)
    {
        auto xSpline = vtkSmartPointer<vtkCardinalSpline>::New();
        auto ySpline = vtkSmartPointer<vtkCardinalSpline>::New();
        auto zSpline = vtkSmartPointer<vtkCardinalSpline>::New();

        auto spline = vtkSmartPointer<vtkParametricSpline>::New();
        spline->SetXSpline(xSpline);
        spline->SetYSpline(ySpline);
        spline->SetZSpline(zSpline);
        spline->SetPoints(points);

        auto functionSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
        functionSource->SetParametricFunction(spline);
        functionSource->Update();

        return functionSource->GetOutput();
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

    vtkSmartPointer<vtkSphere> GetSphereImplicitFunction(const vtkVector3d& sphereCenter, const double radius)
    {
        auto implicitFunction = vtkSmartPointer<vtkSphere>::New();
        implicitFunction->SetCenter(sphereCenter.GetData());
        implicitFunction->SetRadius(radius);

        return implicitFunction;
    }

    vtkSmartPointer<vtkPolyData> GetCubePolyData(const vtkVector3d& pos, const double size)
    {
        auto cube = vtkSmartPointer<vtkCubeSource>::New();
        cube->SetCenter(pos.GetData());
        cube->SetXLength(size);
        cube->SetYLength(size);
        cube->SetZLength(size);
        cube->Update();

        return cube->GetOutput();
    }

    vtkSmartPointer<vtkPolyData> GetCylinderPolyData(const vtkVector3d& center, const double radius, const double height, const int resolution)
    {
        auto cylinderSource = vtkSmartPointer<vtkCylinderSource>::New();
        cylinderSource->SetCenter(center.GetData());
        cylinderSource->SetRadius(radius);
        cylinderSource->SetHeight(height);
        cylinderSource->SetResolution(resolution);

        cylinderSource->Update();

        return cylinderSource->GetOutput();
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
