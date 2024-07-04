#pragma once

#include <vtkSmartPointer.h>
#include <vtkVector.h>


class vtkMatrix3x3;
class vtkMatrix4x4;
class vtkPolyData;
class vtkImageData;

namespace TransformUtil
{
    vtkSmartPointer<vtkMatrix3x3> GetRotationMatrix(double rotationAngleX, double rotationAngleY, double rotationAngleZ);

    vtkSmartPointer<vtkMatrix4x4> GetTransformationMatrix(const vtkVector3d& center, const vtkVector3d& translation, const vtkVector3d& rotation);

    vtkVector3d GetTransformedPoint(const double point[3], vtkMatrix4x4* transformMat);

    vtkVector3d GetTransformedPoint(const vtkVector3d& point, vtkMatrix4x4* transformMat);

    /**
     * given the rotation matrix and rotation center of a point, calculate its transformed position
     */
    vtkVector3d GetTransformedPoint(const vtkVector3d& point, vtkMatrix4x4* rotMat, const vtkVector3d& rotCenter);

    vtkVector3d GetTransformedVector(const double vec[3], vtkMatrix4x4* transformMat);

    vtkVector3d GetTransformedVector(const vtkVector3d& vec, vtkMatrix4x4* transformMat);

    /**
     * given an input vector "from", align it to another vector "to",
     * return the transformation matrix needed to perform the alignment
     */
    vtkSmartPointer<vtkMatrix4x4> GetAlignVectorMatrix(const vtkVector3d& from, const vtkVector3d& to);

    vtkSmartPointer<vtkPolyData> GetTransformedMesh(vtkSmartPointer<vtkPolyData> mesh, vtkMatrix4x4* transformMat);

    vtkSmartPointer<vtkImageData> GetTransformedImageData(vtkSmartPointer<vtkImageData> imageData, vtkMatrix4x4* transformMat, bool cubic = false);
}
