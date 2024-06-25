#include "TransformUtil.h"

#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <vtkMath.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyData.h>
#include <vtkImageData.h>
#include <vtkImageReslice.h>


namespace TransformUtil
{
    vtkVector3d GetTransformedPoint(const double point[3], vtkMatrix4x4* transformMat)
    {
        double in[4]{point[0], point[1], point[2], 1};
        double out[4];
        transformMat->MultiplyPoint(in, out);

        return {out[0], out[1], out[2]};
    }

    vtkVector3d GetTransformedPoint(const vtkVector3d& point, vtkMatrix4x4* transformMat)
    {
        return GetTransformedPoint(point.GetData(), transformMat);
    }


    vtkVector3d GetTransformedPoint(const vtkVector3d& point, vtkMatrix4x4* rotMat, const vtkVector3d& rotCenter)
    {
        auto mat1 = vtkSmartPointer<vtkMatrix4x4>::New();
        auto mat2 = vtkSmartPointer<vtkMatrix4x4>::New();
        for(int i = 0; i < 3; ++i)
        {
            mat1->SetElement(i, 3, rotCenter[i]);
            mat2->SetElement(i, 3, -rotCenter[i]);
        }

        vtkMatrix4x4::Multiply4x4(mat1, rotMat, mat1);
        vtkMatrix4x4::Multiply4x4(mat1, mat2, mat1);

        double inputPoint[4]{point[0], point[1], point[2], 1};
        mat1->MultiplyPoint(inputPoint, inputPoint);

        return vtkVector3d(inputPoint);
    }

    vtkVector3d GetTransformedVector(const double vec[3], vtkMatrix4x4* transformMat)
    {
        double in[4]{vec[0], vec[1], vec[2], 0};
        double out[4];
        transformMat->MultiplyPoint(in, out);

        return {out[0], out[1], out[2]};
    }

    vtkVector3d GetTransformedVector(const vtkVector3d& vec, vtkMatrix4x4* transformMat)
    {
        return GetTransformedVector(vec.GetData(), transformMat);
    }

    vtkSmartPointer<vtkMatrix4x4> GetAlignVectorMatrix(const vtkVector3d& from, const vtkVector3d& to)
    {
        const auto toLength = to.Norm();
        auto angle = acos(from.Dot(to) / toLength * toLength);
        angle = vtkMath::DegreesFromRadians(angle);

        const auto axis = from.Cross(to);
        auto transform = vtkSmartPointer<vtkTransform>::New();
        transform->RotateWXYZ(angle, axis[0], axis[1], axis[2]);
        transform->Update();

        return transform->GetMatrix();
    }

    vtkSmartPointer<vtkPolyData> GetTransformedMesh(vtkSmartPointer<vtkPolyData> mesh, vtkMatrix4x4* transformMat)
    {
        auto transform = vtkSmartPointer<vtkTransform>::New();
        transform->SetMatrix(transformMat);
        transform->Update();

        auto polyFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        polyFilter->SetInputData(mesh);
        polyFilter->SetTransform(transform);
        polyFilter->Update();

        return polyFilter->GetOutput();
    }

    vtkSmartPointer<vtkImageData> GetTransformedImageData(vtkSmartPointer<vtkImageData> imageData, vtkMatrix4x4* transformMat, bool cubic)
    {
        auto reslice = vtkSmartPointer<vtkImageReslice>::New();
        reslice->SetInputData(imageData);
        reslice->SetOutputOrigin(imageData->GetOrigin());
        reslice->SetOutputSpacing(imageData->GetSpacing());
        reslice->GetOutputExtent(imageData->GetExtent());
        reslice->SetResliceAxes(transformMat);

        if(cubic)
            reslice->SetInterpolationModeToCubic();//default: nearest neighbor

        reslice->Update();

        return reslice->GetOutput();
    }
}