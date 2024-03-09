

// Constructors for Edges of the graph
#include "graph.h"
using namespace std;

// Constructors for Edges of the graph

// Super Source and Super Sink infinity edges

Edge::Edge(int capacity, Vertex* origin, Vertex* destination) {
    this->capacity = capacity;
    this->origin = origin;
    this->dest = destination;
}
Edge::Edge(Edge& other){
    this->capacity = other.capacity;
    this->origin = other.origin;
    this->dest= other.dest;
}
// Getters for Edge.
int Edge::getCapacity() const{
    return this->capacity;
}

Vertex* Edge::getDest() const{
    return this->dest;
}

Vertex* Edge::getOrigin() const{
    return this->origin;
}

void Edge::setReverseEdge(Edge* reverse){
    this->reverse = reverse;
}

Edge* Edge::getReverseEdge() const{
    return this->reverse;
}
void Edge::setFlow(int flow){
    this->flow = flow;
}
double Edge::getFlow() const{
    return this->flow;
}