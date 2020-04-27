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


// Blueberry
#include <berryISelectionService.h>
#include <berryIWorkbenchWindow.h>
#include <berryIWorkbenchPage.h>
#include <berryFileEditorInput.h>

// Qmitk
#include <QmitkIOUtil.h>
#include <mitkIOUtil.h>
#include <mitkProgressBar.h>
#include <mitkIDataStorageService.h>
#include <mitkNodePredicateNot.h>
#include <mitkNodePredicateProperty.h>
#include <mitkDataStorageEditorInput.h>
#include <mitkDataStorage.h>
#include "kcl_cemrgapp_festive_Activator.h"
#include "FestiveView.h"

// Qt
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

// mitk image
#include <mitkImage.h>

// CemrgAppModule
#include <CemrgCommandLine.h>

const std::string FestiveView::VIEW_ID = "org.mitk.views.festive";

void FestiveView::SetFocus(){
    m_Controls.button_1->setFocus();
}

void FestiveView::CreateQtPartControl(QWidget *parent)
{
    //create GUI widgets from the Qt Designer's .ui file
    m_Controls.setupUi(parent);
    connect(m_Controls.button_1, SIGNAL(clicked()), this, SLOT(LoadDICOM()));
    connect(m_Controls.button_2, SIGNAL(clicked()), this, SLOT(ProcessIMGS()));
    connect(m_Controls.button_3, SIGNAL(clicked()), this, SLOT(MaskIMGS()));
    connect(m_Controls.button_4, SIGNAL(clicked()), this, SLOT(EstablishConnection()));
    connect(m_Controls.button_5, SIGNAL(clicked()), this, SLOT(CopyServer()));
    connect(m_Controls.button_6, SIGNAL(clicked()), this, SLOT(ReconstructionGSVR()));
    connect(m_Controls.button_6_1, SIGNAL(clicked()), this, SLOT(ReconstructionSVR()));
    connect(m_Controls.button_6_2, SIGNAL(clicked()), this, SLOT(ReconstructionDSVR()));
    connect(m_Controls.button_7, SIGNAL(clicked()), this, SLOT(Download()));
    connect(m_Controls.button_8, SIGNAL(clicked()), this, SLOT(SaveSubjectFolder()));
    connect(m_Controls.button_r, SIGNAL(clicked()), this, SLOT(Reset()));
    connect(m_Controls.button_9, SIGNAL(clicked()), this, SLOT(ExtractDynamics()));
    connect(m_Controls.button_10, SIGNAL(clicked()), this, SLOT(RegisterVolumes()));
    connect(m_Controls.button_11, SIGNAL(clicked()), this, SLOT(StopReconstruction()));
    connect(m_Controls.button_12, SIGNAL(clicked()), this, SLOT(CheckServer()));
    
    
    connect(m_Controls.button_14, SIGNAL(clicked()), this, SLOT(ReorientVolume()));
    connect(m_Controls.button_18, SIGNAL(clicked()), this, SLOT(ReorientVolumeReference()));
    
    connect(m_Controls.button_16, SIGNAL(clicked()), this, SLOT(ResetOrigin()));
    connect(m_Controls.button_17, SIGNAL(clicked()), this, SLOT(MaskVolume()));
    connect(m_Controls.button_19, SIGNAL(clicked()), this, SLOT(CropVolume()));
    
    connect(m_Controls.button_20, SIGNAL(clicked()), this, SLOT(N4Correction()));
    connect(m_Controls.button_21, SIGNAL(clicked()), this, SLOT(LoadAtlas()));
    
    connect(m_Controls.button_22, SIGNAL(clicked()), this, SLOT(LoadBodyAtlas()));
    connect(m_Controls.button_23, SIGNAL(clicked()), this, SLOT(LoadMeasurements()));
    
    connect(m_Controls.button_24, SIGNAL(clicked()), this, SLOT(ThresholdMask()));
    connect(m_Controls.button_25, SIGNAL(clicked()), this, SLOT(ExtractPackages()));

    connect(m_Controls.button_0, SIGNAL(clicked()), this, SLOT(ProcessVolumes()));
    
    connect(m_Controls.button_28, SIGNAL(clicked()), this, SLOT(PropagateLabels()));
    
    connect(m_Controls.button_29, SIGNAL(clicked()), this, SLOT(LabelVolume()));

    
    m_Controls.button_1->setVisible(false);
    m_Controls.button_2->setVisible(false);
    m_Controls.button_2_1->setVisible(false);
    
    
    //Set visibility of buttons
    m_Controls.button_2_1->setVisible(false);
    m_Controls.button_3_1->setVisible(false);
    connect(m_Controls.button_2_1, SIGNAL(clicked()), this, SLOT(ConvertNII()));
    connect(m_Controls.button_3_1, SIGNAL(clicked()), this, SLOT(SaveMASK()));
    
    //  m_Controls.button_10->setVisible(false);
    
    
    //Set visibility of buttons
    m_Controls.button_14->setVisible(false);
    m_Controls.button_18->setVisible(false);
    m_Controls.button_10->setVisible(false);
    m_Controls.button_16->setVisible(false);
    m_Controls.button_19->setVisible(false);
    m_Controls.button_17->setVisible(false);
    m_Controls.button_24->setVisible(false);
    m_Controls.button_23->setVisible(false);
    m_Controls.button_10->setVisible(false);
    m_Controls.button_9->setVisible(false);
    m_Controls.button_25->setVisible(false);
    m_Controls.button_22->setVisible(false);
    m_Controls.button_21->setVisible(false);
    m_Controls.button_20->setVisible(false);
    m_Controls.button_28->setVisible(false);
    m_Controls.button_29->setVisible(false);
    

    
    QString home_dir = QDir::homePath();
    main_mirtk_path = home_dir + "/Software/mirtk-bin/lib/tools/";
    brain_atlas_path = home_dir + "/Software/mirtk-bin/atlases/brain-atlas/";
    body_atlas_path = home_dir + "/Software/mirtk-bin/atlases/body-atlas/";

}


void FestiveView::OnSelectionChanged(berry::IWorkbenchPart::Pointer /*source*/,
                                     const QList<mitk::DataNode::Pointer>& /*nodes */){
    
    
}


void FestiveView::LoadDICOM() {
    
    //Use MITK DICOM editor
    QString editor_id = "org.mitk.editors.dicomeditor";
    berry::IEditorInput::Pointer input(new berry::FileEditorInput(QString()));
    this->GetSite()->GetPage()->OpenEditor(input, editor_id);
}


void FestiveView::ProcessIMGS() {
    
    //Toggle visibility of buttons
    if (m_Controls.button_2_1->isVisible()) {
        m_Controls.button_2_1->setVisible(false);
    } else {
        m_Controls.button_2_1->setVisible(true);
    }
}


void FestiveView::ProcessVolumes() {
    
    bool flag = false;
    
    if (m_Controls.button_14->isVisible())
        flag = false;
    else
        flag = true;
    
    m_Controls.button_14->setVisible(flag);
    m_Controls.button_18->setVisible(flag);
    m_Controls.button_10->setVisible(flag);
    m_Controls.button_16->setVisible(flag);
    m_Controls.button_19->setVisible(flag);
    m_Controls.button_17->setVisible(flag);
    m_Controls.button_24->setVisible(flag);
    m_Controls.button_23->setVisible(flag);
    m_Controls.button_10->setVisible(flag);
    m_Controls.button_9->setVisible(flag);
    m_Controls.button_25->setVisible(flag);
    m_Controls.button_22->setVisible(flag);
    m_Controls.button_21->setVisible(flag);
    m_Controls.button_20->setVisible(flag);
    m_Controls.button_28->setVisible(flag);
    m_Controls.button_29->setVisible(flag);
    
    
}


void FestiveView::ConvertNII() {
    
    //Check for selection of images
    QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
    if (nodes.size() < 3) {
        QMessageBox::warning(NULL, "Attention",
                             "Please load and select at least 3 images from the Data Manager before starting this step!");
        return;
    }//_if
    
    //Ask the user for a dir to store data
    if (directory.isEmpty()) {
        directory = QFileDialog::getExistingDirectory(NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(),
                                                      QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
        if (directory.isEmpty() || directory.simplified().contains(" ")) {
            QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
            directory = QString();
            return;
        }//_if
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
        std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
        directory_processing = directory + dirSep + "Processing";
        
    }
    
    //Convert to Nifti
    int ctr = 0;
    QString path;
    this->BusyCursorOn();
    mitk::ProgressBar::GetInstance()->AddStepsToDo(nodes.size());
    foreach (mitk::DataNode::Pointer node, nodes) {
        mitk::BaseData::Pointer data = node->GetData();
        mitk::ProgressBar::GetInstance()->Progress();
        if (data) {
            //Test if this data item is an image
            mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data.GetPointer());
            if (image) {
                path = directory + mitk::IOUtil::GetDirectorySeparator() + "dcm-" + QString::number(ctr++) + ".nii";
                mitk::IOUtil::Save(image, path.toStdString());
                this->GetDataStorage()->Remove(node);
                mitk::IOUtil::Load(path.toStdString(), *this->GetDataStorage());
            } else
            return;
        } else
        return;
    }//_for
    
    nodes.clear();
    this->BusyCursorOff();
    mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(this->GetDataStorage());
}


void FestiveView::MaskIMGS() {
    
    //Toggle visibility of buttons
    if (m_Controls.button_3_1->isVisible()) {
        m_Controls.button_3_1->setVisible(false);
        return;
    } else {
        m_Controls.button_3_1->setVisible(true);
    }
    
    int reply = QMessageBox::question(NULL, "Question", "Do you have a mask to load?", QMessageBox::Yes, QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        
        QString path = QFileDialog::getOpenFileName(NULL, "Open the mask file", mitk::IOUtil::GetProgramPath().c_str(), QmitkIOUtil::GetFileOpenFilterString());
        if (path.isEmpty()) return;
        mitk::IOUtil::Load(path.toStdString(), *this->GetDataStorage());
        mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(this->GetDataStorage());
        
    } else {
        
        //Show the plugin
        this->GetSite()->GetPage()->ShowView("org.mitk.views.segmentation");
    }//_if
}



void FestiveView::LoadMeasurements() {
    
    this->GetSite()->GetPage()->ShowView("org.mitk.views.measurement");
    
    
}


void FestiveView::ExtractPackages() {
    
    this->BusyCursorOff();
    
    QString test_mirtk = main_mirtk_path + "reorient";
    
    if(QFileInfo::exists(test_mirtk)) {
        
        
        
        if (directory.isEmpty()) {
            
            directory = QFileDialog::getExistingDirectory(NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
            
            if (directory.isEmpty() || directory.simplified().contains(" ")) {
                QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
                directory = QString();
                return;
            }//_if
            
            char dirSep = mitk::IOUtil::GetDirectorySeparator();
            std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
            std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
            directory_processing = directory + dirSep + "Processing";
            
        }
        
        QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
        if (nodes.size() != 1) {
            QMessageBox::warning(NULL, "Attention", "Please select only one volume  from the Data Manager for processing!");
            return;
        }//_if
        
        
        bool nii_found = false;
        
        QString output_volume_0, output_volume_1, output_volume_2, output_volume_3;
        QString input_volume;
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        
        
        foreach (mitk::DataNode::Pointer node, nodes) {
            
            mitk::BaseData::Pointer data = node->GetData();
            
            if (data) {
                
                mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data.GetPointer());
                
                if (image) {
                    
                    input_volume = directory_processing + dirSep + node->GetName().c_str() + "-org.nii";
                    mitk::IOUtil::Save(image, input_volume.toStdString());
                    
                    output_volume_0 = directory_processing + dirSep + node->GetName().c_str() + "-org-p0.nii";
                    output_volume_1 = directory_processing + dirSep + node->GetName().c_str() + "-org-p1.nii";
                    output_volume_2 = directory_processing + dirSep + node->GetName().c_str() + "-org-p2.nii";
                    output_volume_3 = directory_processing + dirSep + node->GetName().c_str() + "-org-p3.nii";
                    
                    nii_found = true;
                    
                } else {
                    
                    this->BusyCursorOff();
                    QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you selected a .nii file!");
                    nodes.clear();
                    return;
                    
                }
                
                
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Selected node does not exists!");
                nodes.clear();
                return;
                
            }
            
        }
        
        
        if (nii_found) {
            
            this->BusyCursorOn();
            
            QString cmd_split_str = main_mirtk_path +  "extract-packages " + input_volume + " 4 ";
            
            std::system((cmd_split_str.toStdString()).c_str());
            
            for (int i=0; i<10; i++) {
                if(!QFileInfo::exists(output_volume_0)) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                } else {
                    break;
                }
            }
            
            this->BusyCursorOff();
            
            bool f0, f1, f2, f3;
            f0 = false;
            f1 = false;
            f2 = false;
            f3 = false;
            int f_count = 0;
            
            if(QFileInfo::exists(output_volume_0)) {
                f0 = true;
                f_count = f_count + 1;
            }
            
            if(QFileInfo::exists(output_volume_1)) {
                f1 = true;
                f_count = f_count + 1;
            }
            
            if(QFileInfo::exists(output_volume_2)) {
                f2 = true;
                f_count = f_count + 1;
            }
            
            if(QFileInfo::exists(output_volume_3)) {
                f3 = true;
                f_count = f_count + 1;
            }
            
            if (f_count > 1) {
                
                if (f0)
                mitk::IOUtil::Load(output_volume_0.toStdString(), *this->GetDataStorage());
                if (f1)
                mitk::IOUtil::Load(output_volume_1.toStdString(), *this->GetDataStorage());
                if (f2)
                mitk::IOUtil::Load(output_volume_2.toStdString(), *this->GetDataStorage());
                if (f3)
                mitk::IOUtil::Load(output_volume_3.toStdString(), *this->GetDataStorage());
                
                QMessageBox::information(NULL, "Attention", "Number of extracted packages: " + QString::number(f_count) + " ");
                
            } else {
                
                QMessageBox::information(NULL, "Attention", "Error! No packages extracted!");
            }
            
            
        } else {
            
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you selected an image!");
            nodes.clear();
            return;
        }
        
        nodes.clear();
        
        
    } else {
        
        this->BusyCursorOff();
        QMessageBox::warning(NULL, "Attention", "Error! Could not find MIRTK executables in " + main_mirtk_path + " !");
        return;
        
    }
    
    
}



