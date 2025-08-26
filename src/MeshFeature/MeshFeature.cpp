#include "MeshFeature.h"

#include <vtkPolyData.h>
#include <vtkCurvatures.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkVectorOperators.h>

#include "MeshFeatureUtil.h"
#include "TopologyUtil.h"


namespace MeshFeature
{
    std::vector<double> GetMeanCurvature(vtkPolyData* inputPolyData)
    {
        auto curvatureFilter = vtkSmartPointer<vtkCurvatures>::New();
        curvatureFilter->SetInputData(inputPolyData);
        curvatureFilter->SetCurvatureTypeToMean();
        curvatureFilter->Update();

        //behind the scene vtk puts calculated value into a vtkPolyData,
        //in order to get the values we get it from an array
        auto meanCurvature = dynamic_cast<vtkDoubleArray*>(curvatureFilter->GetOutput()->GetPointData()->GetAbstractArray(MeanCurvatureID.c_str()));

        const int numOfPoints = inputPolyData->GetNumberOfPoints();
        std::vector<double> curvatureVec;
        curvatureVec.reserve(numOfPoints);

        for(int i = 0; i < numOfPoints; ++i)
        {
            curvatureVec.push_back(meanCurvature->GetValue(i));
        }

        return curvatureVec;
    }

    std::vector<double> GetGaussianCurvature(vtkPolyData* inputPolyData)
    {
        auto curvatureFilter = vtkSmartPointer<vtkCurvatures>::New();
        curvatureFilter->SetInputData(inputPolyData);
        curvatureFilter->SetCurvatureTypeToGaussian();
        curvatureFilter->Update();

        //behind the scene vtk puts calculated value into a vtkPolyData,
        //in order to get the values we get it from an array
        auto meanCurvature = dynamic_cast<vtkDoubleArray*>(curvatureFilter->GetOutput()->GetPointData()->GetAbstractArray(GaussianCurvatureID.c_str()));

        const int numOfPoints = inputPolyData->GetNumberOfPoints();
        std::vector<double> curvatureVec;
        curvatureVec.reserve(numOfPoints);

        for(int i = 0; i < numOfPoints; ++i)
        {
            curvatureVec.push_back(meanCurvature->GetValue(i));
        }

        return curvatureVec;
    }

    vtkVector3d GetAverage(const std::vector<vtkVector3d>& points)
    {
        vtkVector3d avg(0, 0, 0);
        for(const auto& point: points)
        {
            avg += point;
        }

        for(int i = 0; i < 3; ++i)
            avg[i] /= (double) points.size();

        return avg;
    }

    std::vector<std::vector<double>> GetTransposeMatrix(const std::vector<vtkVector3d>& data)
    {
        const int row = 3;
        const int column = data.size();
        std::vector<std::vector<double>> transposeMatrix(row, std::vector<double>(column));

        for(int i = 0; i < column; ++i)
        {
            for(int j = 0; j < row; ++j)
            {
                transposeMatrix[j][i] = data[i][j];
            }
        }

        return transposeMatrix;
    }

    std::vector<double> GetEigenValues(std::vector<vtkVector3d>& points)
    {
        //subtract mean from the data to center it
        const auto center = GetAverage(points);

        for(auto& point: points)
            point -= center;

        //compute the covariance matrix
        const int size = 3;
        const auto transposeMatrix = GetTransposeMatrix(points);
        double covarianceMatrix[size][size];
        for(int i = 0; i < size; ++i)
        {
            for(int j = 0; j < size; ++j)
            {
                covarianceMatrix[i][j] = 0;
            }
        }

        auto numOfPoints = points.size();
        for(int i = 0; i < size; ++i)
        {
            for(int j = 0; j < size; ++j)
            {
                for(int k = 0; k < numOfPoints; ++k)
                {
                    covarianceMatrix[i][j] += (transposeMatrix[i][k] * points[k][j]) / (double) (numOfPoints - 1);
                }
            }
        }

        //perform SVD
        double U[size][size];
        double w[size];//eigen values
        double VT[size][size];//v is eigen vectors
        vtkMath::SingularValueDecomposition3x3(covarianceMatrix, U, w, VT);

        // the output of W needs to be handled (unsigned and sorted)
        std::vector<double> sortedEigenValues(size);
        for(int i = 0; i < size; ++i)
            sortedEigenValues[i] = abs(w[i]);
        std::sort(sortedEigenValues.begin(), sortedEigenValues.end(), std::greater<>());

        return sortedEigenValues;
    }

