
#ifndef PROJECTDA_VERTEX_H
#define PROJECTDA_VERTEX_H

using namespace std;
/**
* @file Vertex.h
* @brief This file contains the Vertex header.
 */

// Standard Library Headers
#include <string>    // Include for string manipulation functionalities
#include <vector>    // Include for vector container functionalities

class Graph;
class Edge;


/**
 * @class Vertex
 * @brief Represents an abstract class that represents a Vertex
 */
class Vertex {
public:
    virtual char getType() const = 0;
    virtual ~Vertex();

    /**
     * @brief Retrieves the number of adjacent edges.
     *
     * Time Complexity: O(1)
     *
     * @return Number of adjacent edges.
     */
    int getOutDegree() const;

    /**
     * @brief Sets the number of adjacent edges.
     *
     * Time Complexity: O(1)
     *
     * @param outDegree Number of adjacent edges.
     */
    void setOutDegree(int outDegree);

    /**
    * @brief Retrieves the number of incoming edges.
    *
    * Time Complexity: O(1)
    *
    * @return Number of incoming edges.
    */
    int getInDegree() const;

    /**
     * @brief Sets the number of incoming edges.
     *
     * Time Complexity: O(1)
     *
     * @param outDegree Number of incoming edges.
     */
    void setInDegree(int inDegree);

    /**
    * @brief Checks if the vertex has been visited.
    *
    * Time Complexity: O(1)
    *
    * @return True if the vertex has been visited, false otherwise.
    */
    bool isVisited() const;

    /**
     * @brief Sets the visited state of the vertex.
     *
     * Time Complexity: O(1)
     *
     * @param isVisited The visited state to set.
     */
    void setVisited(bool isVisited);

    /**
     * @brief Checks if the vertex is enabled.
     *
     * Time Complexity: O(1)
     *
     * @return True if the vertex is enabled, false otherwise.
     */
    bool isEnabled() const;

    /**
     * @brief Sets the enabled state of the vertex.
     *
     * Time Complexity: O(1)
     *
     * @param isEnabled The enabled state to set.
     */
    void setEnabled(bool isEnabled);

    /**
    * @brief Adds an edge to the vertex.
    *
    * Time Compleity: O(E)
    *
    * @param t Pointer to the target vertex.
    * @param capacity Capacity of the edge.
    * @param type Type of the edge.
    * @return Pointer to the added edge.
    */
    Edge* addEdge(Vertex* t, int capacity, const string& type);

    /**
     * @brief Removes an edge from the vertex.
     *
     * Time Compleity: O(E)
     *
     * @param e Pointer to the edge to be removed.
     */
    void removeEdge(const Edge* e);

    /**
     * @brief Sets the path edge of the vertex.
     *
     * Time Complexity: O(1)
     *
     * @param e Pointer to the path edge.
     */
    void setPath(Edge* e);

    /**
     * @brief Gets the path edge of the vertex.
     *
     * Time Complexity: O(1)
     *
     * @return Pointer to the path edge of the vertex.
     */
    Edge* getPath() const;

    /**
   * @brief Removes an incoming edge from the vertex.
   *
   * Time Complexity: O(E)
   *
   * @param e Pointer to the incoming edge to be removed.
   */
    void removeEdgeIncoming(const Edge* e);

    /**
     * @brief Adds an incoming edge to the vertex.
     *
     * Time Complexity: O(E)
     *
     * @param e Pointer to the incoming edge to be added.
     * @return True if the edge was successfully added, false otherwise.
     */
    bool addIncoming(Edge* e);

    /**
     * @brief Gets the vector of incoming edges.
     *
     * Time Complexity: O(1)
     *
     * @return Vector of incoming edges.
     */
    vector<Edge*> getIncoming();

    /**
     * @brief Gets the vector of adjacent edges.
     *
     * Time Complexity: O(1)
     *
     * @return Vector of adjacent edges.
     */
    vector<Edge*> getAdj();

protected:
 /**
  * @brief Indicates whether the vertex has been visited.
  *
  * This boolean field is used to track whether the vertex has been visited during graph traversal algorithms.
  * By default, it is initialized to false.
  */
 bool visited = false;

 /**
  * @brief Indicates whether the vertex is enabled.
  *
  * This boolean field is used to determine whether the vertex is enabled or disabled in the graph.
  * By default, it is initialized to true.
  */
 bool enabled = true;

 /**
  * @brief Stores the out-degree of the vertex.
  *
  * This integer field represents the number of outgoing edges from the vertex.
  * By default, it is initialized to 0.
  */
 int outDegree = 0;

 /**
  * @brief Stores the in-degree of the vertex.
  *
  * This integer field represents the number of incoming edges to the vertex.
  * By default, it is initialized to 0.
  */
 int inDegree = 0;

 /**
  * @brief Stores the adjacent edges of the vertex.
  *
  * This vector stores pointers to the edges that are adjacent to the vertex.
  * It represents the edges going out from the vertex.
  */
 vector<Edge*> adj;

 /**
  * @brief Stores the incoming edges of the vertex.
  *
  * This vector stores pointers to the edges that are incoming to the vertex.
  * It represents the edges coming into the vertex.
  */
 vector<Edge*> incoming;

 /**
  * @brief Pointer to the path edge of the vertex.
  *
  * This pointer stores the path edge associated with the vertex during graph traversal algorithms.
  * It points to the edge that leads to this vertex from its predecessor in the traversal.
  * By default, it is initialized to nullptr.
  */
 Edge* path = nullptr;

};

