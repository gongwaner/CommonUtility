cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES arm64)
endif ()

project(VTKCommonUtility)

find_package(VTK COMPONENTS
        CommonColor
        CommonCore
        CommonDataModel
        FiltersCore
        FiltersGeneral
        IOGeometry
        IOImage
        IOLegacy
        IOPLY
        IOXML
        InteractionStyle
        InteractionImage
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
        Common/Color.h
        IO/IOUtil.cpp
        Mesh/MeshFeature.cpp
        Mesh/MeshFeatureUtil.cpp
        Test/MeshFeatureTest.cpp
        CollisionDetection/TriTriIntersection.cpp
        CollisionDetection/CollisionDetectionUtil.cpp
        Transformation/TransformUtil.cpp
        Mesh/MeshUtil.cpp
        Visualization/VisualizationUtil.cpp
        Test/TestUtil.cpp
        Mesh/GeometricObjectUtil.cpp
        Polygon/PolygonUtil.cpp
        Common/CommonUtil.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})

# vtk_module_autoinit is needed
vtk_module_autoinit(
        TARGETS ${PROJECT_NAME}
        MODULES ${VTK_LIBRARIES}
)