    double GetEigenValuesFeatureVectorMagnitude(const std::vector<double>& eigenValues)
    {
        const double epsilon = 1e-6;
        for(const auto& value: eigenValues)
        {
            if(abs(value) < epsilon)
            {
                std::cerr << "eigen value = " << value << std::endl;
                return -1;
            }
        }

        const double sum = eigenValues[0] + eigenValues[1] + eigenValues[2];

        std::vector<double> featureVector;
        const int iterCnt = 3;
        for(int i = 0; i < iterCnt; ++i)
        {
            featureVector.push_back(eigenValues[i] / sum);
            featureVector.push_back((eigenValues[i] + eigenValues[(i + 1) % iterCnt]) / sum);
        }

        double e1Dive2 = eigenValues[0] / eigenValues[1];
        double e1Dive3 = eigenValues[0] / eigenValues[2];
        double e2Dive3 = eigenValues[1] / eigenValues[2];

        featureVector.push_back(e1Dive2);
        featureVector.push_back(e1Dive3);
        featureVector.push_back(e2Dive3);
        featureVector.push_back(e1Dive2 + e1Dive3);
        featureVector.push_back(e1Dive2 + e2Dive3);
        featureVector.push_back(e1Dive3 + e2Dive3);

        //calculate vector length
        double vecLength = 0.0;
        for(const auto& value: featureVector)
        {
            vecLength += value * value;
        }
        vecLength = std::sqrt(vecLength);

        return vecLength;
    }

    std::vector<double> GetPCAFeature(vtkPolyData* inputPolyData, const int nring)
    {
        const auto verticesCnt = inputPolyData->GetNumberOfPoints();
        std::vector<double> pcaFeatureVec(verticesCnt);

        for(int i = 0; i < verticesCnt; ++i)
        {
            const auto neighbors = TopologyUtil::GetNRingNeighbors(inputPolyData, i, nring);

            std::vector<vtkVector3d> points;
            points.reserve(neighbors.size());
            for(const auto& neighbor: neighbors)
            {
                auto pos = inputPolyData->GetPoint(neighbor);
                points.push_back(vtkVector3d(pos));
            }

            auto eigenValues = GetEigenValues(points);
            pcaFeatureVec[i] = GetEigenValuesFeatureVectorMagnitude(eigenValues);
        }

        return pcaFeatureVec;
    }

    std::vector<double> GetPCAFeature(vtkPolyData* inputPolyData, const std::vector<int>& selectedIndices, const int nring)
    {
        const auto cnt = selectedIndices.size();
        std::vector<double> pcaFeatureVec(cnt);

        for(int i = 0; i < cnt; ++i)
        {
            const auto vid = selectedIndices[i];
            const auto neighbors = TopologyUtil::GetNRingNeighbors(inputPolyData, vid, nring);

            std::vector<vtkVector3d> points;
            points.reserve(neighbors.size());
            for(const auto& neighbor: neighbors)
            {
                auto pos = inputPolyData->GetPoint(neighbor);
                points.push_back(vtkVector3d(pos));
            }

            auto eigenValues = GetEigenValues(points);
            pcaFeatureVec[i] = GetEigenValuesFeatureVectorMagnitude(eigenValues);
        }

        return pcaFeatureVec;
    }
}
