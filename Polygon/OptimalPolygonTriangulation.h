#pragma once

#include <vtkVector.h>
#include <vtkSmartPointer.h>
#include <functional>


class vtkPolyData;
class vtkCellArray;

namespace Algorithm::Triangulation
{
    struct Weights
    {
        double weight = 0.0;
        int k = 0;
    };

    /**
     * Optimal(aka.minimum weight) triangulation using dynamic programming
     * given customized weight function
     */
    class OptimalPolygonTriangulation
    {
    public:
        void SetPolygonPoints(const std::vector<vtkVector3d>& polygonPoints);
        void SetNormal(const vtkVector3d& normal);
        void SetWeightFunction(const std::function<double(const vtkVector3d&, const vtkVector3d&, const vtkVector3d&)>& weightFunc);

        void Triangulate();

        vtkSmartPointer<vtkPolyData> GetTriangulatedPolygon() const;

    private:
        void AddNewTriangle(int vid0, int vid1, int vid2);
        void AddNewTriangles(const std::vector<std::vector<Weights>>& table, int i, int j);
        void GeneratePolyData();
        void ConvexTriangulation();
        void ConcaveTriangulation();

        bool IsValidDiagonal(int i, int j, double epsilon = 1e-6) const;
        bool IsTriangle(const std::vector<std::vector<bool>>& diagonalMatrix, int i, int j, int k) const;

        std::vector<std::pair<int, int>> GetDiagonals() const;

    private:
        std::vector<vtkVector3d> mPolygonPoints;
        vtkVector3d mNormal{0, 0, 1};
        std::function<double(const vtkVector3d&, const vtkVector3d&, const vtkVector3d&)> mWeightFunc;
        bool mIsConvex = false;
        vtkSmartPointer<vtkCellArray> mNewTriangles = nullptr;
        vtkSmartPointer<vtkPolyData> mTriangulatedPolygon = nullptr;
    };
}
