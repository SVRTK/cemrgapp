option(BUILD_CemrgCMDApps "Build ALL command-line apps for Cemrg Module" OFF)

if(BUILD_CemrgCMDApps)
  mitkFunctionCreateCommandLineApp(
    NAME CemrgTemplateApp
    DEPENDS MitkCemrgAppModule
    CPP_FILES CemrgAppCmdTemplate.cpp
  )
  mitkFunctionCreateCommandLineApp(
    NAME CemrgSimpleRegistration
    DEPENDS MitkCemrgAppModule
  )
endif()
