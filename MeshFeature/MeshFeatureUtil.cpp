#include "MeshFeatureUtil.h"

#include <vtkColorTransferFunction.h>
#include <vtkNamedColors.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkScalarBarActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <queue>


namespace MeshFeatureUtil
{
    vtkSmartPointer<vtkColorTransferFunction> GetLookUpTable(vtkColorSeries::ColorSchemes colorScheme, const double scalarRange[2])
    {
        auto colorSeries = vtkSmartPointer<vtkColorSeries>::New();
        colorSeries->SetColorScheme(colorScheme);
        printf("using color scheme #: %d, %s\n", colorSeries->GetColorScheme(), colorSeries->GetColorSchemeName().c_str());

        auto lut = vtkSmartPointer<vtkColorTransferFunction>::New();
        lut->SetColorSpaceToHSV();

        //use a color series to create a transfer function
        auto numColors = colorSeries->GetNumberOfColors();
        auto interval = scalarRange[1] - scalarRange[0];
        const double scale = 1.0 / 255.0;
        for(int i = 0; i < numColors; i++)
        {
            vtkColor3ub color = colorSeries->GetColor(i);

            double dColor[3];
            for(int k = 0; k < 3; ++k)
                dColor[k] = (double) color[k] * scale;

            double t = scalarRange[0] + interval / (numColors - 1) * i;
            lut->AddRGBPoint(t, dColor[0], dColor[1], dColor[2]);
        }

        return lut;
    }

    void VisualizeFeature(vtkSmartPointer<vtkPolyData> polyData, const char* scalarID,
                          const double clampMin, const double clampMax,
                          const int windowWidth, const int windowHeight,
                          vtkColorSeries::ColorSchemes colorScheme)
    {
        auto scalarArray = polyData->GetPointData()->GetScalars(scalarID);
        if(!scalarArray)
        {
            std::cerr << "ERROR: No scalar array named \"" << scalarID << "\" exists!" << std::endl;
            return;
        }

        double scalarRange[2]{clampMin, clampMax};
        auto lut = GetLookUpTable(colorScheme, scalarRange);

        auto actor = vtkSmartPointer<vtkActor>::New();
        {
            auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            mapper->SetInputData(polyData);
            mapper->SetScalarModeToUsePointFieldData();
            mapper->SelectColorArray(scalarID);
            mapper->SetScalarRange(scalarRange);
            mapper->SetLookupTable(lut);

            actor->SetMapper(mapper);
        }

        auto scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
        scalarBar->SetLookupTable(lut);
        scalarBar->SetTitle(scalarID);
        scalarBar->UnconstrainedFontSizeOn();
        scalarBar->SetNumberOfLabels(5);
        scalarBar->SetMaximumWidthInPixels(windowWidth / 8);
        scalarBar->SetMaximumHeightInPixels(windowHeight / 3);

        auto renderer = vtkSmartPointer<vtkRenderer>::New();
        auto renWin = vtkSmartPointer<vtkRenderWindow>::New();
        renWin->AddRenderer(renderer);
        renWin->SetSize(windowWidth, windowHeight);
        renWin->SetWindowName("Feature Visualization");

        auto iRen = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        iRen->SetRenderWindow(renWin);
        //the interactor must be set prior to enabling the widget.
        iRen->SetRenderWindow(renWin);

        //add the actors to the scene
        auto colors = vtkSmartPointer<vtkNamedColors>::New();
        renderer->AddActor(actor);
        renderer->AddActor2D(scalarBar);
        renderer->SetBackground(colors->GetColor3d("DarkSlateGray").GetData());

        renWin->Render();
        iRen->Start();
    }

    void ColorMapping(vtkSmartPointer<vtkPolyData> polyData, const std::vector<double>& feature, const vtkVector4d& initColor)
    {
        auto colorArray = vtkSmartPointer<vtkUnsignedCharArray>::New();
        colorArray->SetNumberOfComponents(4);
        colorArray->SetName(Color::ColorArrayName.c_str());

        //in case feature.size() < polyData->GetNumberOfPoints(), assign it a default color
        for(int i = 0; i < polyData->GetNumberOfPoints(); ++i)
        {
            colorArray->InsertNextTuple4(initColor[0], initColor[1], initColor[2], initColor[3]);
        }

        double min = *std::min_element(feature.begin(), feature.end());
        double max = *std::max_element(feature.begin(), feature.end());
        double interval = max - min;
        printf("min = %f, max = %f, interval = %f\n", min, max, interval);

        auto colorVector = Color::ColorJet;
        for(int i = 0; i < feature.size(); ++i)
        {
            double t = (feature[i] - min) / interval;
            int index = (int) (t * colorVector.size());
            if(index >= colorVector.size())
                index = colorVector.size() - 1;
            auto color = colorVector[index];
            colorArray->SetTuple4(i, color[0], color[1], color[2], color[3]);
        }

        polyData->GetPointData()->AddArray(colorArray);
    }
}
