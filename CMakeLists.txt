cmake_minimum_required(VERSION 3.26)
project(projectDA)

set(CMAKE_CXX_STANDARD 17)

add_executable(projectDA main.cpp
        model/Graph.h
        controller/manager.h
        controller/manager.cpp
        model/Graph.cpp
        view/menu.h
        view/menu.cpp
        model/Edge.cpp
        model/Edge.h
        model/Vertex.cpp
        model/Vertex.h)