/*

void FestiveView::ExtractDynamics() {
    
    
    // MOVE TO LOCAL
    
    //Check for selection of data
    QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
    if (nodes.empty()) {
        QMessageBox::warning(NULL, "Attention","Please select all your items from the Data Manager for into !");
        return;
    }//_if
    
    //Ask the user for a dir to store data
    if (directory.isEmpty()) {
        directory = QFileDialog::getExistingDirectory(
                                                      NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(),
                                                      QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
        if (directory.isEmpty() || directory.simplified().contains(" ")) {
            QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
            directory = QString();
            return;
        }//_if
    }
 
    char dirSep = mitk::IOUtil::GetDirectorySeparator();
    std::system(("rm -rf " + (directory + dirSep + "Dynamics").toStdString()).c_str());
    std::system(("mkdir "  + (directory + dirSep + "Dynamics").toStdString()).c_str());
    imgsList_preproc.clear();
    
    QString path;
    this->BusyCursorOn();
    mitk::ProgressBar::GetInstance()->AddStepsToDo(nodes.size());
    foreach (mitk::DataNode::Pointer node, nodes) {
        mitk::BaseData::Pointer data = node->GetData();
        mitk::ProgressBar::GetInstance()->Progress();
        if (data) {
            //Test if this data item is an image
            mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data.GetPointer());
            if (image) {
                // if (QString::fromStdString(node->GetName()).contains("Mask", Qt::CaseInsensitive)) node->SetName("Mask");
                imgsList_preproc << QString::fromStdString(node->GetName() + ".nii.gz");
                path = directory + dirSep + "Dynamics" + dirSep + node->GetName().c_str() + ".nii.gz";
                mitk::IOUtil::Save(image, path.toStdString());
            } else
            return;
        } else
        return;
    }//_for
    
    std::cout << directory.toStdString() << std::endl;
    
    //Transfer to server, split into dynamics and get back
    if (cmd) {
        mitk::ProgressBar::GetInstance()->AddStepsToDo(1);
        std::unique_ptr<CemrgCommandLine> scpCMD(new CemrgCommandLine());
        if (scpCMD->TransferTFServer(directory, "Dynamics", userID, server, false)) {
            
            
            bool out_flag = scpCMD->SplitStacksIntoDynamics(userID, server, imgsList_preproc);
            
            out_flag = scpCMD->DownloadDynamics(userID, server, directory);
            
            if(out_flag) {
                QMessageBox::information(NULL, "Attention", "Splitting stacks into dynamics was successful! Files are in /Dynamics/ folder.");
                mitk::ProgressBar::GetInstance()->Progress();
            } else {
                QMessageBox::critical(NULL, "Attention", "Splitting into dynamics was not successful!");
                mitk::ProgressBar::GetInstance()->Progress();
            }
            
            
        } else {
            QMessageBox::critical(NULL, "Attention", "Data transfer for splitting into dynamics was not successful!");
            mitk::ProgressBar::GetInstance()->Progress();
        }//_if
    } else {
        QMessageBox::warning(NULL, "Attention", "Server Connection has not been established!");
    }//_if_cmd
    
    //
    
    
    nodes.clear();
    this->BusyCursorOff();
    
    
}

*/


void FestiveView::ExtractDynamics() {
    
    this->BusyCursorOff();
    
    QString test_mirtk = main_mirtk_path + "reorient";
    
    if(QFileInfo::exists(test_mirtk)) {
        
        if (directory.isEmpty()) {
            directory = QFileDialog::getExistingDirectory(NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
            if (directory.isEmpty() || directory.simplified().contains(" ")) {
                QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
                directory = QString();
                return;
            }//_if
            
            char dirSep = mitk::IOUtil::GetDirectorySeparator();
            std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
            std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
            directory_processing = directory + dirSep + "Processing";
            
        }
        
        QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
        if (nodes.size() != 1) {
            QMessageBox::warning(NULL, "Attention", "Please select only one volume  from the Data Manager for processing!");
            return;
        }//_if
        
        
        bool nii_found = false;
        
        QString output_volume_0, output_volume_1, output_volume_2, output_volume_3;
        QString input_volume;
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        
        
        foreach (mitk::DataNode::Pointer node, nodes) {
            
            mitk::BaseData::Pointer data = node->GetData();
            
            if (data) {
                
                mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data.GetPointer());
                
                if (image) {
                    
                    input_volume = directory_processing + dirSep + node->GetName().c_str() + "-org.nii";
                    mitk::IOUtil::Save(image, input_volume.toStdString());
                    
                    output_volume_0 = directory_processing + dirSep + node->GetName().c_str() + "-org-d0.nii";
                    output_volume_1 = directory_processing + dirSep + node->GetName().c_str() + "-org-d1.nii";
                    output_volume_2 = directory_processing + dirSep + node->GetName().c_str() + "-org-d2.nii";
                    output_volume_3 = directory_processing + dirSep + node->GetName().c_str() + "-org-d3.nii";
                    
                    nii_found = true;
                    
                } else {
                    
                    this->BusyCursorOff();
                    QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you selected a .nii file!");
                    nodes.clear();
                    return;
                    
                }
                
                
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Selected node does not exists!");
                nodes.clear();
                return;
                
            }
            
        }
        
        
        if (nii_found) {
            
            this->BusyCursorOn();
            
            QString cmd_split_str = main_mirtk_path +  "split-single-stack " + input_volume + " ";
            
            std::system((cmd_split_str.toStdString()).c_str());
            
            for (int i=0; i<10; i++) {
                if(!QFileInfo::exists(output_volume_0)) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                } else {
                    break;
                }
            }
            
            this->BusyCursorOff();
            
            
            bool f0, f1, f2, f3;
            f0 = false;
            f1 = false;
            f2 = false;
            f3 = false;
            int f_count = 0;
            
            if(QFileInfo::exists(output_volume_0)) {
                f0 = true;
                f_count = f_count + 1;
            }
            
            if(QFileInfo::exists(output_volume_1)) {
                f1 = true;
                f_count = f_count + 1;
            }
            
            if(QFileInfo::exists(output_volume_2)) {
                f2 = true;
                f_count = f_count + 1;
            }
            
            if(QFileInfo::exists(output_volume_3)) {
                f3 = true;
                f_count = f_count + 1;
            }
            
            if (f_count > 1) {
                
                if (f0)
                    mitk::IOUtil::Load(output_volume_0.toStdString(), *this->GetDataStorage());
                if (f1)
                    mitk::IOUtil::Load(output_volume_1.toStdString(), *this->GetDataStorage());
                if (f2)
                    mitk::IOUtil::Load(output_volume_2.toStdString(), *this->GetDataStorage());
                if (f3)
                    mitk::IOUtil::Load(output_volume_3.toStdString(), *this->GetDataStorage());
                
                QMessageBox::information(NULL, "Attention", "Number of extracted dynamics: " + QString::number(f_count) + " ");
                
            } else {
            
                this->BusyCursorOff();
                QMessageBox::information(NULL, "Attention", "The input image is a 3D volume!");
            }
            
            
        } else {
            
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you selected an image!");
            nodes.clear();
            return;
        }
        
        nodes.clear();
        
        
    } else {
        
        this->BusyCursorOff();
        QMessageBox::warning(NULL, "Attention", "Error! Could not find MIRTK executables in " + main_mirtk_path + " !");
        return;
        
    }
    
}





void FestiveView::RegisterVolumes() {
    
    
    QString test_mirtk = main_mirtk_path + "reorient";
    
    
    if(QFileInfo::exists(test_mirtk)) {
        
        
        
        if (directory.isEmpty()) {
            directory = QFileDialog::getExistingDirectory(NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
            if (directory.isEmpty() || directory.simplified().contains(" ")) {
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
                directory = QString();
                return;
            }//_if
            
            char dirSep = mitk::IOUtil::GetDirectorySeparator();
            std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
            std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
            directory_processing = directory + dirSep + "Processing";
            
        }
        
        
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        
        
        
        QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
        if (nodes.size() != 2) {
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Please select two image volumes for registration!");
            return;
        }//_if
        
        
        QString image_1_name;
        QString image_2_name;
        
        QString beginning = "";
        QString short_image_1_name;
        QString short_image_2_name;
        
        bool found_1; 
        bool found_2; 
        int count = 0;
        
        found_1 = false;
        found_2 = false;
        
        
        foreach (mitk::DataNode::Pointer node, nodes) {
            
            count++;
            
            mitk::BaseData::Pointer data = node->GetData();
            
            if (data) {
                
                //Test if this data item is an image
                mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data.GetPointer());
                
                if (image) {
                    
                    if(count == 1) {
                        
                        short_image_1_name = beginning + node->GetName().c_str();
                        image_1_name = directory_processing + dirSep + node->GetName().c_str() + "-org.nii";
                        mitk::IOUtil::Save(image, image_1_name.toStdString());
                        found_1 = true;
                        
                    } else {
                        
                        short_image_2_name = beginning + node->GetName().c_str();
                        image_2_name = directory_processing + dirSep + node->GetName().c_str() + "-org.nii";
                        mitk::IOUtil::Save(image, image_2_name.toStdString());
                        found_2 = true;
                    }
                    
                    
                } else {
                    
                    this->BusyCursorOff();
                    QMessageBox::warning(NULL, "Attention", "Error! Selected node is not an image'!");
                    nodes.clear();
                    return;
                    
                }
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Selected node does not exists!");
                nodes.clear();
                return;
            }
            
        }//_for

        if (!found_1 || !found_2) {
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Error! Please select two images!");
            nodes.clear();
            return;
        }
        
        
        //Prompt for parameters
        QDialog* inputs = new QDialog(0,0);
        m_Register.setupUi(inputs);
        
        m_Register.listWidget->addItem(short_image_1_name);
        m_Register.listWidget->addItem(short_image_2_name);
        
        m_Register.listWidget_2->addItem(short_image_1_name);
        m_Register.listWidget_2->addItem(short_image_2_name);
        
        
        //        m_Register.listWidget->setCurrentRow(0);
        //        m_Register.listWidget_2->setCurrentRow(1);
        
        
        connect(m_Register.buttonBox, SIGNAL(accepted()), inputs, SLOT(accept()));
        connect(m_Register.buttonBox, SIGNAL(rejected()), inputs, SLOT(reject()));
        int dialogCode = inputs->exec();
        
        //Act on dialog return code
        if (dialogCode == QDialog::Accepted) {
            
            this->BusyCursorOn();
            
            
            if (!(m_Register.listWidget->currentItem()) || !(m_Register.listWidget_2->currentItem())) {
                
                QMessageBox::warning(NULL, "Attention", "Error! Please select target and source images!");
                nodes.clear();
                this->BusyCursorOff();
                return;
            }
            
            
            QString target_name = m_Register.listWidget->currentItem()->text();
            QString source_name = m_Register.listWidget_2->currentItem()->text();
            
            
            
            QString output_name = m_Register.lineEdit_2->text();
            
            QString default_output_name = source_name + "-registered.nii.gz" ;
            
//            if (output_name.isEmpty() || !output_name.endsWith(".nii")) output_name = "Registered.nii";
            
            if (output_name.isEmpty()) output_name = default_output_name;
            
            
            
            bool ok_bg, ok_cp;
            
            double bg_value = m_Register.lineEdit_3->text().toDouble(&ok_bg);
            int cp_spacing = m_Register.lineEdit_4->text().toInt(&ok_cp);
            
            bool flag_rigid = m_Register.checkBox_1->checkState();
            bool flag_affine = m_Register.checkBox_2->checkState();
            bool flag_ffd = m_Register.checkBox_3->checkState();
            
            
            
            // target and source names
            QString reg_names = "";
            
            
            // registration model
            QString reg_model = " -model ";
            //        if (flag_rigid) {
            //            reg_model = reg_model + "Rigid";      // run rigid separately
            //        }
            if (flag_affine) {
                //            if (flag_rigid)
                //                reg_model = reg_model + "+";
                reg_model = reg_model + "Affine";
            }
            if (flag_ffd) {
                if (flag_affine)
                reg_model = reg_model + "+";
                reg_model = reg_model + "FFD ";
            }
            
            if (!(flag_rigid || flag_affine || flag_ffd)) {
                
                QMessageBox::warning(NULL, "Attention", "Error! Please select registration type!");
                nodes.clear();
                this->BusyCursorOff();
                return;
            }
            
            // bg option
            QString reg_bg = "";
            if (ok_bg) {
                reg_bg = " -bg " + QString::number(bg_value) + " ";
                
            }
            
            // cp option
            QString reg_cp = "";
            if (ok_cp) {
                reg_cp = " -ds " + QString::number(cp_spacing) + " ";
                
            }
            
            QString registered_name = directory_processing + dirSep + output_name;
            QString reg_output = " -output " + registered_name + " ";
            
            
            
            // --------------------------------------------------------------------------------
            
            if (flag_rigid) {
                
                // init rigid registration
                
                // 1. reset target image
                QString org_target_name = directory_processing + dirSep + target_name + "-org.nii ";
                QString res_target_name = directory_processing + dirSep + target_name + "-res.nii ";
                QString res_dofin_name = directory_processing + dirSep + "dofin-res.dof ";
                QString res_offset_name = directory_processing + dirSep + "offset-res.dof ";
                
                // mirtk reset-image-dof target.nii.gz reset-target.nii.gz reset-dof.dof reset-offset.dof
                
                QString init_res_cmd_str = main_mirtk_path + "reset-image-dof " + org_target_name + res_target_name + res_dofin_name + res_offset_name + " \n";
                std::system((init_res_cmd_str.toStdString()).c_str());
                
                // 2. run rigid registration
                QString reg_init_names = " ";
                reg_init_names = reg_init_names + directory_processing + dirSep + target_name + "-res.nii ";
                reg_init_names = reg_init_names + directory_processing + dirSep + source_name + "-org.nii ";
                
                QString reg_init_model = " -model Rigid ";
                QString reg_init_dofin = " -dofin " + directory_processing + dirSep + "dofin-res.dof ";
                QString reg_init_dofout = " -dofout " + directory_processing + dirSep + "dofout-res.dof ";
                
                // mirtk register reset-target.nii.gz source.nii.gz -model Rigid -bg 0 -dofout org-res.dof -dofin reset-dof.dof
                
                QString init_reg_cmd_str = main_mirtk_path + "register " + reg_init_names + reg_init_model + reg_bg + reg_init_dofin + reg_init_dofout + " \n";
                std::system((init_reg_cmd_str.toStdString()).c_str());
                
                // 3. update output dof
                
                QString init_dofout_name = directory_processing + dirSep + "dofout-res.dof ";
                QString final_dofin_name = directory_processing + dirSep + "dofin-final.dof ";
                
                // mirtk reset-dof-back org-res.dof reset-offset.dof org-res-back.dof
                
                QString init_back_cmd_str = main_mirtk_path + "reset-dof-back " + init_dofout_name + res_offset_name + final_dofin_name + " \n";
                std::system((init_back_cmd_str.toStdString()).c_str());
                
                // 4. update source volume
                
                QString org_source_name = directory_processing + dirSep + source_name + "-org.nii ";
                QString transf_source_name = directory_processing + dirSep + source_name + "-transf.nii ";
                
                //mirtk edit-image source.nii.gz source-sh.nii.gz -dofin_i org-res-back.dof
                
                QString init_vol_cmd_str = main_mirtk_path + "edit-image " + org_source_name + transf_source_name + " -dofin_i " + final_dofin_name + " \n";
                std::system((init_vol_cmd_str.toStdString()).c_str());
                
                
                // with update the source name
                reg_names = "";
                reg_names = reg_names + directory_processing + dirSep + target_name + "-org.nii ";
                reg_names = reg_names + directory_processing + dirSep + source_name + "-transf.nii ";
                
            } else {
                
                reg_names = "";
                reg_names = reg_names + directory_processing + dirSep + target_name + "-org.nii ";
                reg_names = reg_names + directory_processing + dirSep + source_name + "-org.nii ";
            }
            
            
            // --------------------------------------------------------------------------------
            // main registration
            
            if (flag_affine || flag_ffd) {
                
                QString reg_cmd_str = main_mirtk_path + "register " + reg_names + reg_model + reg_bg + reg_cp + reg_output + " \n";
                
                std::system((reg_cmd_str.toStdString()).c_str());
                
            } else {
                
                QString transf_source_name = directory_processing + dirSep + source_name + "-transf.nii ";
                
                QString cp_cmd_str = "cp " + transf_source_name + " " + registered_name + " \n";
                std::system((cp_cmd_str.toStdString()).c_str());
                
            }
            
            
            
            // --------------------------------------------------------------------------------
            
            this->BusyCursorOff();
            
            std::cout << "--------------------------------------------------------------------------------" << std::endl;
            std::cout << registered_name.toStdString() << std::endl;
            std::cout << "--------------------------------------------------------------------------------" << std::endl;
            
            
            if(QFileInfo::exists(registered_name)) {
                
                mitk::IOUtil::Load(registered_name.toStdString(), *this->GetDataStorage());
                QMessageBox::information(NULL, "Attention", "Registration was successful! Output file: " + output_name + " ");
                
            } else {
                
                QMessageBox::warning(NULL, "Attention", "Error! Registration was not successful!");
                nodes.clear();
                return;
            }
            
            
            nodes.clear();
            inputs->deleteLater();
            
            
        } else if(dialogCode == QDialog::Rejected) {
            inputs->close();
            inputs->deleteLater();
        }//_if
        
        
    } else {
        
        this->BusyCursorOff();
        QMessageBox::warning(NULL, "Attention", "Error! Could not find MIRTK executables in " + main_mirtk_path + " !");
        return;
        
    }
    
    
}





