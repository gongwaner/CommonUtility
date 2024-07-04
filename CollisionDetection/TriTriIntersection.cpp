#include "TriTriIntersection.h"

#include <cstdlib>


namespace CollisionDetectionUtil
{
    double Dot(const double v1[3], const double v2[3])
    {
        return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
    }

    void Cross(const double v1[3], const double v2[3], double dest[3])
    {
        dest[0] = v1[1] * v2[2] - v1[2] * v2[1];
        dest[1] = v1[2] * v2[0] - v1[0] * v2[2];
        dest[2] = v1[0] * v2[1] - v1[1] * v2[0];
    }

    void Sub(const double v1[3], const double v2[3], double dest[3])
    {
        dest[0] = v1[0] - v2[0];
        dest[1] = v1[1] - v2[1];
        dest[2] = v1[2] - v2[2];
    }

    void Scale(const double v[3], const double scale, double dest[3])
    {
        dest[0] = scale * v[0];
        dest[1] = scale * v[1];
        dest[2] = scale * v[2];
    }

    int CheckMinMax(const double p1[3], const double q1[3], const double r1[3], const double p2[3], const double q2[3], const double r2[3])
    {
        double v1[3];
        double v2[3];
        double n1[3];

        Sub(p2, q1, v1);
        Sub(p1, q1, v2);
        Cross(v1, v2, n1);
        Sub(q2, q1, v1);
        if(Dot(v1, n1) > 0)
            return 0;

        Sub(p2, p1, v1);
        Sub(r1, p1, v2);
        Cross(v1, v2, n1);
        Sub(r2, p1, v1);
        if(Dot(v1, n1) > 0)
            return 0;

        return 1;
    }

    /**
     * called when the triangles surely intersect.
     * It constructs the segment of intersection of the two triangles if they are not coplanar.
     */
    int ConstructIntersection(const double p1[3], const double q1[3], const double r1[3], const double p2[3], const double q2[3], const double r2[3],
                              const double n1[3], const double n2[3], const double epsilon, double source[3], double target[3])
    {
        double v1[3];
        double v2[3];
        double n[3];
        double v[3];
        double scalar;

        Sub(q1, p1, v1);
        Sub(r2, p1, v2);
        Cross(v1, v2, n);
        Sub(p2, p1, v);

        auto dotVN = Dot(v, n);
        if(dotVN < 0 || abs(dotVN) < epsilon)
        {
            Sub(q2, p1, v2);
            Cross(v1, v2, n);

            auto dotVN = Dot(v, n);
            if(dotVN < 0 && abs(dotVN) > epsilon)
            {
                return 0;
            }
            else
            {
                Sub(r1, p1, v1);
                Cross(v1, v2, n);

                auto dotVN = Dot(v, n);
                if(dotVN > 0 || abs(dotVN) < epsilon)
                {
                    Sub(p1, p2, v1);
                    Sub(p1, r1, v2);
                    scalar = Dot(v1, n2) / Dot(v2, n2);
                    Scale(v2, scalar, v1);
                    Sub(p1, v1, source);

                    Sub(p1, p2, v1);
                    Sub(p1, q1, v2);
                    scalar = Dot(v1, n2) / Dot(v2, n2);
                    Scale(v2, scalar, v1);
                    Sub(p1, v1, target);

                    return 1;
                }
                else
                {
                    Sub(p2, p1, v1);
                    Sub(p2, q2, v2);
                    scalar = Dot(v1, n1) / Dot(v2, n1);
                    Scale(v2, scalar, v1);
                    Sub(p2, v1, source);

                    Sub(p1, p2, v1);
                    Sub(p1, q1, v2);
                    scalar = Dot(v1, n2) / Dot(v2, n2);
                    Scale(v2, scalar, v1);
                    Sub(p1, v1, target);

                    return 1;
                }
            }
        }
        else //Dot(v, n) > 0
        {
            Sub(r1, p1, v1);
            Cross(v1, v2, n);

            auto dotVN = Dot(v, n);
            if(dotVN < 0 || abs(dotVN) < epsilon)
            {
                Sub(q2, p1, v2);
                Cross(v1, v2, n);

                auto dotVN = Dot(v, n);
                if(dotVN > 0 && abs(dotVN) > epsilon)
                {
                    Sub(p1, p2, v1);
                    Sub(p1, r1, v2);
                    scalar = Dot(v1, n2) / Dot(v2, n2);
                    Scale(v2, scalar, v1);
                    Sub(p1, v1, source);

                    Sub(p2, p1, v1);
                    Sub(p2, r2, v2);
                    scalar = Dot(v1, n1) / Dot(v2, n1);
                    Scale(v2, scalar, v1);
                    Sub(p2, v1, target);

                    return 1;
                }
                else
                {
                    Sub(p2, p1, v1);
                    Sub(p2, q2, v2);
                    scalar = Dot(v1, n1) / Dot(v2, n1);
                    Scale(v2, scalar, v1);
                    Sub(p2, v1, source);

                    Sub(p2, p1, v1);
                    Sub(p2, r2, v2);
                    scalar = Dot(v1, n1) / Dot(v2, n1);
                    Scale(v2, scalar, v1);
                    Sub(p2, v1, target);

                    return 1;
                }
            }

            return 0;
        }
    }

