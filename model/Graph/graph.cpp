
/**
 * @file graph.cpp
 * @brief This file contains the graph functions for the application.
 */

// Include the graph.h header file for graph-related functionality.
#include "graph.h"  /**< Provides functionality related to graphs. */

// Standard Library Headers
#include <algorithm>  /**< Standard C++ algorithm library */

/**
 * @brief Constructs an Edge object with the given parameters.
 *
 * This constructor initializes an Edge object with the specified origin vertex,
 * destination vertex, capacity, and type. This serves to represnt the pipes in the model specified.
 *
 * @param orig Pointer to the origin vertex of the edge.
 * @param dest Pointer to the destination vertex of the edge.
 * @param capacity The capacity of the edge.
 * @param type The type of the edge (e.g., "directed", "undirected").
 */
Edge::Edge(Vertex* orig, Vertex* dest,const int capacity,const string& type) {
    this->orig = orig;
    this->dest = dest;
    this->capacity = capacity;
    this->type = type;
}

/**
 * @brief Returns the capacity of the edge.
 * @return capacity
 */
int Edge::getCapacity() const{
    return this->capacity;
}

/**
 * @brief Returns the type of the edge.
 * @return type of edge -> "normal" or "residual"
 */
const string& Edge::getType() const {
    return type;
}

/**
 * @brief Returns a boolean value representing the working status of a pipe.
 * @return enabled ("True" or "False")
 */
bool Edge::isEnabled() const {
    return enabled;
}

/**
 * @brief Sets the enabled state of the edge.
 *
 * This function allows setting the enabled state of the edge. When an edge is enabled,
 * it is considered active or usable in the context of the graph.
 *
 * @param isEnabled A boolean value indicating whether the edge should be enabled or not.
 *                  - true: Enable the edge.
 *                  - false: Disable the edge.
 */
void Edge::setEnabled(bool isEnabled) {
    this->enabled = isEnabled;
}


/**
 * @brief Retrieves the destination vertex of the edge.
 *
 * This function returns a pointer to the destination vertex of the edge.
 * The destination vertex is the vertex to which the edge leads.
 *
 * @return A pointer to the destination vertex.
 */
Vertex* Edge::getDest() const {
    return this->dest;
}

/**
 * @brief Retrieves the origin vertex of the edge.
 *
 * This function returns a pointer to the origin vertex of the edge.
 * The origin vertex is the vertex from which the edge originates.
 *
 * @return A pointer to the origin vertex.
 */
Vertex* Edge::getOrigin() const {
    return this->orig;
}

/**
 * This function allows to set the reverse edge (Original - X-Y, Reverse- Y-X)
 * @param edge
 */

void Edge::setReverseEdge(Edge* edge){
    this->reverse = edge;
}

/**
 * @brief This function allows to get the reverse edge.
 * @return reverse edge
 */
Edge* Edge::getReverseEdge() const{
    return this->reverse;
}

/**
 * @brief Sets the flow of the edge.
 *
 * This function allows changing the flow of an edge in a network flow graph.
 * It updates the flow value of the edge to the specified value.
 *
 * @see Manager::maxFlow Max flow algorithm implemented in the Manager class.
 *
 * @param flow The new flow value to be set for the edge.
 */
void Edge::setFlow(const int flow) {
    this->flow = flow;
}

/**
 * @brief Retrieves the flow of the edge.
 *
 * This function returns the current flow value of the edge.
 *
 * @return The current flow value of the edge.
 */
int Edge::getFlow() const {
    return this->flow;
}

/**
 * @brief Returns the value if a vertex is visited or not.
 * @return visited ("True" or "False")
 */
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

/**
 * @brief  Add vertex
 * This function adds a vertex to the graph
 * @param v Vertex pointer
 * @return If vertex was added returns true else false
 */
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

/**
 * @brief Remove vertex
 * This functions removes a vertex of the graph
 * @param v Vertex pointer
 */
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

/**
 * @brief Remove edge interface
 * This functions removes a edge of the graph
 * @param edge Edge pointer
 */
void Graph::removeEdge(const Edge *edge) {
    edge->getOrigin()->removeEdge(edge);
    delete edge;
}

/**
 * @brief Returns the set of vertexes
 * @return vector of vertex pointers
 */
vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}

/**
 * @brief Returns the number of vertexes
 * @return number of vertexes
 */
int Graph::getNumberOfVertexes() const {
    return n;
}

/**
 * @brief Add edge to a vertex on the graph
 * @param orig Origin vertex
 * @param dest Dest vertex
 * @param capacity Capacity of edge
 * @param type "normal" or "residual"
 * @return Edge pointer
 */
Edge* Graph::addEdge(Vertex *orig, Vertex *dest, const int capacity,const string& type) {
    Edge* result =orig->addEdge(dest,capacity,type);
    return result;
}

/**
 * @brief Gets the code associated with the given vertex.
 *
 * This function retrieves the code associated with the given vertex.
 * If the vertex is of type City, Reservoir, or Station, it returns the respective code.
 * Otherwise, it prints an error message and exits the program.
 *
 * @param v Pointer to the vertex.
 * @return The code associated with the vertex.
 */
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

/**
 * @brief Gets the name associated with the given vertex.
 *
 * This function retrieves the name associated with the given vertex.
 * If the vertex is of type City or Reservoir, it returns the respective name.
 * Otherwise, it prints an error message and exits the program.
 *
 * @param v Pointer to the vertex.
 * @return The name associated with the vertex.
 */
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

/**
 * @brief Destructor for the Graph class.
 *
 * This destructor cleans up the memory allocated for the vertices in the graph.
 * It removes all vertices from the vertex set and deletes them.
 */
Graph::~Graph() {
    while (!vertexSet.empty()) {
        auto it = vertexSet.begin();
        removeVertex(*it);
    }
}