void FestiveView::PropagateLabels() {
    

    
    
    QString test_mirtk = main_mirtk_path + "reorient";
    
    
    if(QFileInfo::exists(test_mirtk)) {
        
        
        
        if (directory.isEmpty()) {
            directory = QFileDialog::getExistingDirectory(NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
            if (directory.isEmpty() || directory.simplified().contains(" ")) {
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
                directory = QString();
                return;
            }//_if
            
            char dirSep = mitk::IOUtil::GetDirectorySeparator();
            std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
            std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
            directory_processing = directory + dirSep + "Processing";
            
        }
        
        
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        
        
        
        QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
        if (nodes.size() != 1) {
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Please select one image volume for label propagation!");
            return;
        }//_if
        
        
        
        
        QString input_image_name;
        
        QString beginning = "";
        QString short_input_image_name;

        

        foreach (mitk::DataNode::Pointer node, nodes) {
            
            mitk::BaseData::Pointer data = node->GetData();
            
            if (data) {
                
                mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data.GetPointer());
                
                if (image) {
                    
                    short_input_image_name = beginning + node->GetName().c_str();
                    input_image_name = directory_processing + dirSep + node->GetName().c_str() + "-org.nii";
                    mitk::IOUtil::Save(image, input_image_name.toStdString());
                    
                } else {
                    
                    this->BusyCursorOff();
                    QMessageBox::warning(NULL, "Attention", "Error! Selected node is not an image'!");
                    nodes.clear();
                    return;
                    
                }
                

                
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Selected node does not exists!");
                nodes.clear();
                return;
            }
            
            
        }
        
        
        //Prompt for parameters
        QDialog* inputs = new QDialog(0,0);
        m_LabelPropagation.setupUi(inputs);
        
        m_LabelPropagation.checkBox_5->setChecked( true );
        
        connect(m_LabelPropagation.buttonBox, SIGNAL(accepted()), inputs, SLOT(accept()));
        connect(m_LabelPropagation.buttonBox, SIGNAL(rejected()), inputs, SLOT(reject()));
        int dialogCode = inputs->exec();
        
        //Act on dialog return code
        if (dialogCode == QDialog::Accepted) {
            
            this->BusyCursorOn();
            
            
            
            // intput volume name
            
//            QString short_output_name = m_Register.lineEdit_2->text();
//
//            QString default_output_name = short_input_image_name + "-label.nii.gz" ;
//
//            if (short_output_name.isEmpty()) short_output_name = default_output_name;
//
//            QString output_image_name;
//            output_image_name = directory_processing + dirSep + short_output_name;
            
            
            
            // label selection / loading
            
            bool ok_ga; //, ok_cnf;
            double ga_value = m_LabelPropagation.lineEdit->text().toDouble(&ok_ga);
            
            if (!ok_ga) {

                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! The GA was not specified!");
                nodes.clear();
                return;
                
            }
//
//            double cnf_value = m_LabelPropagation.lineEdit_4->text().toDouble(&ok_cnf);
//
//
//            bool flag_trunk = m_LabelPropagation.checkBox->checkState();
//
//            bool flag_lung_l = m_LabelPropagation.checkBox_2->checkState();
//            bool flag_lung_r = m_LabelPropagation.checkBox_2->checkState();
//
//            bool flag_liver = m_LabelPropagation.checkBox_5->checkState();
//
//            bool flag_kidney_l = m_LabelPropagation.checkBox_6->checkState();
//            bool flag_kidney_r = m_LabelPropagation.checkBox_4->checkState();
//
//
//            bool flag_heart = m_LabelPropagation.checkBox_7->checkState();
//            bool flag_thymus = m_LabelPropagation.checkBox_1->checkState();

            
            
            // atlas seleciton / loading
            
//            std::cout << "0 --------------------------------------------------------------" << std::endl;
            
            int ga = round(ga_value);
            QString atlas_name = "body-atlas-" + QString::number(ga);
            QString global_atlas_name = body_atlas_path + atlas_name + ".nii.gz";

            if(QFileInfo::exists(global_atlas_name)) {
                
//                mitk::IOUtil::Load(global_atlas_name.toStdString(), *this->GetDataStorage());
//                QMessageBox::information(NULL, "Attention", "Loaded file: " + atlas_name + " ");
                
                
                QString cp_atlas_cmd_str = "cp " + global_atlas_name + " "  + directory_processing + " \n";
                std::system((cp_atlas_cmd_str.toStdString()).c_str());
                
                
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Could not load file: " + atlas_name + " !");
                return;
            }
            
            
//
//            std::cout << "1 --------------------------------------------------------------" << std::endl;
            
            
            
            QString atlas_masked_name = "body-atlas-masked-" + QString::number(ga);
            QString global_atlas_masked_name = body_atlas_path + atlas_masked_name + ".nii.gz";
            
            if(QFileInfo::exists(global_atlas_masked_name)) {
                
//                mitk::IOUtil::Load(global_atlas_masked_name.toStdString(), *this->GetDataStorage());
//                QMessageBox::information(NULL, "Attention", "Loaded file: " + atlas_masked_name + " ");
                
                QString cp_atlas_cmd_str = "cp " + global_atlas_masked_name + " "  + directory_processing + " \n";
                std::system((cp_atlas_cmd_str.toStdString()).c_str());

                
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Could not load file: " + atlas_masked_name + " !");
                return;
            }
            
            

            QString atlas_label_name = "body-atlas-liver-" + QString::number(ga);
            QString global_atlas_label_name = body_atlas_path + atlas_label_name + ".nii.gz";
            
            if(QFileInfo::exists(global_atlas_label_name)) {
                
//                mitk::IOUtil::Load(global_atlas_label_name.toStdString(), *this->GetDataStorage());
//                QMessageBox::information(NULL, "Attention", "Loaded file: " + atlas_label_name + " ");
                
                QString cp_atlas_label_cmd_str = "cp " + global_atlas_label_name + " "  + directory_processing + " \n";
                std::system((cp_atlas_label_cmd_str.toStdString()).c_str());
                
                
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Could not load file: " + atlas_label_name + " !");
                return;
            }

            
            
            // volume - atlas registration
            
            
            
            
//            QString global_atlas_name = body_atlas_path + atlas_name;
//            QString global_atlas_masked_name = body_atlas_path + atlas_masked_name;
//            QString global_atlas_label_name = body_atlas_path + atlas_label_name;
            
            
            
            
            
            QString reg_names = "";
            QString reg_bg = " -bg 0 ";

            
            // 1. rigid+affine alignement
            
            {
                
                QString target_name = short_input_image_name;
                QString source_name = atlas_masked_name;
                
                
                // 1. reset target image
                
                
//                QString org_target_name = directory_processing + dirSep + target_name + "-org.nii ";
//                QString res_target_name = directory_processing + dirSep + target_name + "-res.nii ";
                
                
                QString org_target_name = directory_processing + dirSep + target_name + "-org.nii ";
                QString res_target_name = directory_processing + dirSep + target_name + "-res.nii ";
                
                
                
                
                QString res_dofin_name = directory_processing + dirSep + "dofin-res.dof ";
                QString res_offset_name = directory_processing + dirSep + "offset-res.dof ";
                
                // mirtk reset-image-dof target.nii.gz reset-target.nii.gz reset-dof.dof reset-offset.dof
                
                QString init_res_cmd_str = main_mirtk_path + "reset-image-dof " + org_target_name + res_target_name + res_dofin_name + res_offset_name + " \n";
                std::system((init_res_cmd_str.toStdString()).c_str());
                

                
                // 2. run rigid registration
                
                QString reg_init_names = " ";
                reg_init_names = reg_init_names + directory_processing + dirSep + target_name + "-res.nii ";
                reg_init_names = reg_init_names + directory_processing + dirSep + source_name + ".nii.gz ";
                
                
                
                QString reg_init_model = " -model Rigid ";
                QString reg_init_dofin = " -dofin " + directory_processing + dirSep + "dofin-res.dof ";
                QString reg_init_dofout = " -dofout " + directory_processing + dirSep + "dofout-res.dof ";
            
                
                // mirtk register reset-target.nii.gz source.nii.gz -model Rigid -bg 0 -dofout org-res.dof -dofin reset-dof.dof
                
                QString init_reg_cmd_str = main_mirtk_path + "register " + reg_init_names + reg_init_model + reg_bg + reg_init_dofin + reg_init_dofout + " \n";
                std::system((init_reg_cmd_str.toStdString()).c_str());
                
                
                
                // 3. update output dof
                
                QString init_dofout_name = directory_processing + dirSep + "dofout-res.dof ";
                QString final_dofin_name = directory_processing + dirSep + "dofin-final.dof ";
                
                // mirtk reset-dof-back org-res.dof reset-offset.dof org-res-back.dof
                
                QString init_back_cmd_str = main_mirtk_path + "reset-dof-back " + init_dofout_name + res_offset_name + final_dofin_name + " \n";
                std::system((init_back_cmd_str.toStdString()).c_str());
                
                
                // 4. update source volume
                
                
                source_name = atlas_name;
                
                QString org_source_name = directory_processing + dirSep + source_name + ".nii.gz ";
                QString transf_source_name = directory_processing + dirSep + source_name + "-transf.nii ";
                
                //mirtk edit-image source.nii.gz source-sh.nii.gz -dofin_i org-res-back.dof
                
                QString init_vol_cmd_str = main_mirtk_path + "edit-image " + org_source_name + transf_source_name + " -dofin_i " + final_dofin_name + " \n";
                std::system((init_vol_cmd_str.toStdString()).c_str());
                
                
                QString org_source_label_name = directory_processing + dirSep + atlas_label_name + ".nii.gz ";
                QString transf_source_label_name = directory_processing + dirSep + atlas_label_name + "-transf.nii ";
                
                QString init_vol_label_cmd_str = main_mirtk_path + "edit-image " + org_source_label_name + transf_source_label_name + " -dofin_i " + final_dofin_name + " \n";
                std::system((init_vol_label_cmd_str.toStdString()).c_str());
                
                
                
                
                // with updated source name
                
                QString in1_name = directory_processing + dirSep + target_name + "-org.nii ";
                QString in2_name = directory_processing + dirSep + source_name + "-transf.nii ";
                
                
                QString in1_res_name = directory_processing + dirSep + target_name + "-org-res.nii ";
                QString in2_res_name = directory_processing + dirSep + source_name + "-transf-res.nii ";
                
                
                QString res1_reg_cmd_str = main_mirtk_path + "resample-image " + in1_name + in1_res_name + " -size 1 1 1 " + " \n";
                std::system((res1_reg_cmd_str.toStdString()).c_str());
                
                QString res2_reg_cmd_str = main_mirtk_path + "resample-image " + in2_name + in2_res_name + " -size 1 1 1 " + " \n";
                std::system((res2_reg_cmd_str.toStdString()).c_str());
                
                
//                reg_names = "";
//                reg_names = reg_names + directory_processing + dirSep + target_name + "-org.nii ";
//                reg_names = reg_names + directory_processing + dirSep + source_name + "-transf.nii ";
                
                
                reg_names = " ";
                reg_names = reg_names + in1_res_name + in2_res_name;
                
                std::cout << reg_names.toStdString() << std::endl;
                
            }
            

            
            // 2. ffd registration
            
            
            std::cout << "--------------------------------------------------------------" << std::endl;
            
            
            
            QString registered_name = directory_processing + dirSep + atlas_name + "-registered.nii.gz" ;
            
            QString registered_label_name = directory_processing + dirSep + short_input_image_name + "-label.nii.gz" ;
            
            QString registered_dof = directory_processing + dirSep + "transf-label.dof" ;
            
            reg_bg = " -bg 0 ";
            QString reg_model = " -model Affine+FFD ";
            QString reg_cp = " ";           //  -ds 5
//            QString reg_output = " -output " + registered_name + " ";
            QString reg_output_dof = " -dofout " + registered_dof + " ";
            
            
//            QString reg_cmd_str = main_mirtk_path + "register " + reg_names + reg_model + reg_bg + reg_cp + reg_output + reg_output_dof + " \n";
            
            QString reg_cmd_str = main_mirtk_path + "register " + reg_names + reg_model + reg_bg + reg_cp + reg_output_dof + " \n";
            std::system((reg_cmd_str.toStdString()).c_str());
            
            
            QString transf_label_name = directory_processing + dirSep + atlas_label_name + "-transf.nii ";
            
            QString trl_cmd_str = main_mirtk_path + "transform-image " + transf_label_name + " " + registered_label_name + " -dofin " + registered_dof + " -interp NN \n";
            std::system((trl_cmd_str.toStdString()).c_str());
            
            
            
//            QString atlas_label_name = "body-atlas-liver-" + QString::number(ga);
//            QString global_atlas_label_name = body_atlas_path + atlas_label_name + ".nii.gz";
            
            
            
            this->BusyCursorOff();
            
            
            if(QFileInfo::exists(registered_label_name)) {
                
                
                QString stats_name = directory_processing + dirSep + "stats-" + short_input_image_name + ".txt";
                
                
                QString stats_cmd_str = main_mirtk_path + "mask_stats " + registered_label_name + " 1 > " + stats_name + " \n";
                std::system((stats_cmd_str.toStdString()).c_str());
                
                
                QFile file(stats_name);
                if(!file.open(QIODevice::ReadOnly)) {
                    QMessageBox::information(0, "error", file.errorString());
                }
                
                QTextStream in(&file);
                QString line = in.readLine();
                file.close();
                
        
                std::cout << " Mask volume : " << line.toStdString() << std::endl;

                mitk::IOUtil::Load(registered_label_name.toStdString(), *this->GetDataStorage());
                QMessageBox::information(NULL, "Attention", "Mask volume: " + line + " cc ");
                
                
//                QMessageBox::information(NULL, "Attention", "Label propagation was successful! Output file: " + short_input_image_name + "-label.nii.gz ");
//
                
            } else {
                
                QMessageBox::warning(NULL, "Attention", "Error! Label propagation was not successful!");
                nodes.clear();
                return;
            }
            
            
        
            
            nodes.clear();
            inputs->deleteLater();
            
            
        } else if(dialogCode == QDialog::Rejected) {
            inputs->close();
            inputs->deleteLater();
        }//_if

        
        
    } else {
        
        this->BusyCursorOff();
        QMessageBox::warning(NULL, "Attention", "Error! Could not find MIRTK executables in " + main_mirtk_path + " !");
        return;
        
    }
    
    
}






