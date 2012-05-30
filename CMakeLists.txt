cmake_minimum_required (VERSION 2.8)

project (ResInsight)

set (VIZ_MODULES_FOLDER_NAME VisualizationModules)


# Setup the main platform defines
#-----------------------------------------------------
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(-DCVF_LINUX)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-DCVF_OSX)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        # Linux specific code
        set(CMAKE_CXX_FLAGS 		"-DCVF_LINUX -DUSE_ECL_LIB -pipe -Wextra -Woverloaded-virtual -Wformat")
        set(CMAKE_CXX_FLAGS_DEBUG 	"-ggdb -g3 -O0 -DDEBUG -D_DEBUG")
        set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNO_DEBUG")
        #set(CMAKE_EXE_LINKER_FLAGS  "-Xlinker -rpath .")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
#        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g1")
ELSE()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp")
ENDIF() 


################################################################################
# Defines
################################################################################

set(CMAKE_MAJOR_VERSION 0)
set(CMAKE_MINOR_VERSION 4)
set(CMAKE_PATCH_VERSION 8)

set(PRODUCTVER ${CMAKE_MAJOR_VERSION},${CMAKE_MINOR_VERSION},0,${CMAKE_PATCH_VERSION})
set(STRPRODUCTVER ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION})



################################################################################
# Qt
################################################################################
set (QT_COMPONENTS_REQUIRED QtCore QtGui QtMain QtOpenGl QtNetwork)
find_package (Qt4 COMPONENTS ${QT_COMPONENTS_REQUIRED} REQUIRED)
include (${QT_USE_FILE})

# Open GL
find_package( OpenGL )

################################################################################
# CeeViz
################################################################################
add_subdirectory(${VIZ_MODULES_FOLDER_NAME}/LibCore)
add_subdirectory(${VIZ_MODULES_FOLDER_NAME}/LibGeometry)
add_subdirectory(${VIZ_MODULES_FOLDER_NAME}/LibRender)
add_subdirectory(${VIZ_MODULES_FOLDER_NAME}/LibViewing)
add_subdirectory(${VIZ_MODULES_FOLDER_NAME}/LibGuiQt)

include_directories(
    ${LibCore_SOURCE_DIR}
    ${LibGeometry_SOURCE_DIR}
    ${LibRender_SOURCE_DIR}
    ${LibViewing_SOURCE_DIR}
    ${LibGuiQt_SOURCE_DIR}
)


################################################################################
# Ceetron Application Framework
################################################################################

add_subdirectory(cafProjectDataModel)
add_subdirectory(cafPdmCvf)
add_subdirectory(CommonCode)
add_subdirectory(QtPropertyBrowser)
add_subdirectory(cafAnimControl)
add_subdirectory(cafViewer)

include_directories(
    ${QtPropertyBrowser_SOURCE_DIR}
)

add_subdirectory(cafUserInterface)

include_directories(
    ${cafUserInterface_SOURCE_DIR}
    ${cafProjectDataModel_SOURCE_DIR}
    ${cafPdmCvf_SOURCE_DIR}
    ${CommonCode_SOURCE_DIR}
	${cafAnimControl_SOURCE_DIR}
	${cafViewer_SOURCE_DIR}
)

################################################################################
# Installation settings
################################################################################
set (RESINSIGHT_FINAL_NAME "ResInsight-${STRPRODUCTVER}")
set (RESINSIGHT_FINAL_INSTALL_PATH "/usr/${RESINSIGHT_FINAL_NAME}")

set (CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/Install/)
#set (CMAKE_INSTALL_PREFIX ${RESINSIGHT_FINAL_INSTALL_PATH})

################################################################################
# Application 
################################################################################
add_subdirectory(ApplicationCode)
add_subdirectory(OctavePlugin)

################################################################################
# Unit tests
################################################################################
add_subdirectory(ApplicationCode/ReservoirDataModel/ReservoirDataModel_UnitTests)
add_subdirectory(ApplicationCode/FileInterface/FileInterface_UnitTests)
add_subdirectory(ApplicationCode/ModelVisualization/ModelVisualization_UnitTests)

#add_subdirectory(cafTests/cafProjectDataModel_UnitTests)
#add_subdirectory(cafTests/cafTestApplication)



################################################################################
# Installation packaging
################################################################################

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CPACK_GENERATOR TGZ)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CPACK_GENERATOR ZIP)
endif()

set(CPACK_PACKAGE_VERSION_MAJOR ${CMAKE_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${CMAKE_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${CMAKE_PATCH_VERSION})

set(CPACK_PACKAGE_NAME "ResInsight-bin")

include (CPack)
