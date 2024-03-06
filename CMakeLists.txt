cmake_minimum_required(VERSION 3.26)
project(projectDA)

set(CMAKE_CXX_STANDARD 17)

add_executable(projectDA main.cpp
        "Custom Classes/Vertexes/reservoir.h"
        Graph/graph.h
        "Custom Classes/Vertexes/station.h"
        "Custom Classes/Vertexes/city.h"
        "Custom Classes/pipes.h"
        "Custom Classes/vertexParent.h"
        "Custom Classes/element.h")
