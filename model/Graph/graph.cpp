
#include "graph.h"
using namespace std;

// Constructors for Edges of the graph

// Super Source and Super Sink infinity edges

Edge::Edge(Vertex* orig, Vertex* dest,const int capacity) {
    this->orig = orig;
    this->dest = dest;
    this->capacity = capacity;
}

Edge::~Edge() {
    delete residual;
}

int Edge::getCapacity() const{
    return this->capacity;
}

Vertex* Edge::getDest() const{
    return this->dest;
}

Vertex* Edge::getOrigin() const{
    return this->orig;
}

void Edge::setResidualEdge(Edge* e){
    this->residual = e;
}

Edge* Edge::getResidualEdge() const{
    return this->residual;
}
void Edge::setFlow(const int flow){
    this->flow = flow;
}
double Edge::getFlow() const{
    return this->flow;
}

// Methods for Vertex;
bool Vertex::isVisited() const {
    return visited;
}
void Vertex::setVisited() {
    this->visited = true;
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

    for (const auto e : this->incoming) {
        if (codeOrig == Graph::getCode(e->getOrigin()) && codeDest == Graph::getCode(e->getDest())) {
            return false;
        }
    }
    this->incoming.push_back(e);
    this->inDegree++;
    return true;
}

// Tested
bool Vertex::addEdge(Vertex *t, const int capacity) {
    // If already has return false and dont Increase outDegree;
    const string code = Graph::getCode(t);
    for (const auto e : this->adj) {
        if (Vertex* dest = e->getDest();Graph::getCode(dest) == code) {
            return false;
        }
    }

    auto* edge = new Edge(this,t,capacity);
    this->adj.push_back(edge);
    outDegree++;
    t->addIncoming(edge); // Add edge to incoming of dest
    return true;
}
void Vertex::removeEdge(Edge* e) {
    for (auto it = adj.begin(); it != adj.end(); ++it) {
        if ((*it)->getOrigin() == e->getOrigin() && (*it)->getDest() == e->getDest()) {
            adj.erase(it);
        }
    }
    this->outDegree--;
    e->getDest()->removeEdgeIncoming(e);

}

void Vertex::removeEdgeIncoming(Edge* e) {
    for (auto it = incoming.begin(); it != incoming.end(); ++it) {
        if ((*it)->getOrigin() == e->getOrigin() && (*it)->getDest() == e->getDest()) {
            incoming.erase(it);
        }
    }
    this->inDegree--;
}

vector<Edge *> Vertex::getAdj() {
    return adj;
}

vector<Edge *> Vertex::getIncoming() {
    return incoming;
}

Vertex::~Vertex() {
    // Delete adjacent edges connected to the vertex
    for (const Edge* edge : adj) {
        delete edge;
    }
    adj.clear();

    // Delete incoming edges connected to the vertex
    for (const Edge* edge : incoming) {
        delete edge;
    }
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
    for (auto it = vertexSet.begin(); it != vertexSet.end(); ++it) {
        if (getCode(*it) == getCode(*it)) {
            vertexSet.erase(it);
            n--;
            delete v;
            return;
        }
    }
}

void Graph::removeEdge(Edge *e) {
    e->getOrigin()->removeEdge(e);
    e->getDest()->removeEdge(e);
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
bool Graph::addEdge(Vertex *orig, Vertex *dest, const int capacity) {
    const bool result =orig->addEdge(dest,capacity);
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
    exit(-1);
}

Graph::~Graph() {
    for (const auto v : vertexSet) {
        removeVertex(v);
    }
}


