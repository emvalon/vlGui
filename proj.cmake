cmake_minimum_required(VERSION 2.8.5)

project(vlGui C)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)

set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

include_directories(
    ${ROOT_PAT}/inc
    ${ROOT_PAT}/port/inc
)

AUX_SOURCE_DIRECTORY(${ROOT_PAT}/src DIR_SRCS)
AUX_SOURCE_DIRECTORY(${ROOT_PAT}/src/widgets DIR_SRCS)

list(APPEND SRC
    ${DIR_SRCS}
    ${ROOT_PAT}/port/vlGui_port.c
    ${ROOT_PAT}/src/fonts/vlGui_fonts.c
    ${ROOT_PAT}/src/fonts/vlGui_wenquan_9pt.c
)
