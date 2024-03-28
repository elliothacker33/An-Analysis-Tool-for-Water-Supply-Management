#include "Vertex.h"
#include "Edge.h"
#include "Graph.h"

// Standard Library Headers
#include <algorithm>  /**< Standard C++ algorithm library */

bool Vertex::isVisited() const {
    return visited;
}

/**
 * @brief This function allows changing the state of a vertex,to visited or not visited.
 * @param isVisited
 */
void Vertex::setVisited(bool isVisited) {
    this->visited = isVisited;
}

/**
 * @brief This function returns if the vertex is working or not.
 * @return enabled ("True" or "False")
 */
bool Vertex::isEnabled() const {
    return enabled;
}

/**
 * @brief This function allows to change the state of a vertex, to working or not working.
 * @param isEnabled
 */
void Vertex::setEnabled(bool isEnabled) {
    this->enabled = isEnabled;
}

/**
 * @brief Return the last visited edge during a dfs/bfs.
 * @return path
 */
Edge *Vertex::getPath() const {
    return path;
}

/**
 * @brief Set path is used in dfs and bfs. It represents the edge that was visited last.
 * @param edge
 */
void Vertex::setPath(Edge *edge) {
    path = edge;
}

/**
 * @brief Get the number of adjacent edges
 * @return number of adjacent edges
 */
int Vertex::getOutDegree() const {
    return outDegree;
}

/**
 * @brief Change the current number of adjacent edges, after adding a new edge.
 * @param outDegree
 */
void Vertex::setOutDegree(const int outDegree) {
    this->outDegree = outDegree;
}

/**
 * @brief Get the number of incoming edges
 * @return number of incoming edges
 */
int Vertex::getInDegree() const {
    return inDegree;
}

/**
 * Change the current number of incoming edges, after adding a new edge.
 * @param inDegree
 */
void Vertex::setInDegree(const int inDegree) {
    this->inDegree = inDegree;
}

/**
 * @brief Add a incoming edge
 * @param edge
 * @return boolean value representing if the addition of the edge was successfull or not.
 */
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

/**
 * @brief Add a edge to the vertex
 * @param destination Destination vertex
 * @param capacity Capacity of the edge
 * @param type Type of the edge ("normal" or "residual")
 * @return a pointer to the edge added
 */
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

/**
 * @brief Remove the edge from a vertex
 * @param edge
 */
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

/**
 * @brief Remove edge from destination vertex
 * @param edge
 */
void Vertex::removeEdgeIncoming(const Edge* edge) {
    incoming.erase(remove_if(incoming.begin(), incoming.end(), [&](const Edge* e) {
        if (e->getOrigin() == edge->getOrigin() && e->getDest() == edge->getDest()) {
            return true;
        }
        return false;
    }), incoming.end());
    this->inDegree--;
}

/**
 * @brief Return vector of adjacent edges
 * @return vector of adjacent edges
 */
vector<Edge *> Vertex::getAdj() {
    return adj;
}

/**
 * @brief Return vector of incoming edges
 * @return vector of incoming edges
 */
vector<Edge *> Vertex::getIncoming() {
    return incoming;
}

/**
 * Destrunctor of vertex. Deletes the vertex and the edges associated with him.
 */
Vertex::~Vertex() {
    for (const Edge* edge : adj) {
        delete edge;
    }
    adj.clear();
    incoming.clear();
}


/**
 * @brief Gets the type of the city.
 *
 * This function returns the type of the city, which is represented by the character 'C'.
 *
 * @return The character representing the type of the city ('C').
 */
char City::getType() const {
    return 'C';
}

/**
 * @brief Gets the type of the station.
 *
 * This function returns the type of the station, which is represented by the character 'S'.
 *
 * @return The character representing the type of the station ('S').
 */
char Station::getType() const {
    return 'S';
}

/**
 * @brief Gets the type of the reservoir.
 *
 * This function returns the type of the reservoir, which is represented by the character 'R'.
 *
 * @return The character representing the type of the reservoir ('R').
 */
char Reservoir::getType() const {
    return 'R';
}

/**
 * @brief Gets the name of the city.
 *
 * This function returns the name of the city.
 *
 * @return The name of the city.
 */
string City::getName() const {
    return this->name;
}

/**
 * @brief Gets the code of the city.
 *
 * This function returns the code of the city.
 *
 * @return The code of the city.
 */
string City::getCode() const {
    return this->code;
}

/**
 * @brief Gets the demand of the city.
 *
 * This function returns the demand of the city.
 *
 * @return The demand of the city.
 */
int City::getDemand() const {
    return this->demand;
}

/**
 * @brief Gets the population of the city.
 *
 * This function returns the population of the city.
 *
 * @return The population of the city.
 */
int City::getPopulation() const {
    return this->population;
}

/**
 * @brief Gets the ID of the city.
 *
 * This function returns the ID of the city.
 *
 * @return The ID of the city.
 */
int City::getId() const {
    return this->id;
}

/**
 * @brief Gets the code of the city.
 *
 * This function returns the code of the city.
 *
 * @return The code of the city.
 */

string Station::getCode() const{
    return this->code;
}

/**
 * @brief Gets the ID of the city.
 *
 * This function returns the ID of the city.
 *
 * @return The ID of the city.
 */
int Station::getId() const{
    return this->id;
}

/**
 * @brief Gets the name of the city.
 *
 * This function returns the name of the city.
 *
 * @return The name of the city.
 */

string Reservoir::getName() const{
    return this->name;
}

/**
 * @brief Gets the code of the city.
 *
 * This function returns the code of the city.
 *
 * @return The code of the city.
 */
string Reservoir::getCode() const{
    return this->code;
}

/**
 * @brief Gets the municipality of the city.
 *
 * This function returns the ID of the city.
 *
 * @return The municipality of the city.
 */
string Reservoir::getMunicipality() const{
    return this->municipality;
}

/**
 * @brief Gets the ID of the city.
 *
 * This function returns the ID of the city.
 *
 * @return The ID of the city.
 */
int Reservoir::getId() const{
    return this->id;
}

/**
 * @brief Gets the max delivery of the city.
 *
 * This function returns the ID of the city.
 *
 * @return The max delivery of the city.
 */
int Reservoir::getMaxDelivery() const{
    return this->maxDelivery;
}
