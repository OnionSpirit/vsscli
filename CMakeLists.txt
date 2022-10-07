cmake_minimum_required(VERSION 3.19)
project(vsscli)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)

include(FetchContent)

file(GLOB SRC *.cpp)
file(GLOB HDR *.h)

add_library(${PROJECT_NAME} STATIC ${SRC} ${HDR})




