namespace CollisionDetectionUtil
{
    /**
     *  "Fast and Robust Triangle-Triangle Overlap Test using Orientation Predicates"  by Philippe Guigue and Olivier Devillers
     *  code adapted from https://github.com/erich666/jgt-code/tree/master/Volume_08/Number_1/Guigue2003
     *  This function computes the segment of intersection of the two triangles if it exists.
     *  coplanar returns whether the triangles are coplanar.
     *  source and target are the endpoints of the line segment of intersection.
     *  note: the original code handles coplanar calculation. here the information is NOT calculated.
     */
    int TriangleTriangleIntersection(const double p1[3], const double q1[3], const double r1[3], const double p2[3], const double q2[3], const double r2[3],
                                     int& coplanar, double source[3], double target[3], double epsilon);
}
