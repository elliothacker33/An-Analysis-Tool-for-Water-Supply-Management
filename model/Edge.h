#ifndef PROJECTDA_EDGE_H
#define PROJECTDA_EDGE_H


using namespace std;
// Standard Library Headers
#include <string>    // Include for string manipulation functionalities

class Vertex;

/**
 * @class Edge
 * @brief Represents a edge that connects two vertexes in a graph.
 *
 * This class represents a pipe in the context of the problem, and is represented by two pointer, to the origin vertex
 * and destination vertex, capacity and type of the edge.
 */
class Edge {
public:
    /**
     * @brief Constructs an edge between two vertices with specified capacity and type.
     *
     * Time Complexity: O(1)
     *
     * @param orig Pointer to the origin vertex.
     * @param dest Pointer to the destination vertex.
     * @param capacity The capacity of the edge.
     * @param type The type of the edge.
     */
    Edge(Vertex* orig, Vertex* dest, int capacity, const string& type);

    /**
     * @brief Sets the reverse edge of this edge.
     *
     * Time Complexity: O(1)
     *
     * @param e Pointer to the reverse edge.
     */
    void setReverseEdge(Edge* e);

    /**
     * @brief Sets the flow of the edge.
     *
     * Time Complexity: O(1)
     *
     * @param flow The new flow value to be set for the edge.
     */
    void setFlow(int flow);

    /**
     * @brief Retrieves the flow of the edge.
     *
     * Time Complexity: O(1)
     *
     * @return The current flow value of the edge.
     */
    int getFlow() const;

    /**
     * @brief Sets the enabled state of the edge.
     *
     * Time Complexity: O(1)
     *
     * @param isEnabled The new enabled state for the edge.
     */
    void setEnabled(bool isEnabled);

    /**
     * @brief Checks if the edge is enabled.
     *
     * Time Complexity: O(1)
     *
     * @return True if the edge is enabled, false otherwise.
     */
    bool isEnabled() const;

    /**
     * @brief Retrieves the reverse edge of this edge.
     *
     * Time Complexity: O(1)
     *
     * @return Pointer to the reverse edge.
     */
    Edge* getReverseEdge() const;

    /**
     * @brief Retrieves the capacity of the edge.
     *
     * Time Complexity: O(1)
     *
     * @return The capacity of the edge.
     */
    int getCapacity() const;

    /**
     * @brief Retrieves the type of the edge.
     *
     * Time Complexity: O(1)
     *
     * @return The type of the edge.
     */
    const string& getType() const;

    /**
     * @brief Sets the type of the edge.
     *
     * Time Complexity: O(1)
     *
     * @param type The new type for the edge.
     */
    void setType(const string& type);

    /**
     * @brief Retrieves the destination vertex of the edge.
     *
     * Time Complexity: O(1)
     *
     * @return Pointer to the destination vertex.
     */
    Vertex* getDest() const;

    /**
     * @brief Retrieves the origin vertex of the edge.
     *
     * Time Complexity: O(1)
     *
     * @return Pointer to the origin vertex.
     */
    Vertex* getOrigin() const;

protected:
    Vertex* orig;    /**< Pointer to the origin vertex */
    Vertex* dest;    /**< Pointer to the destination vertex */
    int capacity;    /**< Capacity of the edge */
    string type;     /**< Type of the edge */
    bool enabled = true;     /**< Enabled state of the edge */
    Edge* reverse = nullptr; /**< Pointer to the reverse edge */
    int flow = 0;    /**< Flow of the edge */
};


#endif //PROJECTDA_EDGE_H
