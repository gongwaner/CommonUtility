#pragma once

#include <vtkSmartPointer.h>
#include <vtkVector.h>
#include <vtkColorSeries.h>

#include <vector>

#include "Color.h"


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
