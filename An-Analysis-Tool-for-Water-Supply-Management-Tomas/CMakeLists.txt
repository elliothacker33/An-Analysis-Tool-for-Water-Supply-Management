cmake_minimum_required(VERSION 3.26)
project(projectDA)

set(CMAKE_CXX_STANDARD 17)

add_executable(projectDA main.cpp
        "model/Graph/graph.h"
        "controller/manager.h"
        "view/menu.h"
        "view/menu.cpp"
        "model/Graph/graph.cpp"
        controller/manager.cpp)
