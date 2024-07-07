#include "OptimalPolygonTriangulation.h"

#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkTriangle.h>
#include <vtkVectorOperators.h>
#include <vtkPolygon.h>

#include "../Mesh/MeshUtil.h"

#include "PolygonUtil.h"


namespace Algorithm::Triangulation
{
    void OptimalPolygonTriangulation::SetPolygonPoints(const std::vector<vtkVector3d>& polygonPoints)
    {
        mPolygonPoints = polygonPoints;

        //check if it's convex
        auto points = vtkSmartPointer<vtkPoints>::New();
        for(const auto& point: mPolygonPoints)
            points->InsertNextPoint(point.GetData());

        mIsConvex = vtkPolygon::IsConvex(points);
    }

    void OptimalPolygonTriangulation::SetNormal(const vtkVector3d& normal)
    {
        mNormal = normal;
    }

    void OptimalPolygonTriangulation::SetWeightFunction(const std::function<double(const vtkVector3d&, const vtkVector3d&, const vtkVector3d&)>& weightFunc)
    {
        mWeightFunc = weightFunc;
    }

    void OptimalPolygonTriangulation::AddNewTriangle(int vid0, int vid1, int vid2)
    {
        const auto triNormal = PolygonUtil::GetTriangleNormal(mPolygonPoints[vid0], mPolygonPoints[vid1], mPolygonPoints[vid2]);

        vtkSmartPointer<vtkTriangle> triangle;
        if(triNormal.Dot(mNormal) < 0)
            triangle = PolygonUtil::GetTriangle(vid0, vid1, vid2);
        else
            triangle = PolygonUtil::GetTriangle(vid2, vid1, vid0);

        mNewTriangles->InsertNextCell(triangle);
    }

    void OptimalPolygonTriangulation::AddNewTriangles(const std::vector<std::vector<Weights>>& table, const int i, const int j)
    {
        if(j <= i + 1)
            return;

        if(j == i + 2)
        {
            AddNewTriangle(i, i + 1, i + 2);
            return;
        }

        //std::cout << std::format("i={}, k={}, j={}", i, table[i][j].k, j) << std::endl;
        AddNewTriangle(i, table[i][j].k, j);

        AddNewTriangles(table, i, table[i][j].k);
        AddNewTriangles(table, table[i][j].k, j);
    }

    void OptimalPolygonTriangulation::ConvexTriangulation()
    {
        const auto size = mPolygonPoints.size();

        // table to store results of subproblems.  table[i][j] stores cost of triangulation of points from i to j.
        // The entry table[0][n-1] stores the final result.
        std::vector<std::vector<Weights>> table(size);
        for(int i = 0; i < size; ++i)
        {
            std::vector<Weights> entry(size);
            table[i] = entry;
        }

        for(int gap = 0; gap < size; gap++)
        {
            for(int i = 0, j = gap; j < size; i++, j++)
            {
                if(j < i + 2)
                    table[i][j].weight = 0;
                else
                {
                    table[i][j].weight = DBL_MAX;
                    for(int k = i + 1; k < j; k++)
                    {
                        const double weight = table[i][k].weight + table[k][j].weight + mWeightFunc(mPolygonPoints[i], mPolygonPoints[j], mPolygonPoints[k]);

                        if(table[i][j].weight > weight)
                        {
                            table[i][j].weight = weight;
                            table[i][j].k = k;
                        }
                    }
                }
            }
        }

        AddNewTriangles(table, 0, size - 1);
    }

    /**
     * check is 2 indices form a valid diagonal in polygon
     * for convex polygon the diagonal is valid as long as i,j has interval >= 2
     * for concave polygon the diagonal should be inside polygon
     */
    bool OptimalPolygonTriangulation::IsValidDiagonal(int i, int j, const double epsilon) const
    {
        if(abs(i - j) < 2)
            return false;

        if(i > j)
            std::swap(i, j);

        const int n = mPolygonPoints.size();

        const int begin1 = i + 1;
        const int end1 = j - 1;
        const int begin2 = (j + 1) % n;
        const int end2 = (i > 0) ? i - 1 : n - 1;

        //check intersection between (i,j) and all polygon edges
        //TODO: corner case: if (i,j) and (k,k+1) are coplanar and intersect one of (i,j) is in middle of line, this is a valid diagonal
        for(int k = begin1; k < end1; ++k)
        {
            if(PolygonUtil::LinesIntersect(mPolygonPoints[i], mPolygonPoints[j], mPolygonPoints[k], mPolygonPoints[k + 1]))
                return false;
        }
        for(int k = begin2; k != end2; k = (k + 1) % n)
        {
            if(PolygonUtil::LinesIntersect(mPolygonPoints[i], mPolygonPoints[j], mPolygonPoints[k], mPolygonPoints[(k + 1) % n]))
                return false;
        }

        //no intersections, check if line segment ij is completely inside or completely outside polygon
        const vtkVector3d v1 = mPolygonPoints[i + 1] - mPolygonPoints[i];
        const vtkVector3d v2 = i > 0 ? (mPolygonPoints[i - 1] - mPolygonPoints[i]) : (mPolygonPoints[n - 1] - mPolygonPoints[i]);
        const vtkVector3d v3 = mPolygonPoints[j] - mPolygonPoints[i];

        std::vector<double> crossProducts(3);
        crossProducts[0] = v1.Cross(v2).Dot(mNormal);// V1xV2
        crossProducts[1] = v1.Cross(v3).Dot(mNormal); // V1xV3
        crossProducts[2] = v3.Cross(v2).Dot(mNormal); // V3xV2

        const bool angleLessEqual180 = (crossProducts[0] > 0 || abs(crossProducts[0]) < epsilon) &&
                                       (crossProducts[1] > 0 || abs(crossProducts[1]) < epsilon) &&
                                       (crossProducts[2] > 0 || abs(crossProducts[2]) < epsilon);
        const bool angleGreaterThan180 = (crossProducts[0] < 0) &&
                                         ((crossProducts[1] > 0 || abs(crossProducts[1]) < epsilon) ||
                                          (crossProducts[2] > 0 || abs(crossProducts[2]) < epsilon));

        if(angleLessEqual180 || angleGreaterThan180)
            return true;

        return false;
    }

