#include "Edge.h"
#include "Vertex.h"

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