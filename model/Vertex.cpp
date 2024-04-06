/**
* @file Vertex.cpp
* @brief This file contains the implementation of Vertex.
 */

// Project headers
#include "Vertex.h"
#include "Edge.h"
#include "Graph.h"

// Standard Library Headers
#include <algorithm>

bool Vertex::isVisited() const {
    return visited;
}

void Vertex::setVisited(bool isVisited) {
    this->visited = isVisited;
}

bool Vertex::isEnabled() const {
    return enabled;
}

void Vertex::setEnabled(bool isEnabled) {
    this->enabled = isEnabled;
}

Edge *Vertex::getPath() const {
    return path;
}

void Vertex::setPath(Edge *edge) {
    path = edge;
}

int Vertex::getOutDegree() const {
    return outDegree;
}


void Vertex::setOutDegree(const int outDegree) {
    this->outDegree = outDegree;
}


int Vertex::getInDegree() const {
    return inDegree;
}

void Vertex::setInDegree(const int inDegree) {
    this->inDegree = inDegree;
}

bool Vertex::addIncoming(Edge* edge) {
    const string codeOrig = Graph::getCode(edge->getOrigin());
    const string codeDest = Graph::getCode(edge->getDest());

    for (const auto e : this->incoming) {
        if (codeOrig == Graph::getCode(e->getOrigin()) && codeDest == Graph::getCode(e->getDest())) {
            return false;
        }
    }
    this->incoming.push_back(edge);
    this->inDegree++;
    return true;
}

Edge* Vertex::addEdge(Vertex* destination, const int capacity,const string& type) {
    const string code = Graph::getCode(destination);
    for (const auto e : this->adj) {
        if (Vertex* dest = e->getDest();Graph::getCode(dest) == code) {
            return nullptr;
        }
    }

    auto* edge = new Edge(this,destination,capacity,type);
    this->adj.push_back(edge);
    outDegree++;
    destination->addIncoming(edge);
    return edge;
}

void Vertex::removeEdge(const Edge* edge) {
    adj.erase(remove_if(adj.begin(), adj.end(), [&](const Edge* e) {
        if (e->getOrigin() == edge->getOrigin() && e->getDest() == edge->getDest()) {
            edge->getDest()->removeEdgeIncoming(edge);
            return true;
        }
        return false;
    }), adj.end());

    this->outDegree--;
    edge->getDest()->removeEdgeIncoming(edge);

}

void Vertex::removeEdgeIncoming(const Edge* edge) {
    incoming.erase(remove_if(incoming.begin(), incoming.end(), [&](const Edge* e) {
        if (e->getOrigin() == edge->getOrigin() && e->getDest() == edge->getDest()) {
            return true;
        }
        return false;
    }), incoming.end());
    this->inDegree--;
}

vector<Edge *> Vertex::getAdj() {
    return adj;
}

vector<Edge *> Vertex::getIncoming() {
    return incoming;
}

Vertex::~Vertex() {
    for (const Edge* edge : adj) {
        delete edge;
    }
    adj.clear();
    incoming.clear();
}

char City::getType() const {
    return 'C';
}

char Station::getType() const {
    return 'S';
}

char Reservoir::getType() const {
    return 'R';
}

string City::getName() const {
    return this->name;
}

string City::getCode() const {
    return this->code;
}

int City::getDemand() const {
    return this->demand;
}

int City::getPopulation() const {
    return this->population;
}

int City::getId() const {
    return this->id;
}

string Station::getCode() const{
    return this->code;
}

int Station::getId() const{
    return this->id;
}

string Reservoir::getName() const{
    return this->name;
}

string Reservoir::getCode() const{
    return this->code;
}

string Reservoir::getMunicipality() const{
    return this->municipality;
}

int Reservoir::getId() const{
    return this->id;
}

int Reservoir::getMaxDelivery() const{
    return this->maxDelivery;
}
