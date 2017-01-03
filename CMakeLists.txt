#
# directory structure:
#.
#├── build
#├── CMakeLists.txt
#├── include
#|   └── *.h
#└── source
#    └── *.cpp
#
##################################################
# usage: execute "cmake .." from build directory
##################################################

cmake_minimum_required(VERSION 3.5.2)

#
# project name (=identical to executable name)
#
project(sudoku)

#
# Compiler flags
#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -Wall")

#
# add include directories for header files)
#
include_directories(${PROJECT_SOURCE_DIR}/include)

#Can manually add the sources using the set command as follows:
#set(SOURCES hello.cpp)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES source/*.cpp include/*.h)

add_executable(${PROJECT_NAME} ${SOURCES})

#
# verbose output in Makefile
#
#set(CMAKE_VERBOSE_MAKEFILE on)

#install(TARGETS hello)