void FestiveView::LabelVolume() {
    
    
    
    
    QString test_mirtk = main_mirtk_path + "reorient";
    
    
    if(QFileInfo::exists(test_mirtk)) {
        
        
        
        if (directory.isEmpty()) {
            directory = QFileDialog::getExistingDirectory(NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
            if (directory.isEmpty() || directory.simplified().contains(" ")) {
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
                directory = QString();
                return;
            }//_if
            
            char dirSep = mitk::IOUtil::GetDirectorySeparator();
            std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
            std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
            directory_processing = directory + dirSep + "Processing";
            
        }
        
        
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        
        
        
        QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
        if (nodes.size() != 1) {
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Please select one image volume for label volume calculation!");
            return;
        }
        
        
    
        QString input_image_name;
        
        QString beginning = "";
        QString short_input_image_name;
        
        
        
        foreach (mitk::DataNode::Pointer node, nodes) {
            
            mitk::BaseData::Pointer data = node->GetData();
            
            if (data) {
                
                mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data.GetPointer());
                
                if (image) {
                    
                    short_input_image_name = beginning + node->GetName().c_str();
                    input_image_name = directory_processing + dirSep + node->GetName().c_str() + "-org.nii";
                    mitk::IOUtil::Save(image, input_image_name.toStdString());
                    
                } else {
                    
                    this->BusyCursorOff();
                    QMessageBox::warning(NULL, "Attention", "Error! Selected node is not an image'!");
                    nodes.clear();
                    return;
                    
                }
                
                
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Selected node does not exists!");
                nodes.clear();
                return;
            }
            
            
        }
        
        
        QString stats_name = directory_processing + dirSep + "stats-" + short_input_image_name + ".txt";
        
        
        QString stats_cmd_str = main_mirtk_path + "mask_stats " + input_image_name + " 1 > " + stats_name + " \n";
        std::system((stats_cmd_str.toStdString()).c_str());
        
        
        QFile file(stats_name);
        if(!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(0, "error", file.errorString());
        }
        
        QTextStream in(&file);
        QString line = in.readLine();
        file.close();
        
        
        std::cout << " Mask volume : " << line.toStdString() << std::endl;
        
        QMessageBox::information(NULL, "Attention", "Mask volume: " + line + " cc ");
        
        
        return;
        
    } else {
        
        this->BusyCursorOff();
        QMessageBox::warning(NULL, "Attention", "Error! Could not find MIRTK executables in " + main_mirtk_path + " !");
        return;
        
    }

    
}






void FestiveView::MaskVolume() {
    
    this->BusyCursorOff();
    
    QString test_mirtk = main_mirtk_path + "reorient";
    
    if(QFileInfo::exists(test_mirtk)) {
    
        if (directory.isEmpty()) {
            directory = QFileDialog::getExistingDirectory(NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
            if (directory.isEmpty() || directory.simplified().contains(" ")) {
                
                QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
                directory = QString();
                return;
            }//_if
            
            char dirSep = mitk::IOUtil::GetDirectorySeparator();
            std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
            std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
            directory_processing = directory + dirSep + "Processing";
            
        }
        
        QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
        if (nodes.size() != 2) {
            QMessageBox::warning(NULL, "Attention", "Please select a volume and a mask from the Data Manager for processing!");
            return;
        }//_if
    
        bool nii_found = false;
        bool mask_found = false;
        
        QString masked_volume;
        QString input_volume;
        QString input_mask;
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        
        
        foreach (mitk::DataNode::Pointer node, nodes) {
            
            mitk::BaseData::Pointer data = node->GetData();
            
            if (data) {
                
                mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data.GetPointer());
                
                if (image) {
                    
                    if (QString::fromStdString(node->GetName()).contains("mask", Qt::CaseInsensitive)) {
                        
                        input_mask = directory_processing + dirSep + node->GetName().c_str() + "-mask.nii";
                        mitk::IOUtil::Save(image, input_mask.toStdString());
                        
                        mask_found = true;
                        
                    } else {
                        
                        input_volume = directory_processing + dirSep + node->GetName().c_str() + "-org.nii";
                        mitk::IOUtil::Save(image, input_volume.toStdString());
                        
                        masked_volume = directory_processing + dirSep + node->GetName().c_str() + "-msked.nii";
                        
                        nii_found = true;
                        
                    }
                    
                } else {
                    
                    this->BusyCursorOff();
                    QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you selected .nii files!");
                    nodes.clear();
                    return;
                    
                }
                
                
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Selected node does not exists!");
                nodes.clear();
                return;
                
            }
  
        }
        
        
        if (nii_found && mask_found) {
            
            this->BusyCursorOn();
            
            QString cmd_mask_str = main_mirtk_path +  "mask_volume " + input_volume + " " + input_mask + " " + masked_volume + "";
            
            std::system((cmd_mask_str.toStdString()).c_str());
            
            for (int i=0; i<10; i++) {
                if(!QFileInfo::exists(masked_volume)) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                } else {
                    break;
                }
            }
            
            this->BusyCursorOff();
            
            if(QFileInfo::exists(masked_volume)) {
                
                mitk::IOUtil::Load(masked_volume.toStdString(), *this->GetDataStorage());
                QMessageBox::information(NULL, "Attention", "Volume masking was successful!");
                
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Volume masking was not successful!");
                nodes.clear();
                return;
            }
            
        } else {
            
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you selected an image volume and a mask volume that contains 'mask' in the name!");
            nodes.clear();
            return;
        }
        
        nodes.clear();
        
        
    } else {
        
        this->BusyCursorOff();
        QMessageBox::warning(NULL, "Attention", "Error! Could not find MIRTK executables in " + main_mirtk_path + " !");
        return;
        
    }
    
}





void FestiveView::ResetOrigin() {
    
    this->BusyCursorOff();
    
    QString test_mirtk = main_mirtk_path + "reorient";
    
    if(QFileInfo::exists(test_mirtk)) {
        
        if (directory.isEmpty()) {
            directory = QFileDialog::getExistingDirectory(NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
            if (directory.isEmpty() || directory.simplified().contains(" ")) {
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
                directory = QString();
                return;
            }//_if
            
            char dirSep = mitk::IOUtil::GetDirectorySeparator();
            std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
            std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
            directory_processing = directory + dirSep + "Processing";
            
        }
        
        QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
        if (nodes.size() != 1) {
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Please select only one volume  from the Data Manager for processing!");
            return;
        }//_if
        
        
        bool nii_found = false;
        
        QString reset_volume;
        QString input_volume;
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        
        
        foreach (mitk::DataNode::Pointer node, nodes) {
            
            mitk::BaseData::Pointer data = node->GetData();
            
            if (data) {
                
                mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data.GetPointer());
                
                if (image) {
                    
                    input_volume = directory_processing + dirSep + node->GetName().c_str() + "-org.nii";
                    mitk::IOUtil::Save(image, input_volume.toStdString());
                    
                    reset_volume = directory_processing + dirSep + node->GetName().c_str() + "-reset.nii";
                    
                    nii_found = true;
                    
                } else {
                    
                    this->BusyCursorOff();
                    QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you selected a .nii file!");
                    nodes.clear();
                    return;
                    
                }
                
                
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Selected node does not exists!");
                nodes.clear();
                return;
                
            }
            
        }
        
        
        if (nii_found) {
            
            this->BusyCursorOn();
            
            QString cmd_reset_str = main_mirtk_path +  "edit-image " + input_volume + " " + reset_volume + " -origin 0 0 0 ";
            
            std::system((cmd_reset_str.toStdString()).c_str());
            
            for (int i=0; i<10; i++) {
                if(!QFileInfo::exists(reset_volume)) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                } else {
                    break;
                }
            }
            
            this->BusyCursorOff();
            
            if(QFileInfo::exists(reset_volume)) {
                
                mitk::IOUtil::Load(reset_volume.toStdString(), *this->GetDataStorage());
                QMessageBox::information(NULL, "Attention", "Resetting origin was successful!");
                
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Resetting origin was not successful!");
                nodes.clear();
                return;
            }
            
        } else {
            
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you selected an image!");
            nodes.clear();
            return;
        }
        
        nodes.clear();
        
        
    } else {
        
        this->BusyCursorOff();
        QMessageBox::warning(NULL, "Attention", "Error! Could not find MIRTK executables in " + main_mirtk_path + " !");
        return;
        
    }
    
}






void FestiveView::ThresholdMask() {
    
    this->BusyCursorOff();
    
    QString test_mirtk = main_mirtk_path + "reorient";
    
    if(QFileInfo::exists(test_mirtk)) {
        
        if (directory.isEmpty()) {
            directory = QFileDialog::getExistingDirectory(NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
            if (directory.isEmpty() || directory.simplified().contains(" ")) {
                QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
                directory = QString();
                return;
            }//_if
            
            char dirSep = mitk::IOUtil::GetDirectorySeparator();
            std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
            std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
            directory_processing = directory + dirSep + "Processing";
            
        }
        
        QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
        if (nodes.size() != 1) {
            QMessageBox::warning(NULL, "Attention", "Please select only one volume  from the Data Manager for processing!");
            return;
        }//_if
        
        
        bool nii_found = false;
        
        QString output_volume;
        QString input_volume;
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        
        
        foreach (mitk::DataNode::Pointer node, nodes) {
            
            mitk::BaseData::Pointer data = node->GetData();
            
            if (data) {
                
                mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data.GetPointer());
                
                if (image) {
                    
                    input_volume = directory_processing + dirSep + node->GetName().c_str() + "-org.nii";
                    mitk::IOUtil::Save(image, input_volume.toStdString());
                    
                    output_volume = directory_processing + dirSep + node->GetName().c_str() + "-mask.nii";
                    
                    nii_found = true;
                    
                } else {
                    
                    this->BusyCursorOff();
                    QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you selected a .nii file!");
                    nodes.clear();
                    return;
                    
                }
                
                
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Selected node does not exists!");
                nodes.clear();
                return;
                
            }
            
        }
        
        
        if (nii_found) {
            
            this->BusyCursorOn();
            
            QString cmd_main_str = main_mirtk_path +  "create_mask " + input_volume + " " + output_volume + " ";
            
            std::system((cmd_main_str.toStdString()).c_str());
            
            for (int i=0; i<10; i++) {
                if(!QFileInfo::exists(output_volume)) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                } else {
                    break;
                }
            }
            
            this->BusyCursorOff();
            
            if(QFileInfo::exists(output_volume)) {
                
                mitk::IOUtil::Load(output_volume.toStdString(), *this->GetDataStorage());
                QMessageBox::information(NULL, "Attention", "Thresholding was successful!");
                
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Thresholding was not successful!");
                nodes.clear();
                return;
            }
            
        } else {
            
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you selected an image!");
            nodes.clear();
            return;
        }
        
        nodes.clear();
        
        
    } else {
        
        this->BusyCursorOff();
        QMessageBox::warning(NULL, "Attention", "Error! Could not find MIRTK executables in " + main_mirtk_path + " !");
        return;
        
    }
    
}






