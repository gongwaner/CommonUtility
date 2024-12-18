cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)

if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES arm64)
endif ()

project(VTKCommonUtility)

find_package(VTK COMPONENTS
        CommonColor
        CommonCore
        CommonComputationalGeometry
        CommonDataModel
        FiltersCore
        FiltersGeneral
        IOGeometry
        IOImage
        IOLegacy
        IOPLY
        IOXML
        InteractionImage
        InteractionStyle
        InteractionWidgets
        RenderingAnnotation
        RenderingContextOpenGL2
        RenderingCore
        RenderingFreeType
        RenderingGL2PSOpenGL2
        RenderingOpenGL2
)

if (NOT VTK_FOUND)
    message(FATAL_ERROR "VTKCommonUtility: Unable to find the VTK build folder.")
endif ()

# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")

add_executable(${PROJECT_NAME} MACOSX_BUNDLE main.cpp
        CollisionDetection/CollisionDetectionUtil.cpp
        CollisionDetection/TriTriIntersection.cpp
        Common/Color.h
        Common/CommonUtil.cpp
        IO/IOUtil.cpp
        Mesh/GeometricObjectUtil.cpp
        Mesh/MeshUtil.cpp
        MeshFeature/MeshFeature.cpp
        MeshFeature/MeshFeatureUtil.cpp
        Polygon/PolygonUtil.cpp
        Rendering/RenderingUtil.cpp
        Test/MeshFeatureTest.cpp
        Test/TestUtil.cpp
        Transformation/TransformUtil.cpp
        Visualization/VisualizationUtil.cpp
)

target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${VTK_LIBRARIES}
)

# vtk_module_autoinit is needed
vtk_module_autoinit(
        TARGETS ${PROJECT_NAME}
        MODULES ${VTK_LIBRARIES}
)
