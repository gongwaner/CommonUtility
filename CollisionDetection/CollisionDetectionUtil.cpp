#include "CollisionDetectionUtil.h"

#include <vtkVectorOperators.h>


namespace CollisionDetectionUtil
{
    std::optional<vtkVector3d> GetRayPlaneIntersectionPoint(const vtkVector3d& rayOrigin, const vtkVector3d& rayDir, const vtkVector3d& planeOrigin,
                                                            const vtkVector3d& planeNormal)
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

    std::optional<vtkVector3d> GetRaySphereIntersectionPoint(const vtkVector3d& rayOrigin, const vtkVector3d& rayDir, const vtkVector3d& sphereCenter, const double radius)
    {
        const auto co = rayOrigin - sphereCenter;
        const auto a = rayDir.SquaredNorm();
        const auto b = rayDir.Dot(co);
        const auto c = co.SquaredNorm() - radius * radius;
        const auto delta = b * b - a * c;

        if(delta < 0)
            return std::nullopt;

        //1 or 2 solutions, take the closest (positive) intersection
        const double sqrtDelta = sqrt(delta);
        const double tMin = (-b - sqrtDelta) / a;
        const double tMax = (-b + sqrtDelta) / a;

        return rayOrigin + tMin * rayDir;
    }

    bool RayIntersectsAABB(const vtkVector3d& rayStart, const vtkVector3d& rayEnd, const double bounds[6])
    {
        const auto dir = rayEnd - rayStart;
        const auto invDir = vtkVector3d(1.0 / dir[0], 1.0 / dir[1], 1.0 / dir[2]);

        double t1 = (bounds[0] - rayStart[0]) * invDir[0];//xmin
        double t2 = (bounds[1] - rayStart[0]) * invDir[0];//xmax
        double t3 = (bounds[2] - rayStart[1]) * invDir[1];//ymin
        double t4 = (bounds[3] - rayStart[1]) * invDir[1];//ymax
        double t5 = (bounds[4] - rayStart[2]) * invDir[2];//zmin
        double t6 = (bounds[5] - rayStart[2]) * invDir[2];//zmax

        double tMin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
        double tMax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

        if(tMax < 0)//ray is intersecting AABB, but the whole AABB is behind us
            return false;

        if(tMin > tMax)//ray doesn't intersect AABB
            return false;

        return true;
    }
}