void FestiveView::CropVolume() {
    
    this->BusyCursorOff();
    
    QString test_mirtk = main_mirtk_path + "reorient";
    
    if(QFileInfo::exists(test_mirtk)) {
        
        if (directory.isEmpty()) {
            directory = QFileDialog::getExistingDirectory(NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
            if (directory.isEmpty() || directory.simplified().contains(" ")) {
                QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
                directory = QString();
                return;
            }//_if
            
            char dirSep = mitk::IOUtil::GetDirectorySeparator();
            std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
            std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
            directory_processing = directory + dirSep + "Processing";
            
        }
        
        QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
        if (nodes.size() != 2) {
            QMessageBox::warning(NULL, "Attention", "Please select a volume and a mask from the Data Manager for processing!");
            return;
        }//_if
        
        bool nii_found = false;
        bool mask_found = false;
        
        QString cropped_volume;
        QString input_volume;
        QString input_mask;
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        
        
        foreach (mitk::DataNode::Pointer node, nodes) {
            
            mitk::BaseData::Pointer data = node->GetData();
            
            if (data) {
                
                mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data.GetPointer());
                
                if (image) {
                    
                    if (QString::fromStdString(node->GetName()).contains("mask", Qt::CaseInsensitive)) {
                        
                        input_mask = directory_processing + dirSep + node->GetName().c_str() + "-mask.nii";
                        mitk::IOUtil::Save(image, input_mask.toStdString());

                        mask_found = true;
                        
                    } else {
                        
                        input_volume = directory_processing + dirSep + node->GetName().c_str() + "-org.nii";
                        mitk::IOUtil::Save(image, input_volume.toStdString());
                        
                        cropped_volume = directory_processing + dirSep + node->GetName().c_str() + "-cropped.nii";
                        
                        nii_found = true;
                        
                    }
                    
                } else {
                    
                    this->BusyCursorOff();
                    QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you selected .nii files!");
                    nodes.clear();
                    return;
                    
                }
                
                
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Selected node does not exists!");
                nodes.clear();
                return;
                
            }
            
        }
        
        
        if (nii_found && mask_found) {
            
            this->BusyCursorOn();
            
            QString cmd_crop_str = main_mirtk_path +  "crop_volume " + input_volume + " " + input_mask + " " + cropped_volume + "";
            
            std::system((cmd_crop_str.toStdString()).c_str());
            
            for (int i=0; i<10; i++) {
                if(!QFileInfo::exists(cropped_volume)) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                } else {
                    break;
                }
            }
            
            this->BusyCursorOff();
            
            if(QFileInfo::exists(cropped_volume)) {
                
                mitk::IOUtil::Load(cropped_volume.toStdString(), *this->GetDataStorage());
                QMessageBox::information(NULL, "Attention", "Volume cropping was successful!");
                
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Volume cropping was not successful!");
                nodes.clear();
                return;
            }
            
        } else {
            
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you selected an image volume and a mask volume that contains 'mask' in the name!");
            nodes.clear();
            return;
        }
        
        nodes.clear();
        
        
    } else {
        
        this->BusyCursorOff();
        QMessageBox::warning(NULL, "Attention", "Error! Could not find MIRTK executables in " + main_mirtk_path + " !");
        return;
        
    }
    
}



void FestiveView::ReorientVolumeReference() {
    

    this->GetSite()->GetPage()->ShowView("org.mitk.views.pointsetinteraction");

    
    QString test_mirtk = main_mirtk_path + "reorient";
    
    
    if(QFileInfo::exists(test_mirtk)) {
        

        if (directory.isEmpty()) {
            directory = QFileDialog::getExistingDirectory(NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
            if (directory.isEmpty() || directory.simplified().contains(" ")) {
                QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
                directory = QString();
                return;
            }//_if
            
            char dirSep = mitk::IOUtil::GetDirectorySeparator();
            std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
            std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
            directory_processing = directory + dirSep + "Processing";
            
        }
        

        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        
        QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
        if (nodes.size() != 4) {
            QMessageBox::warning(NULL, "Attention", "Please select two image volumes and two point sets for reorientation (target and source)!");
            return;
        }//_if
        
        
        QString image_1_name;
        QString image_2_name;
        
        QString pointset_1_name;
        QString pointset_2_name;
        
        QString beginning = "";
        QString short_image_1_name;
        QString short_image_2_name;
        
        QString short_pointset_1_name;
        QString short_pointset_2_name;
        
        
        bool found_im_1 = false;
        bool found_im_2 = false;
        bool found_pts_1 = false;
        bool found_pts_2 = false;
        
        int count_im = 0;
        int count_pts = 0;
        
        
        
        foreach (mitk::DataNode::Pointer node, nodes) {
            
            mitk::BaseData::Pointer data = node->GetData();
            
            if (data) {
                
                //Test if this data item is an image
                mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data.GetPointer());
                
                if (image) {
                    
                    if(count_im == 0) {
                        
                        short_image_1_name = beginning + node->GetName().c_str();
                        image_1_name = directory_processing + dirSep + node->GetName().c_str() + "-org.nii";
                        
                        mitk::IOUtil::Save(image, image_1_name.toStdString());
                        found_im_1 = true;
                        count_im++;
                        
                    } else {
                        
                        short_image_2_name = beginning + node->GetName().c_str();
                        image_2_name = directory_processing + dirSep + node->GetName().c_str() + "-org.nii";
                        
                        mitk::IOUtil::Save(image, image_2_name.toStdString());
                        found_im_2 = true;
                        count_im++;
                    }
                    
                    
                } else {
                    
                    mitk::PointSet::Pointer pointset_file = dynamic_cast<mitk::PointSet*>(data.GetPointer());
                    
                    if (pointset_file) {
                        
                        if(count_pts == 0) {
                            
                            short_pointset_1_name = beginning + node->GetName().c_str();
                            pointset_1_name = directory_processing + dirSep + node->GetName().c_str() + "-org.mps";
                            
                            mitk::IOUtil::Save(pointset_file, pointset_1_name.toStdString());
                            found_pts_1 = true;
                            count_pts++;
                            
                        } else {
                            
                            
                            short_pointset_2_name = beginning + node->GetName().c_str();
                            pointset_2_name = directory_processing + dirSep + node->GetName().c_str() + "-org.mps";
                            
                            mitk::IOUtil::Save(pointset_file, pointset_2_name.toStdString());
                            found_pts_2 = true;
                            count_pts++;
                        }
                        
 
                    } else {
                        
                        this->BusyCursorOff();
                        QMessageBox::warning(NULL, "Attention", "Error! Selected nodes is not an image or a pointset'!");
                        nodes.clear();
                        return;
                    }
                    
            
                    
                }
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Selected node does not exists!");
                nodes.clear();
                return;
            }
            
        }//_for
        
        
        if ( !(found_im_1 && found_im_2 && found_pts_1 && found_pts_2 && (count_pts == 2) && (count_im == 2)) ) {
            
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you selected two images and two pointsets'!");
            nodes.clear();
            return;
            
        }
        
        
        
        //Prompt for parameters
        QDialog* inputs = new QDialog(0,0);
        m_ReorientReference.setupUi(inputs);
        
        // target
        m_ReorientReference.listWidget->addItem(short_image_1_name);
        m_ReorientReference.listWidget->addItem(short_image_2_name);
        
        m_ReorientReference.listWidget_3->addItem(short_pointset_1_name);
        m_ReorientReference.listWidget_3->addItem(short_pointset_2_name);
        
        // source
        m_ReorientReference.listWidget_2->addItem(short_image_1_name);
        m_ReorientReference.listWidget_2->addItem(short_image_2_name);

        m_ReorientReference.listWidget_4->addItem(short_pointset_1_name);
        m_ReorientReference.listWidget_4->addItem(short_pointset_2_name);
        
        
        //        m_ReorientReference.listWidget->setCurrentRow(0);
        //        m_ReorientReference.listWidget_2->setCurrentRow(1);
        //        m_ReorientReference.listWidget_3->setCurrentRow(0);
        //        m_ReorientReference.listWidget_4->setCurrentRow(1);
        
        
        connect(m_ReorientReference.buttonBox, SIGNAL(accepted()), inputs, SLOT(accept()));
        connect(m_ReorientReference.buttonBox, SIGNAL(rejected()), inputs, SLOT(reject()));
        int dialogCode = inputs->exec();
        
        //Act on dialog return code
        if (dialogCode == QDialog::Accepted) {
            
            this->BusyCursorOn();
            
            
            bool interpolate_output = m_ReorientReference.checkBox->checkState();
            
            
            if (!(m_ReorientReference.listWidget->currentItem()) || !(m_ReorientReference.listWidget_2->currentItem()) || !(m_ReorientReference.listWidget_3->currentItem()) || !(m_ReorientReference.listWidget_4->currentItem())) {
                
                QMessageBox::warning(NULL, "Attention", "Error! Please select target and source images and pointsets!");
                nodes.clear();
                this->BusyCursorOff();
                return;
            }
            
            
            QString target_image_name = m_ReorientReference.listWidget->currentItem()->text();
            QString source_image_name = m_ReorientReference.listWidget_2->currentItem()->text();
            
            QString target_pointset_name = m_ReorientReference.listWidget_3->currentItem()->text();
            QString source_pointset_name = m_ReorientReference.listWidget_4->currentItem()->text();
            

            
            QString output_name = m_ReorientReference.lineEdit_2->text() + ".nii";
            QString default_output_name = source_image_name + "-reoriented-ref.nii";
            
//            if (output_name.isEmpty() || !output_name.endsWith(".nii")) output_name = "Reoriented.nii";
            if (output_name.isEmpty()) output_name = default_output_name;
            
            output_name = default_output_name;
            
            
            QString reoriented_name = directory + dirSep + output_name;
            QString org_target_image_name = directory_processing + dirSep + target_image_name + "-org.nii ";
            QString org_source_image_name = directory_processing + dirSep + source_image_name + "-org.nii ";
            QString org_target_pointset_name = directory_processing + dirSep + target_pointset_name + "-org.mps ";
            QString org_source_pointset_name = directory_processing + dirSep + source_pointset_name + "-org.mps ";
            QString dofout_name = directory_processing + dirSep + "dofout.dof";
            QString tmp_name = directory_processing + dirSep + "tmp.nii ";
            
            
            int output_mode = 0;
            
            if (interpolate_output) {
                
                output_mode = 1;

            }
            
            QString reflection_name = directory_processing + dirSep + source_image_name + "-reflection.dof";
            QString rm_cmd_str = "rm " + reflection_name + "";
            std::system((rm_cmd_str.toStdString()).c_str());
        
            QString reorientation_cmd_str = main_mirtk_path + "reorient-reference " + " -ti " + org_target_image_name + " -si " + org_source_image_name + " -tp " + org_target_pointset_name + " -sp " + org_source_pointset_name + " -o " + tmp_name + " -do " + dofout_name + " -om " + QString::number(output_mode) + " \n";
            std::system((reorientation_cmd_str.toStdString()).c_str());
            
            
            QString register_cmd_str = main_mirtk_path + "register  " + org_target_image_name + " " + org_source_image_name + " -model Rigid " + " -dofin " + dofout_name + " -dofout " + dofout_name  + " -bg 0 " + " \n";
            std::system((register_cmd_str.toStdString()).c_str());
            
            
            QString transform_cmd_str = main_mirtk_path + "transform-image " + org_source_image_name + " " + reoriented_name + " -dofin " + dofout_name + " -target " + org_target_image_name + " -interp BSpline " + " \n";
            std::system((transform_cmd_str.toStdString()).c_str());
            
 

            this->BusyCursorOff();
            
            
            if(QFileInfo::exists(reoriented_name)) {
                
                mitk::IOUtil::Load(reoriented_name.toStdString(), *this->GetDataStorage());

                if(QFileInfo::exists(reflection_name)) {
                 
                    QMessageBox::information(NULL, "Attention", "Reorientation involved reflection! Output file: " + output_name + " ");
                    
                }
                else {
                    
                    QMessageBox::information(NULL, "Attention", "Reorientation was successful! Output file: " + output_name + " ");
                }
                
                
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Reorientation was not successful!");
                nodes.clear();
                return;
            }
            
            
            nodes.clear();
            inputs->deleteLater();
            this->BusyCursorOff();
            
        } else if(dialogCode == QDialog::Rejected) {
            inputs->close();
            inputs->deleteLater();
        }//_if
        
        
    } else {
        
        QMessageBox::warning(NULL, "Attention", "Error! Could not find MIRTK executables in " + main_mirtk_path + " !");
        return;
        
    }
    
    
}






