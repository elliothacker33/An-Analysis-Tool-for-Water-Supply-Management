/**
* @file Graph.cpp
* @brief This file contains the implementation of Graph.
 */

// Include the graph.h header file for graph-related functionality.
#include "Graph.h"


bool Graph::addVertex(Vertex *v) {
    for (const auto ve : vertexSet) {
        if (getCode(ve) == getCode(v)) {
            return false;
        }
    }
    vertexSet.push_back(v);
    n++;
    return true;
}


void Graph::removeVertex(Vertex* v) {
    auto it = vertexSet.begin();
    while (it != vertexSet.end()) {
        if (getCode(*it) == getCode(v)) {
            vertexSet.erase(it);
            n--;
            delete v;
            return;
        }
        ++it;
    }
}

void Graph::resetVisited(){

    for(auto a: getVertexSet()){
        a->setVisited(false);
    }

}


void Graph::removeEdge(const Edge *edge) {
    edge->getOrigin()->removeEdge(edge);
    delete edge;
}


vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}


int Graph::getNumberOfVertexes() const {
    return n;
}


Edge* Graph::addEdge(Vertex *orig, Vertex *dest, const int capacity,const string& type) {
    Edge* result = orig->addEdge(dest,capacity,type);
    return result;
}


string Graph::getCode(Vertex *v) {

    if(const auto city = dynamic_cast<City*>(v)) {
        return city->getCode();
    }
    if (const auto reservoir = dynamic_cast<Reservoir*>(v)) {
        return reservoir->getCode();
    }
    if(const auto station = dynamic_cast<Station*>(v)) {
        return station->getCode();
    }
    cerr << "Error: Vertex class not defined" << endl;
    exit(EXIT_FAILURE);
}


string Graph::getName(Vertex *v) {

    if(const auto city = dynamic_cast<City*>(v)) {
        return city->getName();
    }
    if (const auto reservoir = dynamic_cast<Reservoir*>(v)) {
        return reservoir->getName();
    }

    cerr << "Error: Vertex class not defined" << endl;
    exit(EXIT_FAILURE);
}

Graph::~Graph() {
    while (!vertexSet.empty()) {
        auto it = vertexSet.begin();
        removeVertex(*it);
    }
}


