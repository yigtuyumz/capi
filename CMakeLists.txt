# minimum CMake version
cmake_minimum_required(VERSION 3.10)

# name of the project
project(capi VERSION 1.0 LANGUAGES C)

# The C Compiler
set(CMAKE_C_COMPILER "gcc")

# set default build type if not specified by user
# Default types: Debug, Release, RelWithDebInfo and MinSizeRel
# but custom build types can also be defined.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_compile_definitions(CAPI_DEBUG=1)
endif()

# enable output of compile commands during generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -std=c99
# set(CMAKE_C_STANDARD 99)
# initialize the C_STANDARD_REQUIRED property on all targets
# set(CMAKE_C_STANDARD_REQUIRED True)

include(ExternalProject)

# git pull external libraries
ExternalProject_Add(
    json-c
    GIT_REPOSITORY https://github.com/json-c/json-c
    GIT_TAG json-c-0.17
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
            -DBUILD_TESTING=OFF
)


if ("${CMAKE_C_COMPILER_FRONTEND_VARIANT}" STREQUAL "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -std=c99")
endif()

# source files of capi, 
# getting *.c files recursively is not a good idea..
file(GLOB_RECURSE SOURCES ./src/*.c)


# create main project
add_executable(capi ${SOURCES})

# add json-c to the project
add_dependencies(capi json-c)

# include header files and libraries of json-c
ExternalProject_Get_Property(json-c install_dir)
# base path for include directives
include_directories(${CMAKE_BINARY_DIR}/include)
link_directories(${CMAKE_BINARY_DIR}/lib/json-c)


# link json-c to the capi
target_link_libraries(capi ${CMAKE_BINARY_DIR}/lib/libjson-c.so)
