#ifndef PROJECTDA_GRAPH_H
#define PROJECTDA_GRAPH_H

using namespace std;

#include "Edge.h" // Inculde edges
#include "Vertex.h" // Include vertexes
// Standard Library Headers
#include <string>    // Include for string manipulation functionalities
#include <vector>    // Include for vector container functionalities
#include <iostream>  // Include for standard input/output stream functionalities

/**
 * @class Graph
 * @brief Represents a graph data structure.
 *
 * This class represents a graph data structure consisting of vertices and edges.
 */

class Graph {
public:
    /**
     * @brief Destructor for the Graph class.
     * This destructor cleans up the memory allocated for the graph's vertices and edges.
     */
    ~Graph();

    /**
     * @brief Gets the vector of vertices in the graph.
     *
     * Time Complexity - O(1)
     *
     * @return Vector containing pointers to the vertices in the graph.
     */
    vector<Vertex*> getVertexSet() const;

    /**
     * @brief Gets the number of vertices in the graph.
     *
     * Time Complexity - O(1)
     *
     * @return The number of vertices in the graph.
     */
    int getNumberOfVertexes() const;

    /**
     * @brief Adds a vertex to the graph.
     *
     * Time Complexity - O(V)
     *
     * @param v Pointer to the vertex to be added.
     * @return True if the vertex was successfully added, false otherwise.
     */
    bool addVertex(Vertex* v);

    /**
     * @brief Removes a vertex from the graph.
     *
     * Time Complexity - O(VE)
     *
     * @param v Pointer to the vertex to be removed.
     */
    void removeVertex(Vertex* v);

    /**
     * @brief Removes an edge from the graph.
     *
     * Time Complexity - O(E)
     *
     * @param e Pointer to the edge to be removed.
     */
    void removeEdge(const Edge* e);

    /**
     * @brief Adds an edge between two vertices in the graph.
     *
     * Time Complexity - O(E)
     *
     * @param orig Pointer to the origin vertex of the edge.
     * @param dest Pointer to the destination vertex of the edge.
     * @param capacity Capacity of the edge.
     * @param type Type of the edge.
     * @return Pointer to the added edge.
     */
    Edge* addEdge(Vertex* orig, Vertex* dest, int capacity, const string& type);

    /**
     * @brief Gets the code associated with the specified vertex.
     *
     *  Time Complexity - O(1)
     *
     * @param v Pointer to the vertex.
     * @return The code associated with the vertex.
     */
    static string getCode(Vertex* v);

    /**
     * @brief Gets the name associated with the specified vertex.
     *
     * Time Complexity - O(1)
     *
     * @param v Pointer to the vertex.
     * @return The name associated with the vertex.
     */
    static string getName(Vertex* v);

protected:
    vector<Vertex*> vertexSet; /**< Vector containing pointers to the vertices of the graph. */
    int n = 0; /**< The number of vertices in the graph. */
};


#endif //PROJECTDA_GRAPH_H
