cmake_minimum_required(VERSION 2.8.5)

project(vlGui)

set(ROOT_PAT ${PROJECT_SOURCE_DIR}/../..)

include(${ROOT_PAT}/proj.cmake)


set(PORT ${ROOT_PAT}/port/driver/simu_GTK/simulated_screen.c)


set(CMAKE_C_FLAGS "-O0 -ggdb") #'pkg-config gtk+-2.0 --cflags --libs gthread-2.0'
# set(COMPILE_FLAGS "-lpthread")
# set(LINK_FLAGS "-lpthread")


AUX_SOURCE_DIRECTORY(./ DIR_MAIN)

add_executable(${PROJECT_NAME} 
${SRC}
${PORT}
${DIR_MAIN}
"main.c"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
                        COMPILE_FLAGS "-pthread"
                        LINK_FLAGS "-pthread")

#check packet config
find_package(PkgConfig REQUIRED)
pkg_check_modules(gtkpkt REQUIRED IMPORTED_TARGET gtk+-2.0)
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::gtkpkt)