    int TriTriInter3D(const double p1[3], const double q1[3], const double r1[3], const double p2[3], const double q2[3], const double r2[3], const double dp2,
                      const double dq2, const double dr2, const double n1[3], const double n2[3], const double epsilon, double source[3], double target[3])
    {
        if(abs(dp2) < epsilon)
        {
            if(abs(dq2) < epsilon)
            {
                if(abs(dr2) < epsilon)
                {
                    //treat coplanar as no intersection
                    return 0;
                }
                else if(dr2 > 0)
                    return ConstructIntersection(p1, q1, r1, r2, p2, q2, n1, n2, epsilon, source, target);
                else //dr2 < 0
                    return ConstructIntersection(p1, r1, q1, r2, p2, q2, n1, n2, epsilon, source, target);
            }
            else if(dq2 < 0)
            {
                if(dr2 > 0 || abs(dr2) < epsilon)
                    return ConstructIntersection(p1, r1, q1, q2, r2, p2, n1, n2, epsilon, source, target);
                else
                    return ConstructIntersection(p1, q1, r1, p2, q2, r2, n1, n2, epsilon, source, target);
            }
            else //dq2 > 0
            {
                if(dr2 > 0 && abs(dr2) > epsilon)
                    return ConstructIntersection(p1, r1, q1, p2, q2, r2, n1, n2, epsilon, source, target);
                else
                    return ConstructIntersection(p1, q1, r1, q2, r2, p2, n1, n2, epsilon, source, target);
            }
        }
        else if(dp2 > 0)
        {
            if(dq2 > 0 && abs(dq2) > epsilon)
                return ConstructIntersection(p1, r1, q1, r2, p2, q2, n1, n2, epsilon, source, target);
            else if(dr2 > 0 && abs(dr2) > epsilon)
                return ConstructIntersection(p1, r1, q1, q2, r2, p2, n1, n2, epsilon, source, target);
            else
                return ConstructIntersection(p1, q1, r1, p2, q2, r2, n1, n2, epsilon, source, target);
        }
        else//dp2 < 0
        {
            if(dq2 < 0 && abs(dq2) > epsilon)
                return ConstructIntersection(p1, q1, r1, r2, p2, q2, n1, n2, epsilon, source, target);
            else if(dr2 < 0 && abs(dr2) > epsilon)
                return ConstructIntersection(p1, q1, r1, q2, r2, p2, n1, n2, epsilon, source, target);
            else
                return ConstructIntersection(p1, r1, q1, p2, q2, r2, n1, n2, epsilon, source, target);
        }
    }

