#include "ImageDataUtil.h"

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkSphere.h>
#include <vtkSampleFunction.h>


namespace ImageDataUtil
{
    vtkVector3d GetVoxelPositionFromIndex(vtkImageData* imageData, const int x, const int y, const int z)
    {
        auto dims = imageData->GetDimensions();
        auto origin = imageData->GetOrigin();
        auto spacing = imageData->GetSpacing();

        vtkVector3d pos;
        pos[0] = origin[0] + x * spacing[0];
        pos[1] = origin[1] + y * spacing[1];
        pos[2] = origin[2] + z * spacing[2];

        return pos;
    }

    vtkVector3i GetVoxelIndexFromPosition(vtkImageData* imageData, const double pos[3])
    {
        double origin[3];
        double spacing[3];
        imageData->GetOrigin(origin);
        imageData->GetSpacing(spacing);

        vtkVector3i voxelIndex;
        for(int i = 0; i < 3; ++i)
        {
            voxelIndex[i] = static_cast<int>((pos[i] - origin[i]) / spacing[i]);
        }

        return voxelIndex;
    }

    std::vector<std::tuple<int, int, int>> GetVoxelIndicesWithinSphere(const int dimension[3], const double origin[3], const double spacing[3],
                                                                       const double sphereCenter[3], const double sphereRadius)
    {
        //create a sphere implicit function
        auto sphere = vtkSmartPointer<vtkSphere>::New();
        sphere->SetCenter(sphereCenter);
        sphere->SetRadius(sphereRadius);

        //calculate the bounding box around the sphere
        int minX = std::max(0, static_cast<int>((sphereCenter[0] - sphereRadius - origin[0]) / spacing[0]));
        int maxX = std::min(dimension[0] - 1, static_cast<int>((sphereCenter[0] + sphereRadius - origin[0]) / spacing[0]));
        int minY = std::max(0, static_cast<int>((sphereCenter[1] - sphereRadius - origin[1]) / spacing[1]));
        int maxY = std::min(dimension[1] - 1, static_cast<int>((sphereCenter[1] + sphereRadius - origin[1]) / spacing[1]));
        int minZ = std::max(0, static_cast<int>((sphereCenter[2] - sphereRadius - origin[2]) / spacing[2]));
        int maxZ = std::min(dimension[2] - 1, static_cast<int>((sphereCenter[2] + sphereRadius - origin[2]) / spacing[2]));

        //sample the implicit function over the image data within the bounding box
        auto sampleFunction = vtkSmartPointer<vtkSampleFunction>::New();
        sampleFunction->SetImplicitFunction(sphere);
        sampleFunction->SetModelBounds(origin[0] + minX * spacing[0], origin[0] + (maxX + 1) * spacing[0],
                                       origin[1] + minY * spacing[1], origin[1] + (maxY + 1) * spacing[1],
                                       origin[2] + minZ * spacing[2], origin[2] + (maxZ + 1) * spacing[2]);
        sampleFunction->SetSampleDimensions(maxX - minX + 1, maxY - minY + 1, maxZ - minZ + 1);
        sampleFunction->ComputeNormalsOff();
        sampleFunction->Update();

        //get the output image data
        vtkImageData* sampledImage = sampleFunction->GetOutput();
        std::vector<std::tuple<int, int, int>> selectedVoxels;
        for(int z = minZ; z <= maxZ; ++z)
        {
            for(int y = minY; y <= maxY; ++y)
            {
                for(int x = minX; x <= maxX; ++x)
                {
                    double value = sampledImage->GetScalarComponentAsDouble(x - minX, y - minY, z - minZ, 0);
                    if(value <= 0.0) // Inside the sphere
                    {
                        selectedVoxels.push_back({x, y, z});
                    }
                }
            }
        }

        return selectedVoxels;
    }
}
