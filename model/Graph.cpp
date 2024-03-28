
/**
 * @file graph.cpp
 * @brief This file contains the graph functions for the application.
 */

// Include the graph.h header file for graph-related functionality.
#include "Graph.h"

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


