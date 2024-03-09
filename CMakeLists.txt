cmake_minimum_required(VERSION 3.26)
project(projectDA)

set(CMAKE_CXX_STANDARD 17)

add_executable(projectDA main.cpp
        "model/Custom Classes/Vertexes/reservoir.h"
        model/Graph/graph.h
        "model/Custom Classes/Vertexes/station.h"
        "model/Custom Classes/Vertexes/city.h"
        "model/Custom Classes/pipes.h"
        "model/Custom Classes/vertexParent.h"
        "model/Custom Classes/element.h"
        controller/manager.h
        view/menu.h)
