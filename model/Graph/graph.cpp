
#include "graph.h"

#include <algorithm>
#include <limits.h>
using namespace std;

// Constructors for Edges of the graph

// Super Source and Super Sink infinity edges

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


Vertex* Edge::getDest() const{
    return this->dest;
}

Vertex* Edge::getOrigin() const{
    return this->orig;
}

void Edge::setReverseEdge(Edge* e){
    this->reverse = e;
}

Edge* Edge::getReverseEdge() const{
    return this->reverse;
}
void Edge::setFlow(const int flow){
    this->flow = flow;
}
int Edge::getFlow() const{
    return this->flow;
}

// Methods for Vertex;
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


void Vertex::setPath(Edge *e) {
    path = e;
}
Edge *Vertex::getPath() const {
    return path;
}

// Tested
int Vertex::getOutDegree() const {
    return outDegree;
}
// Tested
void Vertex::setOutDegree(const int outDegree) {
    this->outDegree = outDegree;
}

// Tested
int Vertex::getInDegree() const {
    return inDegree;
}

// Tested
void Vertex::setInDegree(const int inDegree) {
    this->inDegree = inDegree;
}

// Tested
bool Vertex::addIncoming(Edge* e) {
    const string codeOrig = Graph::getCode(e->getOrigin());
    const string codeDest = Graph::getCode(e->getDest());

    for (const auto edg : this->incoming) {
        if (codeOrig == Graph::getCode(edg->getOrigin()) && codeDest == Graph::getCode(edg->getDest())) {
            return false;
        }
    }
    this->incoming.push_back(e);
    this->inDegree++;
    return true;
}

// Tested
Edge* Vertex::addEdge(Vertex *t, const int capacity,const string& type) {
    // If already has return false and dont Increase outDegree;
    const string code = Graph::getCode(t);
    for (const auto e : this->adj) {
        if (Vertex* dest = e->getDest();Graph::getCode(dest) == code) {
            return nullptr;
        }
    }

    auto* edge = new Edge(this,t,capacity,type);
    this->adj.push_back(edge);
    outDegree++;
    t->addIncoming(edge); // Add edge to incoming of dest
    return edge;
}
void Vertex::removeEdge(const Edge* e) {
    adj.erase(remove_if(adj.begin(), adj.end(), [&](const Edge* edge) {
        if (edge->getOrigin() == e->getOrigin() && edge->getDest() == e->getDest()) {
            e->getDest()->removeEdgeIncoming(e);
            return true;
        }
        return false;
    }), adj.end());

    this->outDegree--;
    e->getDest()->removeEdgeIncoming(e);

}

void Vertex::removeEdgeIncoming(const Edge* e) {
    incoming.erase(remove_if(incoming.begin(), incoming.end(), [&](const Edge* edge) {
        if (edge->getOrigin() == e->getOrigin() && edge->getDest() == e->getDest()) {
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
// Good
Vertex::~Vertex() {
    // Delete adjacent edges connected to the vertex
    for (const Edge* edge : adj) {
        delete edge;
    }
    adj.clear();
    incoming.clear();
}
char City::getType() const{
    return 'C';
}
char Station::getType() const{
    return 'S';
}
char Reservoir::getType() const{
    return 'R';
}

// Methods for City
string City::getName() const{
    return this->name;
}
string City::getCode() const{
    return this->code;
}
int City::getDemand() const{
    return this->demand;
}
int City::getPopulation() const{
    return this->population;
}
int City::getId() const{
    return this->id;
}

// Methods for Station
string Station::getCode() const{
    return this->code;
}
int Station::getId() const{
    return this->id;
}

// Methods for Reservoir
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

// Methods For Graph

// Tested
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

void Graph::removeEdge(const Edge *e) {
    e->getOrigin()->removeEdge(e);
    delete e;
}

// Tested
vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}
// Tested
int Graph::getNumberOfVertexes() const {
    return n;
}
// Tested
Edge* Graph::addEdge(Vertex *orig, Vertex *dest, const int capacity,const string& type) {
    Edge* result =orig->addEdge(dest,capacity,type);
    return result;
}
// Tested
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
// Good
Graph::~Graph() {
    while (!vertexSet.empty()) {
        auto it = vertexSet.begin();
        removeVertex(*it);
    }
}


