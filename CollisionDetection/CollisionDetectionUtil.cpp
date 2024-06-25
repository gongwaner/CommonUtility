#include "CollisionDetectionUtil.h"

#include <vtkVectorOperators.h>


namespace CollisionDetectionUtil
{
    std::optional<vtkVector3d> GetRayPlaneIntersectionPoint(const vtkVector3d& rayOrigin, const vtkVector3d& rayDir, const vtkVector3d& planeOrigin, const vtkVector3d& planeNormal)
    {
        //calculate the denominator
        const auto denominator = rayDir.Dot(planeNormal);

        //ray is parallel to plane. avoid divide by zero
        if(abs(denominator) <= 1e-6)
        {
            return std::nullopt;
        }

        const auto d = -planeOrigin.Dot(planeNormal);//plane equation d
        const auto t = -(planeNormal.Dot(rayOrigin) + d) / denominator;

        return rayOrigin + t * rayDir;
    }
}