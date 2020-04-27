/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
/*=========================================================================
 *
 * Strain Calculations Tools for MITK
 *
 * Cardiac Electromechanics Research Group
 * http://www.cemrg.co.uk/
 * orod.razeghi@kcl.ac.uk
 *
 * This software is distributed WITHOUT ANY WARRANTY or SUPPORT!
 *
=========================================================================*/

#ifndef CemrgStrains_h
#define CemrgStrains_h

#include <mitkSurface.h>
#include <vtkCell.h>
#include <vtkFloatArray.h>
#include <MitkCemrgAppModuleExports.h>
// #include <MyCemrgLibExports.h>


class MITKCEMRGAPPMODULE_EXPORT CemrgStrains {

public:

    /**
     * Strain Test coordinates
     **/
    int controller = 0;
    std::vector<mitk::Surface::Pointer> temporaries;
    CemrgStrains();



    CemrgStrains(QString dir, int refMeshNo);
    ~CemrgStrains();

    std::vector<double> CalculateSqzPlot(int meshNo);
    std::vector<double> CalculateStrainsPlot(int meshNo, mitk::DataNode::Pointer lmNode, int flag);
    double CalculateSDI(std::vector<std::vector<double>> valueVectors, int cycleLengths, int noFrames);

    std::vector<mitk::Surface::Pointer> ReferenceGuideLines(mitk::DataNode::Pointer lmNode);
    mitk::Surface::Pointer ReferenceAHA(mitk::DataNode::Pointer lmNode, int segRatios[]);
    mitk::Surface::Pointer FlattenedAHA();
    vtkSmartPointer<vtkFloatArray> GetFlatSurfScalars() const;
    std::vector<float> GetAHAColour(int label);

    /**
      * Tests change to protected
      **/
public:

    mitk::Point3D ZeroPoint(mitk::Point3D apex, mitk::Point3D point);
    void ZeroVTKMesh(mitk::Point3D apex, mitk::Surface::Pointer surface);

    mitk::Point3D RotatePoint(mitk::Matrix<double,3,3> rotationMatrix, mitk::Point3D point);
    void RotateVTKMesh(mitk::Matrix<double,3,3> rotationMatrix, mitk::Surface::Pointer surface);

    double GetCellArea(vtkSmartPointer<vtkPolyData> pd, vtkIdType cellID);
    mitk::Point3D GetCellCenter(vtkSmartPointer<vtkPolyData> pd, vtkIdType cellID);
    mitk::Matrix<double,3,3> GetCellAxes(vtkSmartPointer<vtkCell> &cell, mitk::Point3D &termPt, mitk::Matrix<double,3,3> &J);

private:

    mitk::Surface::Pointer ReadVTKMesh(int refMshNo);
    std::vector<mitk::Point3D> ConvertMPS(mitk::DataNode::Pointer node);

    double Norm(mitk::Point3D vec);
    double Dot(mitk::Point3D vec1, mitk::Point3D vec2);
    mitk::Point3D Cross(mitk::Point3D vec1, mitk::Point3D vec2);
    std::vector<double> GetMinMax(vtkSmartPointer<vtkPolyData> pd, int dimension);
    mitk::Point3D Circlefit3d(mitk::Point3D point1, mitk::Point3D point2, mitk::Point3D point3);
    mitk::Matrix<double,3,3> CalcRotationMatrix(mitk::Point3D point1, mitk::Point3D point2);

    void AssignpLabels(int layer, std::vector<double>& pLabel, std::vector<int> index, std::vector<double> pAngles, double sepA, double freeA);
    void AssigncLabels(int layer, std::vector<int>& refCellLabels, std::vector<int> index, std::vector<double> cAngles, double sepA, double freeA);

    QString projectDirectory;
    std::vector<double> refArea;
    std::vector<double> refAhaArea;
    std::vector<mitk::Matrix<double,3,3>> refJ;
    std::vector<mitk::Matrix<double,3,3>> refQ;
    std::vector<int> refCellLabels;
    std::vector<double> refPointLabels;
    mitk::Surface::Pointer refSurface;
    mitk::Surface::Pointer flatSurface;
    vtkSmartPointer<vtkFloatArray> flatSurfScalars;
};

#endif // CemrgStrains_h
