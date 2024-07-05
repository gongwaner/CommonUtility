#pragma once

#include <vtkSmartPointer.h>
#include <vtkVector.h>
#include <vtkColorSeries.h>

#include <vector>
#include <unordered_set>

#include "../Common/Color.h"


class vtkPolyData;

namespace MeshFeatureUtil
{
    template<typename T>
    void ClampVector(std::vector<T>& vector, const T& min, const T& max)
    {
        std::transform(std::begin(vector), std::end(vector), std::begin(vector),
                       [&](const T& value)
                       {
                           return std::clamp(value, min, max);
                       });
    }

    vtkSmartPointer<vtkPolyData> GetLargestComponent(vtkPolyData* polyData);

    /**
     * Get all connected components of a given input polydata
     * @param minCellsCnt for optimization purpose, skip components whose cell cnt < specified minimum cells cnt
     */
    std::vector<vtkSmartPointer<vtkPolyData>> GetAllComponents(vtkSmartPointer<vtkPolyData> polyData, int minCellsCnt);

    /**
     * Extract the topological neighbors of given vid
     * @return vector of neighbor vids as std::unordered_set
     */
    std::unordered_set<int> GetNeighborVids(vtkPolyData* polyData, int vid);

    /**
     * Extract the topological n ring neighbors vids of given vid
     * @return vector of n ring neighbor vids. Note: the input vertex itself is included in result.
     */
    std::vector<int> GetNRingNeighbors(vtkPolyData* polyData, int vid, int nring);

    /**
     * Color mapping of scalar values
     * @param scalarID array name for getting scalar values
     * @param colorScheme color scheme for transfer function
     */
    void VisualizeFeature(vtkSmartPointer<vtkPolyData> polyData, const char* scalarID,
                          double clampMin, double clampMax,
                          int windowWidth = 800, int windowHeight = 800,
                          vtkColorSeries::ColorSchemes colorScheme = vtkColorSeries::ColorSchemes::BREWER_DIVERGING_SPECTRAL_10);

    /**
     * Map the value in feature vector to pixel color
     * @param polyData
     * @param feature the vector should be clamped before passing into this function
     * @param initColor if feature size is less than poly vertices cnt, assign it a default color
     */
    void ColorMapping(vtkSmartPointer<vtkPolyData> polyData, const std::vector<double>& feature, const vtkVector4d& initColor = Color::White);
}
