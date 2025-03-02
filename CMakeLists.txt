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

# Define include directories for obj and interface
set(INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Common
        ${CMAKE_CURRENT_SOURCE_DIR}/src/IO
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Mesh
        ${CMAKE_CURRENT_SOURCE_DIR}/src/MeshFeature
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Testing
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Transformation
)

# Define object lib
set(OBJECT_LIBRARY_NAME CommonUtilityObj)

add_library(${OBJECT_LIBRARY_NAME}
        OBJECT
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
)

target_include_directories(${OBJECT_LIBRARY_NAME}
        PRIVATE
        ${INCLUDE_DIRS}
)

target_link_libraries(${OBJECT_LIBRARY_NAME}
        PRIVATE
        ${VTK_LIBRARIES})


# Create the interface
set(INTERFACE_LIBRARY_NAME CommonUtility)
add_library(${INTERFACE_LIBRARY_NAME} INTERFACE)

target_include_directories(${INTERFACE_LIBRARY_NAME}
        INTERFACE
        ${INCLUDE_DIRS}
        #more exposed include directories
        ${CMAKE_CURRENT_SOURCE_DIR}/src/CollisionDetection
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ImageData
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Polygon
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Rendering
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Visualization
)

target_link_libraries(${INTERFACE_LIBRARY_NAME}
        INTERFACE
        $<TARGET_OBJECTS:${OBJECT_LIBRARY_NAME}>
        ${VTK_LIBRARIES}
)


# Add the executable
add_executable(${PROJECT_NAME} MACOSX_BUNDLE
        main.cpp
        Test/MeshFeatureTest.cpp
)

target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${VTK_LIBRARIES}
        ${INTERFACE_LIBRARY_NAME}
)

# vtk_module_autoinit is needed
vtk_module_autoinit(
        TARGETS ${PROJECT_NAME}
        MODULES ${VTK_LIBRARIES}
)
