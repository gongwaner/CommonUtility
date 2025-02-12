#include "TransformUtil.h"

#include <vtkMatrix3x3.h>
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <vtkMath.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyData.h>
#include <vtkImageData.h>
#include <vtkImageReslice.h>


namespace TransformUtil
{
    //ref: https://github.com/hinerm/ITK/blob/21f48c6d98e21ecece09be16a747221d7094d8a9/Modules/Core/Transform/include/itkEuler3DTransform.hxx#L202C1-L202C1
    vtkSmartPointer<vtkMatrix3x3> GetRotationMatrix(const double rotationAngleX, const double rotationAngleY, const double rotationAngleZ)
    {
        auto rotationX = vtkSmartPointer<vtkMatrix3x3>::New();
        {
            const double cosX = std::cos(rotationAngleX);
            const double sinX = std::sin(rotationAngleX);

            rotationX->SetElement(0, 0, 1);
            rotationX->SetElement(0, 1, 0);
            rotationX->SetElement(0, 2, 0);
            rotationX->SetElement(1, 0, 0);
            rotationX->SetElement(1, 1, cosX);
            rotationX->SetElement(1, 2, -sinX);
            rotationX->SetElement(2, 0, 0);
            rotationX->SetElement(2, 1, sinX);
            rotationX->SetElement(2, 2, cosX);
        }

        auto rotationY = vtkSmartPointer<vtkMatrix3x3>::New();
        {
            const double cosY = std::cos(rotationAngleY);
            const double sinY = std::sin(rotationAngleY);

            rotationY->SetElement(0, 0, cosY);
            rotationY->SetElement(0, 1, 0);
            rotationY->SetElement(0, 2, sinY);
            rotationY->SetElement(1, 0, 0);
            rotationY->SetElement(1, 1, 1);
            rotationY->SetElement(1, 2, 0);
            rotationY->SetElement(2, 0, -sinY);
            rotationY->SetElement(2, 1, 0);
            rotationY->SetElement(2, 2, cosY);
        }

        auto rotationZ = vtkSmartPointer<vtkMatrix3x3>::New();
        {
            const double cosZ = std::cos(rotationAngleZ);
            const double sinZ = std::sin(rotationAngleZ);

            rotationZ->SetElement(0, 0, cosZ);
            rotationZ->SetElement(0, 1, -sinZ);
            rotationZ->SetElement(0, 2, 0);
            rotationZ->SetElement(1, 0, sinZ);
            rotationZ->SetElement(1, 1, cosZ);
            rotationZ->SetElement(1, 2, 0);
            rotationZ->SetElement(2, 0, 0);
            rotationZ->SetElement(2, 1, 0);
            rotationZ->SetElement(2, 2, 1);
        }

        //compute Z*Y*X
        auto zyx = vtkSmartPointer<vtkMatrix3x3>::New();
        vtkMatrix3x3::Multiply3x3(rotationZ, rotationY, zyx);
        vtkMatrix3x3::Multiply3x3(zyx, rotationX, zyx);
        zyx->Invert();//by default VTK uses pre-multiply but we need post multiply

        return zyx;
    }

    //ref: https://github.com/InsightSoftwareConsortium/ITK/blob/bf54f34df211145c38100ed262ee7a9740b51ba5/Modules/Core/Common/include/itkImageBase.h#L353
    vtkVector3d GetOffset(const vtkVector3d& center, const vtkVector3d& translation, const vtkMatrix3x3* rotationMatrix)
    {
        vtkVector3d offset;
        const int dimension = 3;
        for(int i = 0; i < dimension; ++i)
        {
            offset[i] = translation[i] + center[i];

            for(int j = 0; j < dimension; ++j)
            {
                offset[i] -= rotationMatrix->GetElement(i, j) * center[j];
            }
        }

        return offset;
    }

    vtkSmartPointer<vtkMatrix4x4> GetTransformationMatrix(const vtkVector3d& center, const vtkVector3d& translation, const vtkVector3d& rotation)
    {
        auto rotationMatrix = GetRotationMatrix(rotation[0], rotation[1], rotation[2]);
        auto offset = GetOffset(center, translation, rotationMatrix);

        auto transformationMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
        const int dimension = 3;
        //set rotation
        for(int i = 0; i < dimension; ++i)
        {
            for(int j = 0; j < dimension; ++j)
            {
                transformationMatrix->SetElement(i, j, rotationMatrix->GetElement(i, j));
            }
        }

        //set offset
        for(int i = 0; i < dimension; ++i)
        {
            transformationMatrix->SetElement(i, 3, offset[i]);
        }

        return transformationMatrix;
    }

    vtkSmartPointer<vtkMatrix4x4> GetTransformationMatrix(const vtkVector3d& rotCenter, vtkMatrix4x4* rotMat)
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

        return mat1;
    }

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
        auto transformMat = GetTransformationMatrix(rotCenter, rotMat);

        double inputPoint[4]{point[0], point[1], point[2], 1};
        transformMat->MultiplyPoint(inputPoint, inputPoint);

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

    void TranslateMesh(vtkSmartPointer<vtkPolyData>& mesh, const vtkVector3d& translation)
    {
        auto transform = vtkSmartPointer<vtkTransform>::New();
        transform->Translate(translation.GetData());

        mesh = GetTransformedMesh(mesh, transform);
    }

    void TransformMesh(vtkSmartPointer<vtkPolyData>& mesh, vtkMatrix4x4* transformMat)
    {
        auto transform = vtkSmartPointer<vtkTransform>::New();
        transform->SetMatrix(transformMat);
        transform->Update();

        mesh = GetTransformedMesh(mesh, transform);
    }

    vtkSmartPointer<vtkPolyData> GetTranslatedMesh(vtkSmartPointer<vtkPolyData> mesh, const vtkVector3d& translation)
    {
        auto transform = vtkSmartPointer<vtkTransform>::New();
        transform->Translate(translation.GetData());

        return GetTransformedMesh(mesh, transform);
    }

    vtkSmartPointer<vtkPolyData> GetTransformedMesh(vtkSmartPointer<vtkPolyData> mesh, vtkTransform* transform)
    {
        auto polyFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        polyFilter->SetInputData(mesh);
        polyFilter->SetTransform(transform);
        polyFilter->Update();

        return polyFilter->GetOutput();
    }

    vtkSmartPointer<vtkPolyData> GetTransformedMesh(vtkSmartPointer<vtkPolyData> mesh, vtkMatrix4x4* transformMat)
    {
        auto transform = vtkSmartPointer<vtkTransform>::New();
        transform->SetMatrix(transformMat);
        transform->Update();

        return GetTransformedMesh(mesh, transform);
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
