# CommonUtility

<br/>**Overview** 
<br/>Common utility functions for VTK projects. 

<br/>**Requirements** 
<br/>VTK 9.4.0

<br/>**Usage**
<br/>Include this project as a submodule for your project. Refer to [git submodule doc](https://git-scm.com/docs/git-submodule) for more details.
<br/>Once you added the submodule into your project, in CMakeLists.txt of your project, do
```
#add the submodule as a subdirectory
add_subdirectory(CommonUtility)

#include submodule
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}
)

#link to submodule
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        CommonUtility
)
```