void FestiveView::N4Correction() {
    
    this->BusyCursorOff();
    
    QString test_mirtk = main_mirtk_path + "reorient";
    
    if(QFileInfo::exists(test_mirtk)) {
        
        if (directory.isEmpty()) {
            directory = QFileDialog::getExistingDirectory(NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
            if (directory.isEmpty() || directory.simplified().contains(" ")) {
                QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
                directory = QString();
                return;
            }//_if
            
            char dirSep = mitk::IOUtil::GetDirectorySeparator();
            std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
            std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
            directory_processing = directory + dirSep + "Processing";
            
        }
        
        QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
        if (nodes.size() != 2) {
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Please select a volume and a mask from the Data Manager for processing!");
            return;
        }//_if
        
        bool nii_found = false;
        bool mask_found = false;
        
        QString output_volume;
        QString input_volume;
        QString input_mask;
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        
        
        foreach (mitk::DataNode::Pointer node, nodes) {
            
            mitk::BaseData::Pointer data = node->GetData();
            
            if (data) {
                
                mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data.GetPointer());
                
                if (image) {
                    
                    if (QString::fromStdString(node->GetName()).contains("mask", Qt::CaseInsensitive)) {
                        
                        input_mask = directory_processing + dirSep + node->GetName().c_str() + "-mask.nii";
                        mitk::IOUtil::Save(image, input_mask.toStdString());

                        mask_found = true;
                        
                    } else {
                        
                        input_volume = directory_processing + dirSep + node->GetName().c_str() + "-org.nii";
                        mitk::IOUtil::Save(image, input_volume.toStdString());
                        
                        
                        output_volume = directory_processing + dirSep + node->GetName().c_str() + "-N4.nii";
                        
                        
                        nii_found = true;
                        
                    }
                    
                } else {
                    this->BusyCursorOff();
                    QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you selected .nii files!");
                    nodes.clear();
                    return;
                    
                }
                
                
            } else {
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Selected node does not exists!");
                nodes.clear();
                return;
                
            }
            
        }
        
    
        
        if (nii_found && mask_found) {
            
            this->BusyCursorOn();
            

            QString cmd_mask_str0 = main_mirtk_path +  "extract-image-region " + input_mask + " " + input_mask + " -Rt1 0 -Rt2 0 ";
            
            std::system((cmd_mask_str0.toStdString()).c_str());

            
            QString cmd_mask_str = main_mirtk_path +  "transform-image " + input_mask + " " + input_mask + " -target " + input_volume + " -interp NN " + "";
            
            std::system((cmd_mask_str.toStdString()).c_str());
            
            
            
            QString cmd_proc_str = main_mirtk_path +  "N4 -i " + input_volume + " -x " + input_mask + " -o " + output_volume + "";
            
            std::system((cmd_proc_str.toStdString()).c_str());
            
            for (int i=0; i<10; i++) {
                if(!QFileInfo::exists(output_volume)) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                } else {
                    break;
                }
            }
            
            this->BusyCursorOff();
            
            if(QFileInfo::exists(output_volume)) {
                
                mitk::IOUtil::Load(output_volume.toStdString(), *this->GetDataStorage());
                QMessageBox::information(NULL, "Attention", "N4 correction was successful!");
                
            } else {
                
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! N4 correction was not successful!");
                nodes.clear();
                return;
            }
            
        } else {
            
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you selected an image volume and a mask volume that contains 'mask' in the name!");
            nodes.clear();
            return;
        }
        
        nodes.clear();
        
        
    } else {
        
        this->BusyCursorOff();
        QMessageBox::warning(NULL, "Attention", "Error! Could not find MIRTK executables in " + main_mirtk_path + " !");
        return;
        
    }
    
}




