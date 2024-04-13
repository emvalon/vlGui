cmake_minimum_required(VERSION 3.20.0)

project(vlGui)

include(../../vlGuiProj.cmake)

set(CMAKE_C_FLAGS "-O0 -ggdb -Werror -Wall -Wextra -Wno-unused-function -Wno-implicit-fallthrough") #'pkg-config gtk+-2.0 --cflags --libs gthread-2.0'

# set(COMPILE_FLAGS "-lpthread")
# set(LINK_FLAGS "-lpthread")
# set(CMAKE_CXX_COMPILER "gcc")

AUX_SOURCE_DIRECTORY(./ DIR_MAIN)
AUX_SOURCE_DIRECTORY(./pictures DIR_PICTURE)

add_executable(${PROJECT_NAME} 
${VLGUI_SRCS}
${DIR_MAIN}
${DIR_PICTURE}
)

# set_target_properties(${PROJECT_NAME} PROPERTIES
#                         COMPILE_FLAGS "-pthread"
#                         LINK_FLAGS "-pthread")

#check packet config
find_package(PkgConfig REQUIRED)
pkg_check_modules(gtkpkt REQUIRED IMPORTED_TARGET gtk+-2.0)
target_link_libraries(${PROJECT_NAME} m PkgConfig::gtkpkt)
