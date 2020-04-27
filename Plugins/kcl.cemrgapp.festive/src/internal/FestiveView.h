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
 * Eikonal Activation Simulation (EASI) Plugin for MITK
 *
 * Cardiac Electromechanics Research Group
 * http://www.cemrg.co.uk/
 * orod.razeghi@kcl.ac.uk
 *
 * This software is distributed WITHOUT ANY WARRANTY or SUPPORT!
 *
 =========================================================================*/


#ifndef FestiveView_h
#define FestiveView_h

#include <berryISelectionListener.h>
#include <QmitkAbstractView.h>
#include <CemrgCommandLine.h>
#include "ui_FestiveViewUILogin.h"
#include "ui_FestiveViewUIReconst.h"
#include "ui_FestiveViewUIReconstSVR.h"
#include "ui_FestiveViewUIReconstDSVR.h"
#include "ui_FestiveViewControls_new.h"
#include "ui_FestiveViewUIPatientInfo.h"
#include "ui_FestiveViewUIServerInfo.h"
#include "ui_FestiveViewUIReorient.h"
#include "ui_FestiveViewUIRegister.h"
#include "ui_FestiveViewUIAtlas.h"
#include "ui_FestiveViewUIAtlasBody.h"
#include "ui_FestiveViewUIReorientReference.h"
#include "ui_FestiveViewUILabelPropagation.h"

#include <QStandardPaths>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>

#include <csignal>
#include <iostream>

/**
 \brief FestiveView
 
 \warning  This class is not yet documented. Use "git blame" and ask the author to provide basic documentation.
 
 \sa QmitkAbstractView
 \ingroup ${plugin_target}_internal
 */
class FestiveView : public QmitkAbstractView
{
    // this is needed for all Qt objects that should have a Qt meta-object
    // (everything that derives from QObject and wants to have signal/slots)
    Q_OBJECT
    
    public:
    static const std::string VIEW_ID;
    
    protected slots:
    /// \brief Called when the user clicks the GUI button
    void LoadDICOM();
    void ProcessIMGS();
    void ConvertNII();
    void MaskIMGS();
    void SaveMASK();
    void EstablishConnection();
    void CopyServer();
    void ReconstructionGSVR();
    void ReconstructionSVR();
    void ReconstructionDSVR();
    void Download();
    void SaveSubjectFolder();
    void Reset();
    void ExtractDynamics();
    void RegisterVolumes();
    void StopReconstruction();
    void CheckServer();
    void ReorientVolume();
    
    void ResetOrigin();
    void MaskVolume();
    void CropVolume();
    void ReorientVolumeReference();
    void N4Correction();
    void LoadAtlas();
    void LoadBodyAtlas();
    void LoadMeasurements();
    void ThresholdMask();
    void ExtractPackages();
    void ProcessVolumes();
    
    void PropagateLabels();
    void LabelVolume();
    
    
    protected:
    virtual void CreateQtPartControl(QWidget *parent) override;
    virtual void SetFocus() override;
    
    /// \brief called by QmitkFunctionality when DataManager's selection has changed
    virtual void OnSelectionChanged(berry::IWorkbenchPart::Pointer source, const QList<mitk::DataNode::Pointer>& nodes) override;
    
    Ui::FestiveViewUILogin m_UILogin;
    Ui::FestiveViewUIReconst m_UIReconst;
    Ui::FestiveViewUIReconstSVR m_UIReconstSVR;
    Ui::FestiveViewUIReconstDSVR m_UIReconstDSVR;
    Ui::FestiveViewControls_new m_Controls;
    Ui::FestiveViewUIPatientInfo m_SubjectCaseInfo;
    Ui::FestiveViewUIServerInfo m_ServerInfo;
    Ui::FestiveViewUIReorient m_Reorient;
    Ui::FestiveViewUIRegister m_Register;
    Ui::FestiveViewUIAtlas m_Atlas;
    Ui::FestiveViewUIAtlasBody m_AtlasBody;
    Ui::FestiveViewUIReorientReference m_ReorientReference;
    Ui::FestiveViewUILabelPropagation m_LabelPropagation;
    
    
    
    private:
    QString userID;
    QString server;
    QString outName;
    QString directory;
    QString directory_processing;
    QStringList imgsList;
    QStringList imgsList_preproc;
    QString main_mirtk_path;
    QString brain_atlas_path;
    QString body_atlas_path;
    
    
    std::unique_ptr<CemrgCommandLine> cmd;
    std::unique_ptr<CemrgCommandLine> current_svr_recon_cmd;
    std::unique_ptr<CemrgCommandLine> current_dsvr_recon_cmd;
    std::unique_ptr<CemrgCommandLine> current_gsvr_recon_cmd;
    std::unique_ptr<CemrgCommandLine> check_cmd;
    
    
};

#endif // FestiveView_h

