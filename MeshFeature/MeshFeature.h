#pragma once

#include <string>
#include <vector>

class vtkPolyData;

namespace MeshFeature
{
    const std::string MeanCurvatureID = "Mean_Curvature";
    const std::string GaussianCurvatureID = "Gauss_Curvature";

    std::vector<double> GetMeanCurvature(vtkPolyData* inputPolyData);

    std::vector<double> GetGaussianCurvature(vtkPolyData* inputPolyData);

    /**
     * Calculate per-vertex feature value of PCA eigen values
     * @param nring nring neighbors for PCA eigen values calculation
     * @return feature vector magnitude of PCA eigen values
     */
    std::vector<double> GetPCAFeature(vtkPolyData* inputPolyData, int nring);

    /**
     * An optimized version of PCA feature calculation.
     * reduce calculation size by only calculating selected indices
     */
    std::vector<double> GetPCAFeature(vtkPolyData* inputPolyData, const std::vector<int>& selectedIndices, int nring);
}
