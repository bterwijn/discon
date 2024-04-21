cmake_minimum_required(VERSION 3.10)  # Specify the minimum version of CMake

project(Discon)  # Name your project

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add an executable with one source file
add_executable(discon discon.cpp)

# Set full optimization using -O3
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-O3" COMPILER_SUPPORTS_O3)
if(COMPILER_SUPPORTS_O3)
    target_compile_options(discon PRIVATE -O3)
    message(STATUS "Using -O3 on compiler ${CMAKE_CXX_COMPILER}.")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no -O3 support.")
endif()
