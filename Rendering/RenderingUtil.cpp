#include "RenderingUtil.h"

#include <vtkImageData.h>
#include <vtkRenderWindow.h>
#include <vtkWindowToImageFilter.h>


namespace RenderingUtil
{
    vtkSmartPointer<vtkImageData> GetScreenshot(vtkRenderWindow* renderWindow)
    {
        auto windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
        windowToImageFilter->SetInput(renderWindow);
        windowToImageFilter->Update();

        return windowToImageFilter->GetOutput();
    }
}
