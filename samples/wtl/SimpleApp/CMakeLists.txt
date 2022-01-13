cmake_minimum_required (VERSION 3.15)

project (SimpleApp)

add_definitions(-DWINVER=0x0501 -DUNICODE -D_UNICODE)

set (SimpleApp_SRC_DIR ${WTL_SAMPLES_DIR}/SimpleApp)
set (SimpleApp_HEADER_FILES ${SimpleApp_SRC_DIR}/stdafx.h
                            ${SimpleApp_SRC_DIR}/View.h
                            ${SimpleApp_SRC_DIR}/SimpleApp.h
                            ${SimpleApp_SRC_DIR}/MainFrm.h
                            ${SimpleApp_SRC_DIR}/AboutDlg.h
                            ${SimpleApp_SRC_DIR}/XSDRAWSTLVRML_DataSource.h)
set (SimpleApp_SOURCE_FILES ${SimpleApp_SRC_DIR}/stdafx.cpp
                            ${SimpleApp_SRC_DIR}/View.cpp
                            ${SimpleApp_SRC_DIR}/SimpleApp.cpp
                            ${SimpleApp_SRC_DIR}/XSDRAWSTLVRML_DataSource.cpp)
set (SimpleApp_RESOURCE_DIR ${SimpleApp_SRC_DIR}/res)
set (SimpleApp_RESOURCE_HEADER ${SimpleApp_SRC_DIR}/resource.h)
set (SimpleApp_RESOURCE_FILES ${SimpleApp_RESOURCE_DIR}/toolbar.bmp
                              ${SimpleApp_RESOURCE_DIR}/SimpleApp.ico
                              ${SimpleApp_SRC_DIR}/SimpleApp.rc)

source_group("Source Files" FILES ${SimpleApp_SOURCE_FILES})
source_group("Header Files" FILES ${SimpleApp_HEADER_FILES}
                                  ${SimpleApp_RESOURCE_HEADER})
source_group("Resource Files" FILES ${SimpleApp_RESOURCE_FILES})

add_executable(SimpleApp WIN32 ${SimpleApp_SOURCE_FILES}
                                ${SimpleApp_HEADER_FILES}
                                ${SimpleApp_RESOURCE_FILES}
                                ${SimpleApp_RESOURCE_HEADER})

set_property(TARGET SimpleApp PROPERTY FOLDER "Samples/wtl")

find_program(NUGET nuget)
if(NOT NUGET)
  message (FATAL "CMake could not find the nuget command line tool. Please install it!")
else()
  configure_file(packages.config.in packages.config COPYONLY)
  execute_process(COMMAND 
    $(NUGET) restore package.config -SolutionDirectory ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
endif()

set_property(TARGET SimpleApp PROPERTY VS_PROJECT_IMPORT "${CMAKE_BINARY_DIR}/packages/wtl.10.0.10320/build/native/wtl.targets")

if (SINGLE_GENERATOR)
  install (TARGETS SimpleApp DESTINATION "${INSTALL_DIR_BIN}")
else()
  install (TARGETS SimpleApp
           CONFIGURATIONS Release RelWithDebInfo
           DESTINATION "${INSTALL_DIR_BIN}")
  install (TARGETS SimpleApp
           CONFIGURATIONS Debug
           DESTINATION "${INSTALL_DIR_BIN}d")
endif()

include_directories (${CMAKE_BINARY_DIR}/inc)
set (WTL_USED_LIBS  TKVRML
                          TKSTL
                          TKBRep
                          TKIGES
                          TKShHealing
                          TKSTEP
                          TKSTEPBase
                          TKSTEP209
                          TKSTEPAttr
                          TKXSBase
                          TKBool
                          TKBO
                          TKCAF
                          TKCDF
                          TKernel
                          TKFeat
                          TKFillet
                          TKG2d
                          TKG3d
                          TKGeomAlgo
                          TKGeomBase
                          TKHLR
                          TKMath
                          TKOffset
                          TKOpenGl
                          TKPrim
                          TKService
                          TKTopAlgo
                          TKMesh
                          TKV3d
                          TKMeshVS
                          ${CSF_OpenGlLibs})

target_link_libraries(SimpleApp ${WTL_USED_LIBS})