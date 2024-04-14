cmake_minimum_required(VERSION 3.20.0)

#################################  How to use this CMakeList file ##################################
# 1. If you are not using the CMake to compile, add corresponding souce files into your project.
# 2. If you are using CMake to manage your project, Please read the following steps. Refer to the
#    example's CMakeLists.txt.
#    a. Select the os and the screen driver in your CMakeList.txt by setting the global variable
#       `VLGUI_PORT_OS_SELECT`. The source code is in the folder ./port/os. The Cmake will chose 
#       which *.c will be used automatically. if want to use your own driver and os, you can add 
#       the source code in the corresponding path. Then change the variables said above.
#    b. Include this Cmake file via `include(${ROOT_PAT}/vlGuiProj.cmake)` after setting the variable.
#    c. Add the `VLGUI_SRCS` into your target
####################################################################################################
# set(VLGUI_PORT_OS_SELECT        linux)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

message("#####################################################################################")
message("####\t\t\t\tvlGui Version: ${PROJECT_VERSION}\t\t\t\t#####")
message("#####################################################################################\n")

set(VLGUI_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR})

include_directories(
    ${VLGUI_ROOT_PATH}/inc
)

if (NOT DEFINED VLGUI_PORT_OS_SELECT)
    set(VLGUI_PORT_OS_SELECT linux)
endif()

AUX_SOURCE_DIRECTORY(${VLGUI_ROOT_PATH}/src             _VLGUI_SRC_DIR)
AUX_SOURCE_DIRECTORY(${VLGUI_ROOT_PATH}/src/fonts       _VLGUI_FONTS_DIR)
AUX_SOURCE_DIRECTORY(${VLGUI_ROOT_PATH}/port/driver     _VLGUI_DRIVER_DIR)

list(APPEND VLGUI_SRCS
    ${_VLGUI_SRC_DIR}
    ${_VLGUI_FONTS_DIR}
    # Include the code of porting layer
    ${_VLGUI_DRIVER_DIR}
    ${VLGUI_ROOT_PATH}/port/vlGui_port.c
    ${VLGUI_ROOT_PATH}/port/os/vlGui_${VLGUI_PORT_OS_SELECT}.c
)
