#pragma once

#include <vtkVector.h>

class vtkImageData;

namespace ImageDataUtil
{
    vtkVector3d GetVoxelPositionFromIndex(vtkImageData* imageData, int x, int y, int z);

    vtkVector3i GetVoxelIndexFromPosition(vtkImageData* imageData, const double pos[3]);

    //given imagedata dimension, origin, spacing, and specified sphere center and radius, return the voxel indices within the sphere
    std::vector<std::tuple<int, int, int>> GetVoxelIndicesWithinSphere(const int dimension[3], const double origin[3], const double spacing[3],
                                                                       const double sphereCenter[3], double sphereRadius);
}