/**
 * @class City
 * @brief Represents a city vertex in a graph.
 *
 * This class inherits from the Vertex class and represents a city vertex in a graph.
 * It stores information such as the city's name, code, demand, population, and ID.
 */
class City final : public Vertex {
public:
    /**
     * @brief Constructor to initialize a City object.
     *
     * Constructs a City object with the specified name, ID, code, demand, and population.
     *
     * @param name The name of the city.
     * @param id The ID of the city.
     * @param code The code of the city.
     * @param demand The demand of the city.
     * @param population The population of the city.
     */
    City(const string& name, const int id, const string& code, const int demand, const int population)
        : name(name), code(code), demand(demand), population(population), id(id) {}

    /**
     * @brief Gets the type of the vertex.
     *
     * Time Complexity - O(1)
     *
     * @return The type of the vertex ('C' for City).
     */
    char getType() const override;

    /**
     * @brief Gets the name of the city.
     *
     * Time Complexity - O(1)
     *
     * @return The name of the city.
     */
    string getName() const;

    /**
     * @brief Gets the code of the city.
     *
     * Time Complexity - O(1)
     *
     * @return The code of the city.
     */
    string getCode() const;

    /**
     * @brief Gets the demand of the city.
     *
     * Time Complexity - O(1)
     *
     * @return The demand of the city.
     */
    int getDemand() const;

    /**
     * @brief Gets the population of the city.
     *
     * Time Complexity - O(1)
     *
     * @return The population of the city.
     */
    int getPopulation() const;

    /**
     * @brief Gets the ID of the city.
     *
     * Time Complexity - O(1)
     *
     * @return The ID of the city.
     */
    int getId() const;

private:
    string name;        /**< The name of the city. */
    string code;        /**< The code of the city. */
    int demand;         /**< The demand of the city. */
    int population;     /**< The population of the city. */
    int id;             /**< The ID of the city. */
};

/**
 * @class Station
 * @brief Represents a station vertex in a graph.
 *
 * This class inherits from the Vertex class and represents a station vertex in a graph.
 * It stores information such as the station's code and ID.
 */
class Station final : public Vertex {
public:
    /**
    * @brief Constructor to initialize a Station object.
    *
    * Constructs a Station object with the specified ID, code.
    *
    * @param id The ID of the city.
    * @param code The code of the city.
    */
    Station(const int id,const string& code) : code(code),id(id){}

    /**
     * @brief Gets the type of the vertex.
     *
     * Time Complexity - O(1)
     *
     * @return The type of the vertex ('S' for Station).
     */
    char getType() const override;

     /**
       * @brief Gets the code of the station.
       *
       * Time Complexity - O(1)
       *
       * @return The code of the station.
       */
     string getCode() const;

     /**
      * @brief Gets the ID of the station.
      *
      * Time Complexity - O(1)
      *
      * @return The ID of the station.
      */
     int getId() const;


private:
    string code; /**< The code of the station. */
    int id; /**< The ID of the station. */
};


/**
 * @class Reservoir
 * @brief Represents a reservoir vertex in a graph.
 *
 * This class inherits from the Vertex class and represents a station vertex in a graph.
 * It stores information such as the resrvoir's name, code, maxDelivery, municipality, and ID.
 */
class Reservoir final : public Vertex {
public:
     /**
    * @brief Constructor to initialize a Reservoir object.
    *
    * Constructs a Reservoir object with the specified name,municipality,maxDelivery,ID, code.
    *
    * @param id The ID of the resevoir.
    * @param code The code of the reservoir.
    * @param municipality The municipality of the reservoir.
    * @param name The name of the reservoir.
    * @param maxDelivery The max delivery of the reservoir.
    */
    Reservoir(const string& name,const string& municipality,const int id, const string& code,const int maxDelivery)
        : name(name), code(code), municipality(municipality), maxDelivery(maxDelivery), id(id) {}

   /**
    * @brief Gets the type of the vertex.
    *
    * Time Complexity - O(1)
    *
    * @return The type of the vertex ('R' for Reservoir).
    */
    char getType() const override;

    /**
     * @brief Gets the name of the resevoir.
     *
     * Time Complexity - O(1)
     *
     * @return The name of the reservoir.
     */
    string getName() const;
    /**
    * @brief Gets the municipality of the resevoir.
    *
    * Time Complexity - O(1)
    *
    * @return The municipality of the reservoir.
    */
    string getMunicipality() const;
    /**
    * @brief Gets the code of the resevoir.
    *
    * Time Complexity - O(1)
    *
    * @return The code of the reservoir.
    */
    string getCode() const;

    /**
    * @brief Gets the max delivery of the resevoir.
    *
    * Time Complexity - O(1)
    *
    * @return The max delivery of the reservoir.
    */
    int getMaxDelivery() const;

    /**
    * @brief Gets the ID of the resevoir.
    *
    * Time Complexity - O(1)
    *
    * @return The ID of the reservoir.
    */
    int getId() const;


private:
    string name; /**< The name of the reservoir. */
    string municipality; /**< The municipality of the reservoir. */
    string code; /**< The code of the reservoir. */
    int maxDelivery; /**< The max delivery of the reservoir. */
    int id; /**< The id of the reservoir. */

};


#endif //PROJECTDA_VERTEX_H
