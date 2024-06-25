#pragma  once

#include <vtkVector.h>
#include <optional>

namespace CollisionDetectionUtil
{
    std::optional<vtkVector3d> GetRayPlaneIntersectionPoint(const vtkVector3d& rayOrigin, const vtkVector3d& rayDir, const vtkVector3d& planeOrigin, const vtkVector3d& planeNormal);
}


