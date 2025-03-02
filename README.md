# CommonUtility

<br/>**Overview** 
<br/>Common utility functions for VTK projects. 

<br/>**Requirements** 
<br/>VTK 9.2+

<br/>**Usage**
<br/>Include this project as a submodule for your project. Refer to [git submodule doc](https://git-scm.com/docs/git-submodule) for more details.
<br/>Once you added the submodule into your project, in CMakeLists.txt of your project, do
```
# Add the submodule as a subdirectory
add_subdirectory(CommonUtility)

#link to submodule
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        CommonUtility
)
```
