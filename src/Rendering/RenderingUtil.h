#pragma once

#include <vtkSmartPointer.h>


class vtkImageData;
class vtkRenderWindow;

namespace RenderingUtil
{
    vtkSmartPointer<vtkImageData> GetScreenshot(vtkRenderWindow* renderWindow);
}