    std::vector<std::pair<int, int>> OptimalPolygonTriangulation::GetDiagonals() const
    {
        std::vector<std::pair<int, int>> diagonalVec;

        int diagonals = mPolygonPoints.size() * (mPolygonPoints.size() - 3);
        diagonals = diagonals >> 1; // n(n-3)/2 for convex polygons

        int index = 0;
        int end;
        if(mIsConvex)
        {
            while(diagonals > 0)
            {
                end = (index > 0) ? mPolygonPoints.size() - 1 : mPolygonPoints.size() - 2;
                for(int i = index + 2; i <= end; ++i)
                {
                    diagonalVec.push_back(std::make_pair(index, i));
                    --diagonals;
                }
                ++index;
            }
        }
        else
        {
            while(diagonals > 0)
            {
                end = (index > 0) ? mPolygonPoints.size() - 1 : mPolygonPoints.size() - 2;
                for(int i = index + 2; i <= end; ++i)
                {
                    if(IsValidDiagonal(index, i))
                        diagonalVec.push_back(std::make_pair(index, i));
                    --diagonals;
                }
                ++index;
            }
        }

        return diagonalVec;
    }

    /**
     * a triangle consists of 1 edge and 2 diagonals or 2 edges and 1 diagonal or 3 diagonals
     * if a line segment is neither edge nor diagonal, it can't be a part of a triangle
     */
    bool OptimalPolygonTriangulation::IsTriangle(const std::vector<std::vector<bool>>& diagonalMatrix, const int i, const int j, const int k) const
    {
        auto absValue = abs(i - j);
        bool edge = ((absValue == 1) || (absValue == (mPolygonPoints.size() - 1)));
        if(!(edge || diagonalMatrix[i][j]))
            return false;

        absValue = abs(i - k);
        edge = ((absValue == 1) || (absValue == (mPolygonPoints.size() - 1)));
        if(!(edge || diagonalMatrix[i][k]))
            return false;

        absValue = abs(j - k);
        edge = ((absValue == 1) || (absValue == (mPolygonPoints.size() - 1)));
        if(!(edge || diagonalMatrix[j][k]))
            return false;

        return true;
    }

    void OptimalPolygonTriangulation::ConcaveTriangulation()
    {
        const auto size = mPolygonPoints.size();

        //get all valid diagonals
        std::vector<std::pair<int, int>> validDiagonals = GetDiagonals();
        std::vector<std::vector<bool>> diagonalMatrix(size);
        for(int i = 0; i < size; ++i)
        {
            std::vector<bool> entry(size, false);
            diagonalMatrix[i] = entry;
        }

        //initialize valid diagonals
        for(const auto& diagonal: validDiagonals)
        {
            diagonalMatrix[diagonal.first][diagonal.second] = true;
            diagonalMatrix[diagonal.second][diagonal.first] = true;
        }

        // table to store results of subproblems.  table[i][j] stores cost of triangulation of points from i to j.
        // The entry table[0][n-1] stores the final result.
        std::vector<std::vector<Weights>> table(size);
        for(int i = 0; i < size; ++i)
        {
            std::vector<Weights> entry(size);
            table[i] = entry;
        }

        //dynamic programming
        for(int gap = 0; gap < size; gap++)
        {
            for(int i = 0, j = gap; j < size; i++, j++)
            {
                if(j < i + 2)
                    table[i][j].weight = 0;
                else if(j == i + 2)
                {
                    if(diagonalMatrix[i][j])
                        table[i][j].weight = mWeightFunc(mPolygonPoints[i], mPolygonPoints[i + 1], mPolygonPoints[i + 2]);
                    else
                        table[i][j].weight = DBL_MAX;
                }
                else
                {
                    table[i][j].weight = DBL_MAX;
                    if((j < size - 1) && !diagonalMatrix[i][j]) //ij must be a diagonal, except 0 -> n - 1
                    {
                        continue;
                    }

                    for(int k = i + 1; k < j; k++)
                    {
                        if(!IsTriangle(diagonalMatrix, i, j, k))
                        {
                            continue;
                        }

                        const double weight = table[i][k].weight + table[k][j].weight + mWeightFunc(mPolygonPoints[i], mPolygonPoints[j], mPolygonPoints[k]);

                        if(table[i][j].weight > weight)
                        {
                            table[i][j].weight = weight;
                            table[i][j].k = k;
                        }
                    }
                }
            }
        }

        AddNewTriangles(table, 0, size - 1);
    }

    void OptimalPolygonTriangulation::GeneratePolyData()
    {
        mTriangulatedPolygon = MeshUtil::GetPolyData(mPolygonPoints, mNewTriangles);
    }

    void OptimalPolygonTriangulation::Triangulate()
    {
        mNewTriangles = vtkSmartPointer<vtkCellArray>::New();

        if(mPolygonPoints.size() < 3)
            return;

        if(mPolygonPoints.size() == 3)
        {
            AddNewTriangle(0, 1, 2);
            GeneratePolyData();
            return;
        }

        if(mIsConvex)
            ConvexTriangulation();
        else
            ConcaveTriangulation();

        GeneratePolyData();
    }

    vtkSmartPointer<vtkPolyData> OptimalPolygonTriangulation::GetTriangulatedPolygon() const
    {
        return mTriangulatedPolygon;
    }
}
