cmake_minimum_required(VERSION 3.10)
project(aclj)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(
  CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -g  -W -Wall -pedantic"
)

# Directories
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Source files
file(GLOB_RECURSE SRCS ${SRC_DIR}/*.cpp)

# Include directories
include_directories(${INCLUDE_DIR})

# Add executable
add_executable(aclj ${SRCS})


# Set output directory for executable
set_target_properties(aclj  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

