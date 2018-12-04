cmake_minimum_required(VERSION 3.3)
project(fp CXX)
include(.ipd/cmake/CMakeLists.txt)

set(CMAKE_CXX_STANDARD 14)

add_executable(fp
        README.md src/spaceInvaders.cpp)

add_program(model
        src/model.h)
target_link_libraries(model ge211)
