#pragma  once

#include <vtkVector.h>
#include <optional>

namespace CollisionDetectionUtil
{
    std::optional<vtkVector3d> GetRayPlaneIntersectionPoint(const vtkVector3d& rayOrigin, const vtkVector3d& rayDir, const vtkVector3d& planeOrigin, const vtkVector3d& planeNormal);

    /**
     * Test intersection between ray and AABB(axis aligned bounding box).
     * the bounding box is represented by bounds[6]{xmin, xmax, ymin, ymax, zmin, zmax}
     * note this function only checks if there's intersection or not, does not contain any intersection points info
     */
    bool RayIntersectsAABB(const vtkVector3d& rayStart, const vtkVector3d& rayEnd, const double bounds[6]);
}


