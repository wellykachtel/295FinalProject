cmake_minimum_required(VERSION 3.3)
project(invaders CXX)
include(.ipd/cmake/CMakeLists.txt)

add_program(model_test
        test/model_test.cpp
        src/model.cpp
        src/view.cpp
        src/controller.cpp)
target_link_libraries(model_test ge211 catch)

add_program(invaders
        src/spaceInvaders.cpp
        src/model.cpp
        src/view.cpp
        src/controller.cpp)
target_link_libraries(invaders ge211)

