
#include "graph.h"
using namespace std;

// Constructors for Edges of the graph

// Super Source and Super Sink infinity edges

Edge::Edge(Vertex* orig, Vertex* dest,const int capacity) {
    this->orig = orig;
    this->dest = dest;
    this->capacity = capacity;
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
    t->addIncoming(edge);
    return true;
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

bool Graph::addVertex(Vertex *v) {
    if (const string code = getCode(v); code == "") {
        // Change this later
        vertexSet.push_back(v);
        n++;
        return true;
    }
    return false;

}
vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}
int Graph::getNumberOfVertexes() const {
    return n;
}
bool Graph::addEdge(Vertex *orig, Vertex *dest, const int capacity) {
    const bool result =orig->addEdge(dest,capacity);
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
    exit(-1);
}

bool Graph::removeVertex(Vertex *v) {
    if (const auto city = dynamic_cast<City*>(v)) {
        const string code = city->getCode();
        for (auto it = vertexSet.begin(); it != vertexSet.end(); ) {
            if (const auto cityInSet = dynamic_cast<City*>(*it)) {
                if (cityInSet->getCode() == code) {
                    it = vertexSet.erase(it);
                    return true;
                } else {
                    ++it;
                }
            } else {
                ++it;
            }
        }
    } else if (const auto reservoir = dynamic_cast<Reservoir*>(v)) {
        const string code = reservoir->getCode();
        for (auto it = vertexSet.begin(); it != vertexSet.end(); ) {
            if (const auto reservoirInSet = dynamic_cast<Reservoir*>(*it)) {
                if (reservoirInSet->getCode() == code) {
                    it = vertexSet.erase(it);
                    return true;
                } else {
                    ++it;
                }
            } else {
                ++it;
            }
        }
    } else if (const auto station = dynamic_cast<Station*>(v)) {
        const string code = station->getCode();
        for (auto it = vertexSet.begin(); it != vertexSet.end(); ) {
            if (const auto stationInSet = dynamic_cast<Station*>(*it)) {
                if (stationInSet->getCode() == code) {
                    it = vertexSet.erase(it);
                    return true;
                } else {
                    ++it;
                }
            } else {
                ++it;
            }
        }
    }

    delete v;
    return false;
}
