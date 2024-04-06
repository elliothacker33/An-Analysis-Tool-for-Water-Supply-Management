/**
* @file Edge.cpp
* @brief This file contains the implementation of Edge.
 */

// Project headers
#include "Edge.h"
#include "Vertex.h"


Edge::Edge(Vertex* orig, Vertex* dest,const int capacity,const string& type) {
    this->orig = orig;
    this->dest = dest;
    this->capacity = capacity;
    this->type = type;
}

int Edge::getCapacity() const{
    return this->capacity;
}

const string& Edge::getType() const {
    return type;
}

bool Edge::isEnabled() const {
    return enabled;
}

void Edge::setEnabled(bool isEnabled) {
    this->enabled = isEnabled;
}

Vertex* Edge::getDest() const {
    return this->dest;
}

Vertex* Edge::getOrigin() const {
    return this->orig;
}

void Edge::setReverseEdge(Edge* edge){
    this->reverse = edge;
}

Edge* Edge::getReverseEdge() const{
    return this->reverse;
}

void Edge::setFlow(const int flow) {
    this->flow = flow;
}

int Edge::getFlow() const {
    return this->flow;
}
