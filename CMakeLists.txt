cmake_minimum_required(VERSION 3.27)
project(watersupply)

set(CMAKE_CXX_STANDARD 17)

add_executable(watersupply src/main.cpp
        src/model/Graph.h
        src/controller/manager.h
        src/controller/manager.cpp
        src/model/Graph.cpp
        src/view/menu.h
        src/view/menu.cpp
        src/model/Edge.cpp
        src/model/Edge.h
        src/model/Vertex.cpp
        src/model/Vertex.h)

