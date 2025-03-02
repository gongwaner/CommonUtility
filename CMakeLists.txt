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
        ImagingHybrid
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

add_executable(${PROJECT_NAME} MACOSX_BUNDLE
        main.cpp
        src/CollisionDetection/CollisionDetectionUtil.cpp
        src/CollisionDetection/TriTriIntersection.cpp
        src/Common/Color.h
        src/Common/CommonUtil.cpp
        src/ImageData/ImageDataUtil.cpp
        src/IO/IOUtil.cpp
        src/Mesh/GeometricObjectUtil.cpp
        src/Mesh/MeshUtil.cpp
        src/MeshFeature/MeshFeature.cpp
        src/MeshFeature/MeshFeatureUtil.cpp
        src/Polygon/PolygonUtil.cpp
        src/Rendering/RenderingUtil.cpp
        src/Testing/TestUtil.cpp
        src/Transformation/TransformUtil.cpp
        src/Visualization/VisualizationUtil.cpp
        Test/MeshFeatureTest.cpp
)

target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/Common
        ${CMAKE_SOURCE_DIR}/src/IO
        ${CMAKE_SOURCE_DIR}/src/Mesh
        ${CMAKE_SOURCE_DIR}/src/MeshFeature
        ${CMAKE_SOURCE_DIR}/src/Testing
        ${CMAKE_SOURCE_DIR}/src/Transformation
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