void FestiveView::LoadAtlas() {
    
    
    if (directory.isEmpty()) {
        directory = QFileDialog::getExistingDirectory(NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
        if (directory.isEmpty() || directory.simplified().contains(" ")) {
            QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
            directory = QString();
            return;
        }//_if
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
        std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
        directory_processing = directory + dirSep + "Processing";
        
    }
    
    //Prompt for parameters
    QDialog* inputs = new QDialog(0,0);
    m_Atlas.setupUi(inputs);

    connect(m_Atlas.buttonBox, SIGNAL(accepted()), inputs, SLOT(accept()));
    connect(m_Atlas.buttonBox, SIGNAL(rejected()), inputs, SLOT(reject()));
    int dialogCode = inputs->exec();
    
    //Act on dialog return code
    if (dialogCode == QDialog::Accepted) {
        
        this->BusyCursorOn();
        
        std::vector<bool> selected_atlas_files;
        selected_atlas_files.push_back(m_Atlas.box_21->checkState());
        selected_atlas_files.push_back(m_Atlas.box_22->checkState());
        selected_atlas_files.push_back(m_Atlas.box_23->checkState());
        selected_atlas_files.push_back(m_Atlas.box_24->checkState());
        selected_atlas_files.push_back(m_Atlas.box_25->checkState());
        selected_atlas_files.push_back(m_Atlas.box_26->checkState());
        selected_atlas_files.push_back(m_Atlas.box_27->checkState());
        selected_atlas_files.push_back(m_Atlas.box_28->checkState());
        selected_atlas_files.push_back(m_Atlas.box_29->checkState());
        
        selected_atlas_files.push_back(m_Atlas.box_30->checkState());
        selected_atlas_files.push_back(m_Atlas.box_31->checkState());
        selected_atlas_files.push_back(m_Atlas.box_32->checkState());
        selected_atlas_files.push_back(m_Atlas.box_33->checkState());
        selected_atlas_files.push_back(m_Atlas.box_34->checkState());
        selected_atlas_files.push_back(m_Atlas.box_35->checkState());
        selected_atlas_files.push_back(m_Atlas.box_36->checkState());
        selected_atlas_files.push_back(m_Atlas.box_37->checkState());
        selected_atlas_files.push_back(m_Atlas.box_38->checkState());
        
        int N = selected_atlas_files.size();
        
        for (int i=0; i<N; i++) {
            
            if (selected_atlas_files[i]) {
                
                int ga = i + 21;
                QString atlas_name = "brain-atlas-" + QString::number(ga) + ".nii.gz";
                
                QString file_name = brain_atlas_path + atlas_name;

                
//                std::cout << "---------------------------------------------------------" << std::endl;
//                std::cout << brain_atlas_path.toStdString() << std::endl;
//                std::cout << (QString::number(ga)).toStdString() << std::endl;
//                std::cout << file_name.toStdString() << std::endl;
//                std::cout << "---------------------------------------------------------" << std::endl;
//
                
                
//                mitk::IOUtil::Load(file_name.toStdString(), *this->GetDataStorage());
//                QMessageBox::information(NULL, "Attention", "Loaded file: " + atlas_name + " ");
//
                
                
                if(QFileInfo::exists(file_name)) {

                    mitk::IOUtil::Load(file_name.toStdString(), *this->GetDataStorage());
                    QMessageBox::information(NULL, "Attention", "Loaded file: " + atlas_name + " ");

                } else {

                    this->BusyCursorOff();
                    QMessageBox::warning(NULL, "Attention", "Could not load file: " + atlas_name + " !");
                    this->BusyCursorOff();
                    return;
                }
                
                
  
            }
            

        }

        this->BusyCursorOff();
        
    
    } else if(dialogCode == QDialog::Rejected) {
        inputs->close();
        inputs->deleteLater();
        
        
    }//_if

}



void FestiveView::LoadBodyAtlas() {
    
    
    if (directory.isEmpty()) {
        directory = QFileDialog::getExistingDirectory(NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
        if (directory.isEmpty() || directory.simplified().contains(" ")) {
            QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
            directory = QString();
            return;
        }//_if
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
        std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
        directory_processing = directory + dirSep + "Processing";
        
    }
    
    //Prompt for parameters
    QDialog* inputs = new QDialog(0,0);
    m_AtlasBody.setupUi(inputs);
    
    connect(m_AtlasBody.buttonBox, SIGNAL(accepted()), inputs, SLOT(accept()));
    connect(m_AtlasBody.buttonBox, SIGNAL(rejected()), inputs, SLOT(reject()));
    int dialogCode = inputs->exec();
    
    //Act on dialog return code
    if (dialogCode == QDialog::Accepted) {
        
        this->BusyCursorOn();
        
        std::vector<bool> selected_atlas_files;
        
        selected_atlas_files.push_back(m_AtlasBody.box_20->checkState());
        selected_atlas_files.push_back(m_AtlasBody.box_21->checkState());
        selected_atlas_files.push_back(m_AtlasBody.box_22->checkState());
        selected_atlas_files.push_back(m_AtlasBody.box_23->checkState());
        selected_atlas_files.push_back(m_AtlasBody.box_24->checkState());
        selected_atlas_files.push_back(m_AtlasBody.box_25->checkState());
        selected_atlas_files.push_back(m_AtlasBody.box_26->checkState());
        selected_atlas_files.push_back(m_AtlasBody.box_27->checkState());
        selected_atlas_files.push_back(m_AtlasBody.box_28->checkState());
        selected_atlas_files.push_back(m_AtlasBody.box_29->checkState());
        
        selected_atlas_files.push_back(m_AtlasBody.box_30->checkState());
        selected_atlas_files.push_back(m_AtlasBody.box_31->checkState());

        int N = selected_atlas_files.size();
        
        for (int i=0; i<N; i++) {
            
            if (selected_atlas_files[i]) {
                
                int ga = i + 20;
                QString atlas_name = "body-atlas-" + QString::number(ga) + ".nii.gz";
                QString file_name = body_atlas_path + atlas_name;
                
                
                if(QFileInfo::exists(file_name)) {
                    
                    mitk::IOUtil::Load(file_name.toStdString(), *this->GetDataStorage());
                    QMessageBox::information(NULL, "Attention", "Loaded file: " + atlas_name + " ");
                    
                } else {
                    
                    this->BusyCursorOff();
                    QMessageBox::warning(NULL, "Attention", "Could not load file: " + atlas_name + " !");
                    return;
                }
                
            }
            
            
        }
        
        this->BusyCursorOff();
        
        
    } else if(dialogCode == QDialog::Rejected) {
        inputs->close();
        inputs->deleteLater();
        
    }//_if
    
    
}



void FestiveView::ReorientVolume() {


    this->GetSite()->GetPage()->ShowView("org.mitk.views.pointsetinteraction");
    
    
    QString test_mirtk = main_mirtk_path + "reorient";
    
    if(QFileInfo::exists(test_mirtk)) {
        
        
        if (directory.isEmpty()) {
            directory = QFileDialog::getExistingDirectory(NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(), QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
            if (directory.isEmpty() || directory.simplified().contains(" ")) {
                QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
                directory = QString();
                return;
            }//_if
            
            char dirSep = mitk::IOUtil::GetDirectorySeparator();
            std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
            std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
            directory_processing = directory + dirSep + "Processing";
            
        }
        
        
        QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
        if (nodes.size() != 2) {
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Please select a volume and a point set from the Data Manager for processing!");
            return;
        }//_if
        
        
        bool nii_found = false;
        bool mps_found = false;
        
        QString reoriented_volume;
        QString input_volume;
        QString input_pointset;
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        
        foreach (mitk::DataNode::Pointer node, nodes) {
            
            mitk::BaseData::Pointer data = node->GetData();
            
            if (data) {
                
                //Test if this data item is an image
                mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data.GetPointer());
                
                if (image) {
                    
                    input_volume = directory + dirSep + node->GetName().c_str() + "-org.nii";
                    mitk::IOUtil::Save(image, input_volume.toStdString());
                    
                    reoriented_volume = directory_processing + dirSep + node->GetName().c_str() + "-reoriented.nii";
                    
                    nii_found = true;
                    
                } else {
                    
                    mitk::PointSet::Pointer pointset_file = dynamic_cast<mitk::PointSet*>(data.GetPointer());
                    
                    if (pointset_file) {
                        
                        input_pointset = directory_processing + dirSep + dirSep + node->GetName().c_str() + ".mps";
                        
                        mitk::IOUtil::Save(pointset_file, input_pointset.toStdString());
                        
                        if(QFileInfo::exists(input_pointset)) {
                            
                            mps_found = true;
                            
                        } else {
                            
                            this->BusyCursorOff();
                            QMessageBox::warning(NULL, "Attention", "Error! The pointset file could not be saved!");
                            nodes.clear();
                            return;
                        }
                        
                    } else {
                        
                        this->BusyCursorOff();
                        QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you selected one image and one pointset!");
                        nodes.clear();
                        return;
                        
                    }
                    
                    
//                    if (QString::fromStdString(node->GetName()).contains("pointset", Qt::CaseInsensitive)) {
//
//                        mitk::PointSet::Pointer pointset_file = dynamic_cast<mitk::PointSet*>(data.GetPointer());
//
//                        input_pointset = directory + dirSep + dirSep + node->GetName().c_str() + ".mps";
//
//                        mitk::IOUtil::Save(pointset_file, input_pointset.toStdString());
//
//
//                        if(QFileInfo::exists(input_pointset)) {
//
//                            mps_found = true;
//
//                        } else {
//
//                            QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you that the selected point set was saved as .mps file with the name '*pointset*'!");
//                            nodes.clear();
//                            return;
//                        }
//
//
//
//                    } else {
//
//                        QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you that the selected point set was saved as .mps file with the name '*pointset*'!");
//                        nodes.clear();
//                        return;
//                    }
//
                    
                    
                }
            } else {
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Selected node does not exists!");
                nodes.clear();
                return;
            }
        }//_for
        
        
        if (nii_found && mps_found) {
            
            
            this->BusyCursorOn();
            
            
            QString cmd_reorient_str = main_mirtk_path +  "reorient -i " + input_volume + " -p " + input_pointset + " -o " + reoriented_volume + "";
            
            std::system((cmd_reorient_str.toStdString()).c_str());
            
            for (int i=0; i<10; i++) {
                if(!QFileInfo::exists(reoriented_volume)) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                } else {
                    break;
                }
            }
            
            //        std::unique_ptr<CemrgCommandLine> regCMD(new CemrgCommandLine());
            //        regCMD->RunReorientation(cmd_reorient_str);
            
            this->BusyCursorOff();
            
            if(QFileInfo::exists(reoriented_volume)) {
                
                mitk::IOUtil::Load(reoriented_volume.toStdString(), *this->GetDataStorage());
                QMessageBox::information(NULL, "Attention", "Reorientation was successful!");
                
            } else {
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Error! Reorientation was not successful!");
                nodes.clear();
                return;
            }
            
        } else {
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Error! Please make sure that you selected an image volume, and that the point set was saved as .mps file with the name '*pointset*'!");
            nodes.clear();
            return;
        }
        
        
        nodes.clear();
        
    } else {
        this->BusyCursorOff();
        QMessageBox::warning(NULL, "Attention", "Error! Could not find MIRTK executables in " + main_mirtk_path + " !");
        return;
        
    }
    
}


void FestiveView::StopReconstruction() {
    
    bool flag_stopped = false;
    
    if (current_svr_recon_cmd) {
        current_svr_recon_cmd->StopProcess();
        flag_stopped = true;
    }
    
    if (current_gsvr_recon_cmd) {
        current_gsvr_recon_cmd->StopProcess();
        flag_stopped = true;
    }
    
    if (current_dsvr_recon_cmd) {
        current_dsvr_recon_cmd->StopProcess();
        flag_stopped = true;
    }
    
    if (flag_stopped) {
        
        QMessageBox::critical(NULL, "Attention", "All current reconstructions were stopped!");
        mitk::ProgressBar::GetInstance()->Progress();
    }
    
    this->BusyCursorOff();
    
}


void FestiveView::CheckServer() {
    
    
    
    //Prompt for parameters
    QDialog* inputs = new QDialog(0,0);
    m_ServerInfo.setupUi(inputs);
    connect(m_ServerInfo.buttonBox, SIGNAL(accepted()), inputs, SLOT(accept()));
    connect(m_ServerInfo.buttonBox, SIGNAL(rejected()), inputs, SLOT(reject()));
    int dialogCode = inputs->exec();
    
    
    //Act on dialog return code
    if (dialogCode == QDialog::Accepted) {
        
        
        
        QString userName = m_ServerInfo.lineEdit_1->text();
        QString serverName = m_ServerInfo.lineEdit_2->text();
        
        
        if (userName.isEmpty()) {
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Please enter a valid username!");
            return;
        }//_if
        
        if (serverName.isEmpty()) serverName = server;
        
        
        //        this->BusyCursorOn();
        mitk::ProgressBar::GetInstance()->AddStepsToDo(1);
        
        if (cmd) {
            
            check_cmd = std::unique_ptr<CemrgCommandLine>(new CemrgCommandLine());
            check_cmd->CheckServer(userName, serverName, 0);
            
            
        } else {
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Server Connection has not been established!");
            mitk::ProgressBar::GetInstance()->Progress();
        }//_if_cmd
        
        //Clean up
        //        this->BusyCursorOff();
        inputs->deleteLater();
        
    } else if(dialogCode == QDialog::Rejected) {
        inputs->close();
        inputs->deleteLater();
    }//_if
    
    
}



void FestiveView::SaveMASK() {
    
    //Check for selection of images
    QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
    if (nodes.size() != 1) {
        QMessageBox::warning(NULL, "Attention","Please select a mask from the Data Manager to save!");
        return;
    }
    
    //Ask the user for a dir to store data
    if (directory.isEmpty()) {
        directory = QFileDialog::getExistingDirectory(NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(),
                                                      QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
        if (directory.isEmpty() || directory.simplified().contains(" ")) {
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
            directory = QString();
            return;
        }//_if
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
        std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
        directory_processing = directory + dirSep + "Processing";
        
    }
    
    //Find the selected node
    QString path;
    mitk::DataNode::Pointer mskNode = nodes.at(0);
    mitk::BaseData::Pointer data = mskNode->GetData();
    if (data) {
        //Test if this data item is an image
        mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data.GetPointer());
        if (image) {
            
            bool ok;
            QString fileName = "Mask.nii";
            fileName = QInputDialog::getText(NULL, tr("Save As"), tr("File Name:"), QLineEdit::Normal, fileName, &ok);
            if (ok && !fileName.isEmpty() && fileName.endsWith(".nii")) {
                
                path = directory + mitk::IOUtil::GetDirectorySeparator() + fileName;
                mitk::IOUtil::Save(image, path.toStdString());
                
            } else {
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Please type a file name with the right extension (i.e. .nii)!");
                return;
            }//_fileName
            
        } else {
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Please select a mask image from the Data Manager!");
            return;
        }//_image
    } else
    return;
}


void FestiveView::EstablishConnection() {
    
    //Prompt for username and password
    QDialog* inputs = new QDialog(0,0);
    m_UILogin.setupUi(inputs);
    connect(m_UILogin.buttonBox, SIGNAL(accepted()), inputs, SLOT(accept()));
    connect(m_UILogin.buttonBox, SIGNAL(rejected()), inputs, SLOT(reject()));
    int dialogCode = inputs->exec();
    
    //Act on dialog return code
    if (dialogCode == QDialog::Accepted) {
        
        userID = m_UILogin.lineEdit_1->text();
        server = m_UILogin.lineEdit_2->text();
        
        //Set default values
        if (userID.isEmpty()) {
            QMessageBox::warning(NULL, "Attention", "Please enter a valid username!");
            return;
        }//_if
        
        
        //        // is message is not really needed
        if (server.isEmpty()) {
            //            QMessageBox::warning(NULL, "Attention", "Reverting to default server!");
            server = "beastie02"; //"gpubeastie03-pc";
        }//_if
        
        
        this->BusyCursorOn();
        mitk::ProgressBar::GetInstance()->AddStepsToDo(1);
        
        cmd = std::unique_ptr<CemrgCommandLine>(new CemrgCommandLine());
        if (cmd) {
            if (cmd->ConnectToServer(userID,server)) {
                this->BusyCursorOff();
                QMessageBox::information(NULL, "Attention", "Connection has been established!");
                mitk::ProgressBar::GetInstance()->Progress();
            } else {
                this->BusyCursorOff();
                QMessageBox::critical(NULL, "Attention", "Connection has not been established!");
                mitk::ProgressBar::GetInstance()->Progress();
            }
        }//_if_cmd
        
        //Clean up
        mitk::ProgressBar::GetInstance()->Progress();
        this->BusyCursorOff();
        inputs->deleteLater();
        
    } else if(dialogCode == QDialog::Rejected) {
        inputs->close();
        inputs->deleteLater();
    }//_if
}


void FestiveView::CopyServer() {
    
    //Check for selection of data
    QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
    if (nodes.empty()) {
        QMessageBox::warning(
                             NULL, "Attention",
                             "Please select all your items from the Data Manager for transfering to the server!");
        return;
    }//_if
    
    //Ask the user for a dir to store data
    if (directory.isEmpty()) {
        directory = QFileDialog::getExistingDirectory(
                                                      NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(),
                                                      QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
        if (directory.isEmpty() || directory.simplified().contains(" ")) {
            QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
            directory = QString();
            return;
        }//_if
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
        std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
        directory_processing = directory + dirSep + "Processing";
        
    }
    
    /*** UNIX based commands ***/
    char dirSep = mitk::IOUtil::GetDirectorySeparator();
    std::system(("rm -rf " + (directory + dirSep + "Transfer").toStdString()).c_str());
    std::system(("mkdir "  + (directory + dirSep + "Transfer").toStdString()).c_str());
    
//    std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
//    std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
    
    imgsList.clear();
    
    QString path;
    this->BusyCursorOn();
    mitk::ProgressBar::GetInstance()->AddStepsToDo(nodes.size());
    foreach (mitk::DataNode::Pointer node, nodes) {
        mitk::BaseData::Pointer data = node->GetData();
        mitk::ProgressBar::GetInstance()->Progress();
        if (data) {
            //Test if this data item is an image
            mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data.GetPointer());
            if (image) {
                if (QString::fromStdString(node->GetName()).contains("Mask", Qt::CaseInsensitive)) node->SetName("Mask");
                imgsList << QString::fromStdString(node->GetName() + ".nii.gz");
                path = directory + dirSep + "Transfer" + dirSep + node->GetName().c_str() + ".nii.gz";
                mitk::IOUtil::Save(image, path.toStdString());
            } else
            return;
        } else
        return;
    }//_for
    
    //Transfer to server
    if (cmd) {
        mitk::ProgressBar::GetInstance()->AddStepsToDo(1);
        std::unique_ptr<CemrgCommandLine> scpCMD(new CemrgCommandLine());
        if (scpCMD->TransferTFServer(directory, "Transfer", userID, server, false)) {
            this->BusyCursorOff();
            QMessageBox::information(NULL, "Attention", "Transfer was successful!");
            mitk::ProgressBar::GetInstance()->Progress();
        } else {
            this->BusyCursorOff();
            QMessageBox::critical(NULL, "Attention", "Transfer was not successful!");
            mitk::ProgressBar::GetInstance()->Progress();
        }//_if
    } else {
        this->BusyCursorOff();
        QMessageBox::warning(NULL, "Attention", "Server Connection has not been established!");
    }//_if_cmd
    
    nodes.clear();
    this->BusyCursorOff();
}


void FestiveView::SaveSubjectFolder() {
    
    this->BusyCursorOff();
    
    if (!userID.isEmpty()) {
        
        QString serverName = "gpubeastie01-pc";
        
        //Prompt for parameters
        QDialog* inputs = new QDialog(0,0);
        m_SubjectCaseInfo.setupUi(inputs);
        connect(m_SubjectCaseInfo.buttonBox, SIGNAL(accepted()), inputs, SLOT(accept()));
        connect(m_SubjectCaseInfo.buttonBox, SIGNAL(rejected()), inputs, SLOT(reject()));
        int dialogCode = inputs->exec();
        
        
        //Act on dialog return code
        if (dialogCode == QDialog::Accepted) {
            
            
            QString caseId = m_SubjectCaseInfo.lineEdit_1->text();
            QString caseDate = m_SubjectCaseInfo.lineEdit_2->text();
            QString locationServer = m_SubjectCaseInfo.lineEdit_3->text();
            
            
            if (caseDate.isEmpty() || caseId.isEmpty()) {
                QMessageBox::warning(NULL, "Attention", "Please specify case ID and date!");
                mitk::ProgressBar::GetInstance()->Progress();
                return;
            }
            
            if (locationServer.isEmpty()) {
                locationServer = "/pnraw01/FetalPreprocessing/BodyRecontructions";
            }
            
            QString subjectFolderName;
            
            if (!locationServer.endsWith("/")) {
                subjectFolderName = locationServer + "/" + caseDate + "_" + caseId;
            }
            else {
                subjectFolderName = locationServer + caseDate + "_" + caseId;
            }
            
            
            this->BusyCursorOn();
            mitk::ProgressBar::GetInstance()->AddStepsToDo(1);
            
            if (cmd) {
                std::unique_ptr<CemrgCommandLine> conCMD(new CemrgCommandLine());
                
                if (conCMD->SaveResultsToServer(userID, serverName, subjectFolderName)) {
                    this->BusyCursorOff();
                    QMessageBox::information(NULL, "Attention", "File transfer Completed!");
                }
                else {
                    this->BusyCursorOff();
                    QMessageBox::information(NULL, "Attention", "File was not successful!");
                }
                mitk::ProgressBar::GetInstance()->Progress();
            } else {
                this->BusyCursorOff();
                QMessageBox::warning(NULL, "Attention", "Server Connection has not been established!");
                mitk::ProgressBar::GetInstance()->Progress();
            }//_if_cmd
            
            //Clean up
            this->BusyCursorOff();
            inputs->deleteLater();
            
        } else if(dialogCode == QDialog::Rejected) {
            inputs->close();
            inputs->deleteLater();
        }//_if
        
    }
    else {
        this->BusyCursorOff();
        QMessageBox::warning(NULL, "Attention", "You need to establish connection first!");
    }
    
    this->BusyCursorOff();
    
}


void FestiveView::ReconstructionGSVR() {
    
    this->BusyCursorOff();
    
    //Check for selection of data
    QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
    if (nodes.size() != 1) {
        QMessageBox::warning(
                             NULL, "Attention",
                             "Please select the target image from the Data Manager before running the reconstruction step!");
        return;
    }//_if
    
    //Ask the user for a dir to store data
    if (directory.isEmpty()) {
        directory = QFileDialog::getExistingDirectory(
                                                      NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(),
                                                      QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
        if (directory.isEmpty() || directory.simplified().contains(" ")) {
            QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
            directory = QString();
            return;
        }//_if
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
        std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
        directory_processing = directory + dirSep + "Processing";
        
    }
    
    //Prompt for parameters
    QDialog* inputs = new QDialog(0,0);
    m_UIReconst.setupUi(inputs);
    connect(m_UIReconst.buttonBox, SIGNAL(accepted()), inputs, SLOT(accept()));
    connect(m_UIReconst.buttonBox, SIGNAL(rejected()), inputs, SLOT(reject()));
    int dialogCode = inputs->exec();
    
    //Act on dialog return code
    if (dialogCode == QDialog::Accepted) {
        
        bool ok1, ok2, ok3, ok6;
        double reso = m_UIReconst.lineEdit_1->text().toDouble(&ok1);
        double delt = m_UIReconst.lineEdit_2->text().toDouble(&ok2);
        int package = m_UIReconst.lineEdit_3->text().toInt(&ok3);
        QString out = m_UIReconst.lineEdit_4->text();
        int n_iterations = m_UIReconst.lineEdit_6->text().toInt(&ok6);
        
        QString serverName = m_UIReconst.lineEdit_5->text();
        
        
        // always on GB3
        //        QString serverName = "gpubeastie03-pc";
        
        //        // this message is not really needed
        //        //Set default values
        //        if (!ok1 || !ok2 || !ok3 || out.isEmpty() || !out.endsWith(".nii"))
        //            QMessageBox::warning(NULL, "Attention", "Reverting to default parameters!");
        
        if (!ok1) reso = 0.75;
        if (!ok2) delt = 150.0;
        if (!ok3) package = 4;
        
        
        if (serverName.isEmpty()) serverName = "gpubeastie03-pc";
        if (!ok6) n_iterations = 3;
        outName = (out.isEmpty()) ? "GSVR-Output.nii" : out;
        // outName = (out.isEmpty() || !out.endsWith(".nii")) ? "GSVR-Output.nii" : out;
        //_if
        
        //        this->BusyCursorOn();
        mitk::ProgressBar::GetInstance()->AddStepsToDo(1);
        
        if (cmd) {
            
            QString targetImage = QString::fromStdString(nodes.at(0)->GetName() + ".nii.gz");
            
            // new cmd object
            current_gsvr_recon_cmd = std::unique_ptr<CemrgCommandLine>(new CemrgCommandLine());
            current_gsvr_recon_cmd->GSVRReconstruction(userID, serverName, imgsList, targetImage, reso, delt, n_iterations, package, outName);
            
            //            QMessageBox::information(NULL, "Attention", "Reconstruction Completed!");
            //            mitk::ProgressBar::GetInstance()->Progress();
            
            
        } else {
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Server Connection has not been established!");
            mitk::ProgressBar::GetInstance()->Progress();
        }//_if_cmd
        
        //Clean up
        //        this->BusyCursorOff();
        
        inputs->deleteLater();
        
    } else if(dialogCode == QDialog::Rejected) {
        inputs->close();
        inputs->deleteLater();
    }//_if
    
    this->BusyCursorOff();
    
}


void FestiveView::ReconstructionSVR() {
    
    this->BusyCursorOff();
    
    //Check for selection of data
    QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
    if (nodes.size() != 1) {
        QMessageBox::warning(NULL, "Attention","Please select the target image from the Data Manager before running the reconstruction step!");
        return;
    }//_if
    
    //Ask the user for a dir to store data
    if (directory.isEmpty()) {
        directory = QFileDialog::getExistingDirectory(
                                                      NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(),
                                                      QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
        if (directory.isEmpty() || directory.simplified().contains(" ")) {
            QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
            directory = QString();
            return;
        }//_if
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
        std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
        directory_processing = directory + dirSep + "Processing";
        
    }
    
    
    //Prompt for parameters
    QDialog* inputs = new QDialog(0,0);
    m_UIReconstSVR.setupUi(inputs);
    connect(m_UIReconstSVR.buttonBox, SIGNAL(accepted()), inputs, SLOT(accept()));
    connect(m_UIReconstSVR.buttonBox, SIGNAL(rejected()), inputs, SLOT(reject()));
    int dialogCode = inputs->exec();
    
    
    //Act on dialog return code
    if (dialogCode == QDialog::Accepted) {
        
        bool ok1, ok2, ok3, ok6, ok8, ok7;
        double reso = m_UIReconstSVR.lineEdit_1->text().toDouble(&ok1);
        double delt = m_UIReconstSVR.lineEdit_2->text().toDouble(&ok2);
        int package = m_UIReconstSVR.lineEdit_3->text().toInt(&ok3);
        double lambda = m_UIReconstSVR.lineEdit_8->text().toDouble(&ok8);
        double thickness = m_UIReconstSVR.lineEdit_7->text().toDouble(&ok7);
        QString out = m_UIReconstSVR.lineEdit_4->text();
        QString serverName = m_UIReconstSVR.lineEdit_5->text();
        int n_iterations = m_UIReconstSVR.lineEdit_6->text().toInt(&ok6);
        
        
        bool flag_svronly = m_UIReconstSVR.checkBox_1->checkState();
        bool flag_noglobal = m_UIReconstSVR.checkBox_2->checkState();
        
        
        
        //        // this message is not really needed
        //        //Set default values
        //        if (!ok1 || !ok2 || !ok3 || out.isEmpty() || !out.endsWith(".nii"))
        //        QMessageBox::warning(NULL, "Attention", "Reverting to default parameters!");
        
        if (!ok1) reso = 0.75;
        if (!ok2) delt = 150.0;
        if (!ok3) package = 1;
        if (!ok8) lambda = 0.01;
        if (!ok7) thickness = -1;
        if (serverName.isEmpty()) serverName = "beastie02";
        if (!ok6) n_iterations = 3;
        outName = (out.isEmpty()) ? "SVR-Output.nii" : out;
        // outName = (out.isEmpty() || !out.endsWith(".nii")) ? "SVR-Output.nii" : out;
        //_if
        
        
        std::cout << thickness << std::endl;
        
        
        //        this->BusyCursorOn();
        mitk::ProgressBar::GetInstance()->AddStepsToDo(1);
        
        if (cmd) {
            
            QString targetImage = QString::fromStdString(nodes.at(0)->GetName() + ".nii.gz");
            
            current_svr_recon_cmd = std::unique_ptr<CemrgCommandLine>(new CemrgCommandLine());
            current_svr_recon_cmd->SVRReconstruction(userID, serverName, imgsList, targetImage, reso, delt, n_iterations, package, lambda, thickness, flag_svronly, flag_noglobal, outName);
            
            // std::unique_ptr<CemrgCommandLine> conCMD(new CemrgCommandLine());
            // conCMD->SVRReconstruction(userID, serverName, imgsList, targetImage, reso, delt, n_iterations, package, outName);
            
            
            //            // this message is not required now
            //            QMessageBox::information(NULL, "Attention", "Reconstruction Completed!");
            //            mitk::ProgressBar::GetInstance()->Progress();
            
            
        } else {
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Server Connection has not been established!");
            mitk::ProgressBar::GetInstance()->Progress();
        }//_if_cmd
        
        //Clean up
        //        this->BusyCursorOff();
        inputs->deleteLater();
        
    } else if(dialogCode == QDialog::Rejected) {
        inputs->close();
        inputs->deleteLater();
    }//_if
    
    this->BusyCursorOff();
}


void FestiveView::ReconstructionDSVR() {
    
    this->BusyCursorOff();
    
    //Check for selection of data
    QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
    if (nodes.size() != 1) {
        QMessageBox::warning(NULL, "Attention", "Please select the target image from the Data Manager before running the reconstruction step!");
        return;
    }//_if
    
    //Ask the user for a dir to store data
    if (directory.isEmpty()) {
        directory = QFileDialog::getExistingDirectory(NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(),
                                                      QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
        if (directory.isEmpty() || directory.simplified().contains(" ")) {
            QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
            directory = QString();
            return;
        }//_if
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
        std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
        directory_processing = directory + dirSep + "Processing";
        
    }
    
    //Prompt for parameters
    QDialog* inputs = new QDialog(0,0);
    m_UIReconstDSVR.setupUi(inputs);
    connect(m_UIReconstDSVR.buttonBox, SIGNAL(accepted()), inputs, SLOT(accept()));
    connect(m_UIReconstDSVR.buttonBox, SIGNAL(rejected()), inputs, SLOT(reject()));
    int dialogCode = inputs->exec();
    
    
    //Act on dialog return code
    if (dialogCode == QDialog::Accepted) {
        
        bool ok1, ok2, ok3, ok6, ok7;
        double resolution = m_UIReconstDSVR.lineEdit_1->text().toDouble(&ok1);
        double delta = m_UIReconstDSVR.lineEdit_2->text().toDouble(&ok2);
        double lambda = m_UIReconstDSVR.lineEdit_6->text().toDouble(&ok6);
        double thickness = m_UIReconstDSVR.lineEdit_7->text().toDouble(&ok7);
        int dilation = m_UIReconstDSVR.lineEdit_3->text().toInt(&ok3);
        QString out = m_UIReconstDSVR.lineEdit_4->text();
        QString serverName = m_UIReconstDSVR.lineEdit_5->text();
        
        
        bool flag_remote = m_UIReconstDSVR.checkBox->checkState();
        
        
        //        // this message is not really needed
        //        //Set default values
        //        if (!ok1 || !ok2 || !ok3 || out.isEmpty() || !out.endsWith(".nii"))
        //        QMessageBox::warning(NULL, "Attention", "Reverting to default parameters!");
        
        
        if (!ok1) resolution = 0.85;
        if (!ok2) delta = 150.0;
        if (!ok6) lambda = 0.015;
        if (!ok3) dilation = 7;
        if (!ok7) thickness = -1;
        if (serverName.isEmpty()) serverName = "beastie02"; //server;
        outName = (out.isEmpty())? "DSVR-Output.nii" : out;

        // outName = (out.isEmpty() || !out.endsWith(".nii")) ? "DSVR-Output.nii" : out;
        //_if
        
        //        this->BusyCursorOn();
        mitk::ProgressBar::GetInstance()->AddStepsToDo(1);
        
        if (cmd) {
            
            QString targetImage = QString::fromStdString(nodes.at(0)->GetName() + ".nii.gz");
            
            
            current_dsvr_recon_cmd = std::unique_ptr<CemrgCommandLine>(new CemrgCommandLine());
            current_dsvr_recon_cmd->DSVRReconstruction(userID, serverName, imgsList, targetImage, resolution, thickness, delta, lambda, dilation, flag_remote, outName);
            
            
            // std::unique_ptr<CemrgCommandLine> conCMD(new CemrgCommandLine());
            // conCMD->DSVRReconstruction(userID, serverName, imgsList, targetImage, reso, delt, dilation, outName);
            
            //            // this message is not required now
            //            QMessageBox::information(NULL, "Attention", "Reconstruction Completed!");
            //            mitk::ProgressBar::GetInstance()->Progress();
            
            
        } else {
            this->BusyCursorOff();
            QMessageBox::warning(NULL, "Attention", "Server Connection has not been established!");
            mitk::ProgressBar::GetInstance()->Progress();
        }//_if_cmd
        
        //Clean up
        //        this->BusyCursorOff();
        inputs->deleteLater();
        
    } else if(dialogCode == QDialog::Rejected) {
        inputs->close();
        inputs->deleteLater();
    }//_if
    
    this->BusyCursorOff();
    
}


void FestiveView::Download() {
    
    this->BusyCursorOff();
    
    //Ask the user for a dir to store data
    if (directory.isEmpty()) {
        directory = QFileDialog::getExistingDirectory(
                                                      NULL, "Open Project Directory", (QDir::homePath().toStdString()).c_str(),
                                                      QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
        if (directory.isEmpty() || directory.simplified().contains(" ")) {
            QMessageBox::warning(NULL, "Attention", "Please select a project directory with no spaces in the path!");
            directory = QString();
            return;
        }//_if
        
        char dirSep = mitk::IOUtil::GetDirectorySeparator();
        std::system(("rm -rf " + (directory + dirSep + "Processing").toStdString()).c_str());
        std::system(("mkdir "  + (directory + dirSep + "Processing").toStdString()).c_str());
        directory_processing = directory + dirSep + "Processing";
        
    }
    
    //File to download
    bool ok;
    outName = QInputDialog::getText(NULL, tr("To Download"), tr("File Name:"), QLineEdit::Normal, outName, &ok);
    // if (!ok || outName.isEmpty() || !outName.endsWith(".nii") )

    // if (!ok || outName.isEmpty() )
    // QMessageBox::warning(NULL, "Attention", "Reverting to default file name!");
    
//    outName = (outName.isEmpty() || !outName.endsWith(".nii")) ? "SVR-Output.nii" : outName;
    
    outName = (outName.isEmpty()) ? "SVR-Output.nii" : outName;
    
    
    
    //Transfer from server
    this->BusyCursorOn();
    if (cmd) {
        mitk::ProgressBar::GetInstance()->AddStepsToDo(1);
        std::unique_ptr<CemrgCommandLine> scpCMD(new CemrgCommandLine());
        if (scpCMD->TransferTFServer(directory, outName, userID, server, true)) {
            
            QString path = directory + mitk::IOUtil::GetDirectorySeparator() + outName;
            if (mitk::IOUtil::Load(path.toStdString(), *this->GetDataStorage()).IsNull()) {
                
                QMessageBox::critical(NULL, "Attention", "Transfer was not successful!");
                mitk::ProgressBar::GetInstance()->Progress();
                return;
            }//_if
            QMessageBox::information(NULL, "Attention", "Transfer was successful!");
            mitk::ProgressBar::GetInstance()->Progress();
            
        } else {
            QMessageBox::critical(NULL, "Attention", "Transfer was not successful!");
            mitk::ProgressBar::GetInstance()->Progress();
        }//_if
    } else {
        QMessageBox::warning(NULL, "Attention", "Server Connection has not been established!");
    }//_if_cmd
    this->BusyCursorOff();
}




void FestiveView::Reset() {
    
    this->BusyCursorOff();
    
    if (!userID.isEmpty()) {
        
        this->BusyCursorOn();
        mitk::ProgressBar::GetInstance()->AddStepsToDo(1);
        
        if (cmd) {
            std::unique_ptr<CemrgCommandLine> conCMD(new CemrgCommandLine());
            bool result_c = conCMD->CleanRemote(userID, server);
            if (result_c) {
                QMessageBox::information(NULL, "Attention", "All files were removed!");
            } else {
                QMessageBox::warning(NULL, "Attention", "Error!");
            }
            
            mitk::ProgressBar::GetInstance()->Progress();
        } else {
            QMessageBox::warning(NULL, "Attention", "Server Connection has not been established!");
            mitk::ProgressBar::GetInstance()->Progress();
        }//_if_cmd
        
        this->BusyCursorOff();
        
    }
    else {
        QMessageBox::warning(NULL, "Attention", "You need to establish connection first!");
    }
    
    
    /*
     
     try {
     
     ctkPluginContext* context = mitk::kcl_cemrgapp_festive_Activator::getContext();
     mitk::IDataStorageService* dss = 0;
     ctkServiceReference dsRef = context->getServiceReference<mitk::IDataStorageService>();
     
     if (dsRef)
     dss = context->getService<mitk::IDataStorageService>(dsRef);
     
     if (!dss) {
     MITK_WARN << "IDataStorageService service not available. Unable to close project.";
     context->ungetService(dsRef);
     return;
     }
     
     mitk::IDataStorageReference::Pointer dataStorageRef = dss->GetActiveDataStorage();
     if (dataStorageRef.IsNull()) {
     //No active data storage set (i.e. not editor with a DataStorageEditorInput is active).
     dataStorageRef = dss->GetDefaultDataStorage();
     }
     
     mitk::DataStorage::Pointer dataStorage = dataStorageRef->GetDataStorage();
     if (dataStorage.IsNull()) {
     MITK_WARN << "No data storage available. Cannot close project.";
     return;
     }
     
     //Check if we got the default datastorage and if there is anything else then helper object in the storage
     if (dataStorageRef->IsDefault() && dataStorage->GetSubset(
     mitk::NodePredicateNot::New(
     mitk::NodePredicateProperty::New("helper object", mitk::BoolProperty::New(true))))->empty())
     return;
     
     //Remove everything
     mitk::DataStorage::SetOfObjects::ConstPointer nodesToRemove = dataStorage->GetAll();
     dataStorage->Remove(nodesToRemove);
     
     //void CleanRemote(QString userID, QString server);
     
     
     
     //Remove the datastorage from the data storage service
     dss->RemoveDataStorageReference(dataStorageRef);
     
     //Close all editors with this data storage as input
     mitk::DataStorageEditorInput::Pointer dsInput(new mitk::DataStorageEditorInput(dataStorageRef));
     QList<berry::IEditorReference::Pointer> dsEditors =
     this->GetSite()->GetPage()->FindEditors(dsInput, QString(), berry::IWorkbenchPage::MATCH_INPUT);
     
     if (!dsEditors.empty()) {
     QList<berry::IEditorReference::Pointer> editorsToClose = dsEditors;
     this->GetSite()->GetPage()->CloseEditors(editorsToClose, false);
     }
     
     } catch (std::exception& e) {
     
     MITK_ERROR << "Exception caught during closing project: " << e.what();
     QMessageBox::warning(NULL, "Error", QString("An error occurred during Close Project: %1").arg(e.what()));
     }//_try
     
     //Clear project directory
     directory.clear();
     cmd.reset();
     
     */
    
}



