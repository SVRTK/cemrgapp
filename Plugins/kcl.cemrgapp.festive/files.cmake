set(SRC_CPP_FILES

)

set(INTERNAL_CPP_FILES
  kcl_cemrgapp_festive_Activator.cpp
  FestiveView.cpp
)

set(UI_FILES
  src/internal/FestiveViewControls_new.ui
  src/internal/FestiveViewUILogin.ui
  src/internal/FestiveViewUIReconst.ui
  src/internal/FestiveViewUIReconstMulti.ui
  src/internal/FestiveViewUIReconstSVR.ui
  src/internal/FestiveViewUIReconstDSVR.ui
  src/internal/FestiveViewUIPatientInfo.ui
  src/internal/FestiveViewUIServerInfo.ui
  src/internal/FestiveViewUIReorient.ui
  src/internal/FestiveViewUIRegister.ui
  src/internal/FestiveViewUIAtlas.ui
  src/internal/FestiveViewUIAtlasBody.ui
  src/internal/FestiveViewUIReorientReference.ui
  src/internal/FestiveViewUILabelPropagation.ui
)

set(MOC_H_FILES
  src/internal/kcl_cemrgapp_festive_Activator.h
  src/internal/FestiveView.h
)

# list of resource files which can be used by the plug-in
# system without loading the plug-ins shared library,
# for example the icon used in the menu and tabs for the
# plug-in views in the workbench
set(CACHED_RESOURCE_FILES
  resources/icon.xpm
  plugin.xml
)

# list of Qt .qrc files which contain additional resources
# specific to this plugin
set(QRC_FILES

)

set(CPP_FILES )

foreach(file ${SRC_CPP_FILES})
  set(CPP_FILES ${CPP_FILES} src/${file})
endforeach(file ${SRC_CPP_FILES})

foreach(file ${INTERNAL_CPP_FILES})
  set(CPP_FILES ${CPP_FILES} src/internal/${file})
endforeach(file ${INTERNAL_CPP_FILES})
