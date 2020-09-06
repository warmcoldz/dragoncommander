cmake_minimum_required(VERSION 3.12)

project(DragonCommander)

add_definitions(-D_WIN32_WINNT=0x600)
add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

find_package(gRPC CONFIG REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)