    int TriangleTriangleIntersection(const double p1[3], const double q1[3], const double r1[3], const double p2[3], const double q2[3], const double r2[3],
                                     int& coplanar, double source[3], double target[3], const double epsilon)
    {
        coplanar = 0;

        double dp1, dq1, dr1, dp2, dq2, dr2;
        double v1[3], v2[3];
        double n1[3], n2[3];

        // Compute distance signs  of p1, q1 and r1 to the plane of triangle(p2,q2,r2)
        Sub(p2, r2, v1);
        Sub(q2, r2, v2);
        Cross(v1, v2, n2);

        Sub(p1, r2, v1);
        dp1 = Dot(v1, n2);
        Sub(q1, r2, v1);
        dq1 = Dot(v1, n2);
        Sub(r1, r2, v1);
        dr1 = Dot(v1, n2);

        if(dp1 * dq1 > 0 && dp1 * dr1 > 0)
            return 0;

        // Compute distance signs  of p2, q2 and r2 to the plane of triangle(p1,q1,r1)
        Sub(q1, p1, v1);
        Sub(r1, p1, v2);
        Cross(v1, v2, n1);

        Sub(p2, r1, v1);
        dp2 = Dot(v1, n1);
        Sub(q2, r1, v1);
        dq2 = Dot(v1, n1);
        Sub(r2, r1, v1);
        dr2 = Dot(v1, n1);

        if(dp2 * dq2 > 0 && dp2 * dr2 > 0)
            return 0;

        // Permutation in a canonical form of T1's vertices
        if(abs(dp1) < epsilon)
        {
            if(abs(dq1) < epsilon)
            {
                if(abs(dr1) < epsilon)
                {
                    // triangles are co-planar
                    coplanar = 1;
                    return 0;
                }
                else if(dr1 > 0)
                    return TriTriInter3D(r1, p1, q1, p2, q2, r2, dp2, dq2, dr2, n1, n2, epsilon, source, target);
                else //dr1 < 0
                    return TriTriInter3D(r1, p1, q1, p2, r2, q2, dp2, dr2, dq2, n1, n2, epsilon, source, target);
            }
            else if(dq1 < 0)
            {
                if(dr1 > 0 || abs(dr1) < epsilon)
                    return TriTriInter3D(q1, r1, p1, p2, r2, q2, dp2, dr2, dq2, n1, n2, epsilon, source, target);
                else
                    return TriTriInter3D(p1, q1, r1, p2, q2, r2, dp2, dq2, dr2, n1, n2, epsilon, source, target);
            }
            else //dq1 > 0
            {
                if(dr1 > 0 && abs(dr1) > epsilon)
                    return TriTriInter3D(p1, q1, r1, p2, r2, q2, dp2, dr2, dq2, n1, n2, epsilon, source, target);
                else
                    return TriTriInter3D(q1, r1, p1, p2, q2, r2, dp2, dq2, dr2, n1, n2, epsilon, source, target);
            }
        }
        else if(dp1 > 0)
        {
            if(dq1 > 0 && abs(dq1) > epsilon)
                return TriTriInter3D(r1, p1, q1, p2, r2, q2, dp2, dr2, dq2, n1, n2, epsilon, source, target);
            else if(dr1 > 0 && abs(dr1) > epsilon)
                return TriTriInter3D(q1, r1, p1, p2, r2, q2, dp2, dr2, dq2, n1, n2, epsilon, source, target);
            else
                return TriTriInter3D(p1, q1, r1, p2, q2, r2, dp2, dq2, dr2, n1, n2, epsilon, source, target);
        }
        else //dp1 < 0
        {
            if(dq1 < 0 && abs(dq1) > epsilon)
                return TriTriInter3D(r1, p1, q1, p2, q2, r2, dp2, dq2, dr2, n1, n2, epsilon, source, target);
            else if(dr1 < 0 && abs(dr1) > epsilon)
                return TriTriInter3D(q1, r1, p1, p2, q2, r2, dp2, dq2, dr2, n1, n2, epsilon, source, target);
            else
                return TriTriInter3D(p1, q1, r1, p2, r2, q2, dp2, dr2, dq2, n1, n2, epsilon, source, target);
        }
    }
}
