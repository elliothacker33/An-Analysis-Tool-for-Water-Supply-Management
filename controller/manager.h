
#ifndef PROJECTDA_MANAGER_H
#define PROJECTDA_MANAGER_H
#include <string>
#include <unordered_map>
#include "../model/Graph.h"

/**
* @file manager.h
* @brief This file contains the manager header.
 */
using namespace std;

// For complexity Work -> unorderd map complexity is O(1) to access and O(n) to build. Improving factor by 1/n
/**
 * @class Manager
 * @brief The Manager class manages various operations related to the network graph.
 */
class Manager{

    Graph* graph;

    /**
     * @brief Imports the cities from csv
     * @param pathCities Path to the CSV file containing city data.
     */
    void importCities(const string& pathCities); // Import cities csv

    /**
     * @brief Imports the reservoirs from csv
     * @param pathReservoirs Path to the CSV file containing reservoir data.
     */
    void importReservoirs(const string& pathReservoirs); // Import reservoirs csv

    /**
     * @brief Imports the stations from csv
     * @param pathStations Path to the CSV file containing station data.
     */
    void importStations(const string& pathStations); // Import stations csv

    /**
     * @brief Imports the pipes from csv
     * @param pathPipes Path to the CSV file containing pipe data.
     */
    void importPipes(const string& pathPipes) const; // Import pipes csv


    unordered_map<string,Vertex*> stations; /**< Identifier that maps a string to a Station on the graph. */
    unordered_map<string,Vertex*> reservoirs; /**< Identifier that maps a string to a Reservoir on the graph. */
    unordered_map<string,Vertex*> cities; /**< Identifier that maps a string to a City on the graph. */

    /**
     * @brief Adds the super sink vertex to the graph
     *
     * This function creates a super sink vertex with a predefined identifier and adds it to the graph.
     *
     * Time complexity: O(V + C), where V is the number of vertices and C the number of cities
     *
     * @return Pointer to the added super sink vertex.
     */
    Vertex* addSuperSink();

    /**
     * @brief Adds a super source vertex to the graph.
     *
     * This function creates a super source vertex with a predefined identifier and adds it to the graph.
     *
     * Time complexity: O(V + R), where V is the number of vertices and R the number of reservoirs
     *
     * @return Pointer to the added super source vertex.
     */
    Vertex* addSuperSource();

    /**
     * @brief Performs a breadth-first search (BFS) to find a flow path from the super source to the super sink.
     *
     * This function performs a BFS traversal starting from the super source vertex and explores adjacent vertices to find a path to the super sink vertex. The search continues until a path from the super source to the super sink is found or until all reachable vertices are visited.
     *
     * Time complexity: O(V + E), where V is the number of vertices and E is the number of edges in the graph.
     *
     * @param superSource Pointer to the super source vertex.
     * @param superSink Pointer to the super sink vertex.
     * @return A vector of edges representing the flow path from the super source to the super sink, if found. If no path is found, an empty vector is returned.
     */
    vector<Edge*> bfs_flow(Vertex* superSource,Vertex* superSink);

    /**
     * @brief Performs a depth-first search (DFS) to find a flow path from the super source to the super sink.
     *
     * This function performs a DFS traversal starting from the super source vertex and explores adjacent vertices recursively to find a path to the super sink vertex. The search continues until a path from the super source to the super sink is found or until all reachable vertices are visited.
     *
     * Time complexity: O(V + E), where V is the number of vertices and E is the number of edges in the graph.
     *
     * @param superSource Pointer to the super source vertex.
     * @param superSink Pointer to the super sink vertex.
     * @return A vector of edges representing the flow path from the super source to the super sink, if found. If no path is found, an empty vector is returned.
     */
    vector<Edge*> dfs_flow(Vertex* superSource,Vertex* superSink);

    /**
     * @brief Helper function for depth-first search (DFS) to find a flow path from the current vertex to the super sink.
     *
     * This function is a helper function used in a depth-first search (DFS) traversal to find a flow path from the current vertex to the super sink vertex. It recursively explores adjacent vertices and edges to find a path to the super sink. If a path is found, it is stored in the provided vector.
     *
     * Time complexity: O(V + E), where V is the number of vertices and E is the number of edges in the graph.
     *
     * @param currentVertex Pointer to the current vertex being visited.
     * @param superSink Pointer to the super sink vertex.
     * @param path Reference to a vector to store the flow path if found.
     * @return True if a flow path from the current vertex to the super sink is found, false otherwise.
     */
    bool dfs_helper(Vertex *currentVertex, Vertex *superSink,vector<Edge*>& path);

    /**
     * @brief Finds the minimum flow value among the edges in the given path.
     *
     * This function iterates over the edges in the provided path and finds the minimum flow value among them.
     *
     * Time complexity: O(n), where n is the number of edges in the provided path.
     *
     * @param path A vector of pointers to edges representing a flow path.
     * @return The minimum flow value among the edges in the path.
     */
    int findMinEdge(const vector<Edge*>& path);

    /**
     * @brief Finds the maximum flow in the graph using a specified flow path exploration method.
     *
     * This function calculates the maximum flow in the graph using a specified method for exploring flow paths. It first adds a super source and a super sink to the graph, initializes flow values on edges, and then iteratively explores flow paths using the specified method until no more flow paths can be found. The function returns a vector of pairs containing the code of each city and its corresponding flow value in the maximum flow.
     *
     * Time complexity: O(V + E), where V is the number of vertices and E is the number of edges in the graph.
     *
     * @param function Pointer to a member function of the Manager class that represents the method for exploring flow paths. This function must take two Vertex pointers (superSource and superSink) as arguments and return a vector of Edge pointers representing a flow path.
     * @return A vector of pairs where each pair contains the code of a city and its corresponding flow value in the maximum flow.
     */
    vector<pair<string,int>> maxFlow(vector<Edge*> (Manager::*function)(Vertex*,Vertex*));

    /**
     * @brief Finds the maximum flow in the graph using the Edmonds-Karp algorithm.
     *
     * This function calculates the maximum flow in the graph using the Edmonds-Karp algorithm, which is based on  the Ford-Fulkerson method with BFS (breadth-first search) for finding augmenting paths. It returns a vector of pairs containing the code of each city and its corresponding flow value in the maximum flow.
     *
     * Time complexity: O(V + E), where V is the number of vertices and E is the number of edges in the graph.
     *
     * @return A vector of pairs where each pair contains the code of a city and its corresponding flow value in the maximum flow.
     */
    vector<pair<string,int>> maxFlowEdmondsKarp();

    /**
     * @brief Finds the maximum flow in the graph using the Ford-Fulkerson algorithm.
     *
     * This function calculates the maximum flow in the graph using the Ford-Fulkerson algorithm, which is based on the method of augmenting paths. It uses depth-first search (DFS) for finding augmenting paths. It returns a vector of pairs containing the code of each city and its corresponding flow value in the maximum flow.
     *
     * Time complexity: O(V + E), where V is the number of vertices and E is the number of edges in the graph.
     *
     * @return A vector of pairs where each pair contains the code of a city and its corresponding flow value in the maximum flow.
     */
    vector<pair<string,int>> maxFlowFordFulkerson();

    /**
     * @brief Creates a CSV file containing flow information.
     *
     * This function creates a CSV file at the specified path containing flow information for each city. It takes a vector of pairs where each pair contains the code of a city and its corresponding flow value. The CSV file format includes columns for the name of the city, its code, and the flow value.
     *
     * Time complexity: O(n), where n is the number of pairs in the flows vector.
     *
     * @param path The file path where the CSV file will be created.
     * @param flows A vector of pairs where each pair contains the code of a city and its corresponding flow value.
     */
    void createCsvFileFlows(const string& path,vector<pair<string,int>>& flows);

    /**
     * @brief Creates a CSV file indicating whether stations can be disabled.
     *
     * This function creates a CSV file at the specified path indicating whether each station can be disabled. It takes a vector of pairs where each pair contains the code of a station and a boolean indicating whether it can be disabled. The CSV file format includes columns for the station code and whether it can be disabled.
     *
     * Time complexity:  O(n), where n is the number of pairs in the disable vector.
     *
     * @param path The file path where the CSV file will be created.
     * @param disable A vector of pairs where each pair contains the code of a station and a boolean indicating whether it can be disabled.
     */
    void createCsvFileDisable(const string& path, vector<pair<string,bool>>& disable);

    /**
     * @brief Creates a CSV file indicating whether pipes can be disabled.
     *
     * This function creates a CSV file at the specified path indicating whether each pipe can be disabled. It takes a vector of pairs where each pair contains a pointer to a pipe edge and a boolean indicating whether it can be disabled. The CSV file format includes columns for the origin vertex code, destination vertex code, and whether the pipe can be disabled.
     *
     * Time complexity: O(n), where n is the number of pairs in the disable vector.
     *
     * @param path The file path where the CSV file will be created.
     * @param disable A vector of pairs where each pair contains a pointer to a pipe edge and a boolean indicating whether it can be disabled.
     */
    void createCsvFilePipesDisable(const string &path, vector<pair<Edge*, bool> > &disable);

    /**
     * @brief Creates a CSV file indicating whether cities have enough water.
     *
     * This function creates a CSV file at the specified path indicating whether each city has enough water. It takes a vector of pairs where each pair contains the code of a city and a boolean indicating whether it has enough water. The CSV file format includes columns for the city code and whether it has enough water.
     *
     * Time complexity: O(n), where n is the number of pairs in the enoughWater vector.
     *
     * @param path The file path where the CSV file will be created.
     * @param enoughWater A vector of pairs where each pair contains the code of a city and a boolean indicating whether it has enough water.
     */
    void createCsvFileEnoughWater(const string &path, vector<pair<string, bool>> &enoughWater);

    /**
     * @brief Creates a CSV file containing rates information.
     *
     * This function creates a CSV file at the specified path containing rates information for each entity. It takes a vector of pairs where each pair contains the code of an entity and its corresponding rate value. The CSV file format includes columns for the name of the entity, its code, and the rate value.
     *
     * Time complexity: O(n), where n is the number of pairs in the rates vector.
     *
     * @param path The file path where the CSV file will be created.
     * @param rates A vector of pairs where each pair contains the code of an entity and its corresponding rate value.
     */
    void createCsvFileRates(const string& path,vector<pair<string,double>>& rates);

    /**
     * @brief Calculates flow rates for cities based on the provided flows and generates a CSV file.
     *
     * This function calculates flow rates for cities based on the provided flows and generates a CSV file containing the calculated rates. It takes a vector of pairs where each pair contains the code of a city and its corresponding flow value. It calculates the flow rate for each city as a percentage of the total flow. The CSV file format includes columns for the city code and its corresponding flow rate.
     *
     * Time complexity: O(n + m), where n is the number of pairs in the flows vector and m is the number of pairs in the rates vector.
     *
     * @param flows A vector of pairs where each pair contains the code of a city and its corresponding flow value.
     * @param outputFile The file path where the CSV file with flow rates will be created.
     */
    void calculateFlowRates(const vector<pair<string, int>>& flows, const string& outputFile);

    /**
     * @brief Prints and exports the top K flows to a CSV file.
     *
     * This function sorts the flows vector in descending order based on flow values and prints the top K flows to the console. It then exports the top K flows to a CSV file at the specified path. The CSV file format includes columns for the city code and its corresponding flow value.
     *
     * Time complexity: O(n log n), where n is the number of pairs in the flows vector.
     *
     * @param flows A vector of pairs where each pair contains the code of a city and its corresponding flow value.
     * @param k The number of top flows to print and export.
     * @param outputFile The file path where the CSV file with top flows will be created.
     */
    void printTopKFlows(vector<pair<string, int>>& flows, int k, const string& outputFile);

    /**
     * @brief Prints flow metrics for chosen cities and exports them to a CSV file.
     *
     * This function calculates and prints flow metrics for the chosen cities based on the provided flows. It calculates the total flow for the chosen cities, prints individual flow values for each chosen city, and exports the flow metrics to a CSV file at the specified path. The CSV file format includes columns for the city code and its corresponding flow value.
     *
     * Time complexity: O(n + m), where n is the number of pairs in the flows vector and m is the number of chosen cities.
     *
     * @param flows A vector of pairs where each pair contains the code of a city and its corresponding flow value.
     * @param chosenCities A vector containing the codes of the chosen cities for which flow metrics will be calculated.
     * @param outputFile The file path where the CSV file with flow metrics will be created.
     */
    void printFlowMetrics(vector<pair<string, int>>& flows,vector<string>& chosenCities, const string& outputFile);

    /**
     * @brief Disables stations with the given codes.
     *
     * This function disables stations in the graph based on the provided station codes. It iterates over the provided vector of station codes and disables each corresponding station in the graph.
     *
     * Time complexity: O(n), where n is the number of station codes in the stations vector.
     *
     * @param stations A vector containing the codes of the stations to be disabled.
     */
    void disableStations(vector<string>& stations);

    /**
     * @brief Disables the specified pipes.
     *
     * This function disables the pipes in the graph based on the provided vector of pipe pointers. It iterates over the provided vector of pipe pointers and disables each corresponding pipe in the graph.
     *
     * Time compplexity: O(n), where n is the number of pipes in the pipes vector.
     *
     * @param pipes A vector containing pointers to the pipes to be disabled.
     */
    void disablePipes(vector<Edge*>& pipes);

    /**
     * @brief Disables reservoirs with the given codes.
     *
     * This function disables reservoirs in the graph based on the provided reservoir codes. It iterates over the provided vector of reservoir codes and disables each corresponding reservoir in the graph.
     *
     * Time complexity: O(n), where n is the number of reservoir codes in the reservoirs vector.
     *
     * @param reservoirs A vector containing the codes of the reservoirs to be disabled.
     */
    void disableReservoirs(vector<string>& reservoirs);


    void dfs_disable(Vertex* reservoir);

    /**
     * @brief Shuts down reservoirs and checks the impact on network flow.
     *
     * This function shuts down the reservoirs specified in the `reservoirs` vector and checks the impact on the network flow using the specified flow calculation function.
     *
     * Time complexity: O(f + n + m), where f is the time complexity of the specified flow calculation function, n is the number of cities, and m is the number of reservoirs.
     *
     * @param flowfunction A pointer to a member function of the Manager class that calculates network flow.
     * @param reservoirs A vector containing the codes of the reservoirs to be shut down.
     * @return True if the network flow remains unaffected after shutting down the reservoirs, false otherwise.
     */
    bool shutdownReservoirs(vector<pair<string,int>> (Manager::*flowfunction)(),vector<string>& reservoirs);

    /**
     * @brief Shuts down pipes and checks the impact on network flow.
     *
     * This function shuts down the specified pipes and checks the impact on the network flow using the specified  flow calculation function.
     *
     * Time complexity: O(f + m + n), where f is the time complexity of the specified flow calculation function, m is the number of pipes, and n is the number of cities.
     *
     * @param flowfunction A pointer to a member function of the Manager class that calculates network flow.
     * @param pipes A vector containing pointers to the edges (pipes) to be shut down.
     * @return True if the network flow remains unaffected after shutting down the pipes, false otherwise.
     */
    bool shutdownPipes(vector<pair<string,int>> (Manager::*flowfunction)(),vector<Edge*>& pipes);

    /**
     * @brief Shuts down stations and checks the impact on network flow.
     *
     * This function shuts down the specified stations and checks the impact on the network flow using the specified  flow calculation function.
     *
     * Time complexity: O(f + s + n), where f is the time complexity of the specified flow calculation function, s is the number of stations, and n is the number of cities.
     *
     * @param function A pointer to a member function of the Manager class that calculates network flow.
     * @param stations A vector containing the codes of stations to be shut down.
     * @return True if the network flow remains unaffected after shutting down the stations, false otherwise.
     */
    bool shutdownStations(vector<pair<string,int>> (Manager::*function)(),vector<string>& stations);


    vector<pair<string,int>> graphChangeFlowsAfterReservoirsDisabled(vector<string>& reservoirs);

    /**
     * @brief Shuts down pipes and calculates the percentage decrease in flow for each city.
     *
     * This function shuts down the specified pipes and calculates the percentage decrease in flow for each city  using the specified flow calculation function.
     *
     * Time complexity: O(f + p + n), where f is the time complexity of the specified flow calculation function, p is the number of pipes, and n is the number of cities.
     *
     * @param flowfunction A pointer to a member function of the Manager class that calculates network flow.
     * @param pipes A vector containing pointers to the pipes to be shut down.
     * @return A vector of pairs containing the city code and the percentage decrease in flow for each city.
     */
    vector<pair<string,double>> shutdownPipesWithDecrease(vector<pair<string,int>> (Manager::*flowfunction)(),vector<Edge*>& pipes);

    /**
     * @brief Determines if each city can get enough water based on provided flow data.
     *
     * This function determines if each city in the provided list of city codes can get enough water  based on the flow data provided. It compares the flow data with the demand for each city and  returns a vector of pairs indicating whether each city can get enough water.
     *
     * Time complexity: O(m), where m is the number of flow data entries.
     *
     * @param codes A vector containing the codes of the cities to be checked.
     * @param flows A vector containing pairs of city codes and corresponding flow data.
     * @return A vector of pairs containing the city code and a boolean indicating if the city can get enough water.
     */
    vector<pair<string,bool>> canCityGetEnoughWater(vector<string>& codes,vector<pair<string,int>>& flows);

public:
    /**
     * @brief Default constructor for the Manager class.
     *
     * This constructor initializes a new Manager object. It creates a new instance of the Graph class  to manage the underlying graph data structure.
     *
     * Time complexity: O(1)
     */
    Manager();

    /**
     * @brief Destructor for the Manager class.
     *
     * This destructor cleans up resources used by the Manager object. It deletes the dynamically allocated graph object to prevent memory leaks.
     *
     * Time complexity: O(n + m), where n is the number of vertices and m is the number of edges in the graph.
     */
    ~Manager();

    /**
     * @brief Getter function for the graph object.
     *
     * Time complexity: O(1)
     *
     * @return A pointer to the graph object.
     */
    Graph* getGraph() const;

    /**
     * @brief Imports data from files to initialize the graph.
     *
     * @param pathCities The path to the file containing city data.
     * @param pathReservoirs The path to the file containing reservoir data.
     * @param pathStations The path to the file containing station data.
     * @param pathPipes The path to the file containing pipe data.
     */
    void importFiles(const string& pathCities,const string& pathReservoirs,const string& pathStations,const string& pathPipes);

    /**
     * @brief Resets the graph and its properties.
     *
     * Time complexity: O(V + E + R), where V is the number of vertices, E is the number of edges, and R is the number of residual edges.
     */
    void resetGraph();

    /**
     * @brief Resets the manager and its properties.
     *
     * Time complexity: O(V + E), where V is the number of vertices and E is the number of edges in the graph.
     */
    void resetManager();

    /**
     * @brief Get the unordered map of stations.
     *
     * @return An unordered map containing the stations, where the keys are the station codes and the values are pointers to the corresponding vertex objects.
     */
    unordered_map<string,Vertex*> getStations() const; // get unordered map of stations

    /**
     * @brief Get the unordered map of reservoirs.
     *
     * @return An unordered map containing the reservoirs, where the keys are the reservoir codes and the values are pointers to the corresponding vertex objects.
     */
    unordered_map<string,Vertex*> getReservoirs() const; // get unordered map of reservoir

    /**
     * @brief Get the unordered map of cities.
     *
     * @return An unordered map containing the cities, where the keys are the city codes and the values are pointers to the corresponding vertex objects.
     */
    unordered_map<string,Vertex*> getCities() const; // get unordered map of cities

    /**
     * @brief Find a vertex in the manager's maps based on its identifier.
     *
     * This function searches for a vertex with the given identifier in the maps of cities, reservoirs, and stations. If found, it returns a pointer to the corresponding vertex; otherwise, it returns nullptr.
     *
     * Time complexity:  O(1)
     *
     * @param identifier The identifier of the vertex to find.
     * @return A pointer to the vertex if found; otherwise, nullptr.
     */
    Vertex* findVertexInMap(const string& identifier) const;

    /**
     * @brief Get the total number of edges in the graph.
     *
     * This function calculates and returns the total number of edges present in the graph managed by the Manager object.
     *
     * Time complexity:  O(V + E)
     *
     * @return The total number of edges in the graph.
     */
    int getHowManyEdges() const;

    /* Exercise 2.1 */
    /**
     * @brief Perform the Edmonds-Karp algorithm and print flow metrics for selected cities.
     *
     * This function calculates the maximum flow in the network using the Edmonds-Karp algorithm and prints flow metrics for the selected cities to a CSV file.
     *
     * Time complexity: O(V + E)
     *
     * @param cities A vector containing the codes of cities for which flow metrics are to be calculated.
     */
    void getEdmondsKarpXCity(vector<string>& cities);

    /**
     * @brief Computes the maximum flow using the Ford-Fulkerson algorithm for the specified cities.
     *
     * This function calculates the maximum flow for each city in the input vector using the Ford-Fulkerson algorithm and prints the flow metrics to a CSV file.
     *
     * Time complexity: O(V + E)
     *
     * @param cities A vector of strings representing the codes of cities for which the maximum flow is to be calculated.
     */
    void getFordFulkersonXCity(vector<string>& cities);

    /**
     * @brief Run the Edmonds-Karp algorithm to find maximum flow for all cities.
     *
     * This function runs the Edmonds-Karp algorithm to find the maximum flow for all cities in the network. It optionally resets the graph after computation and saves the results to a CSV file.
     *
     * Time complexity: O(V.E^2)
     *
     * @param reset Flag indicating whether to reset the graph after computation. Default is true.
     */
    void getEdmondsKarpAllCities(bool reset = true);

    /**
     * @brief Run the Ford-Fulkerson algorithm to find maximum flow for all cities.
     *
     * This function runs the Ford-Fulkerson algorithm to find the maximum flow for all cities in the network. It optionally resets the graph after computation and saves the results to a CSV file.
     *
     * Time complexity: O(E⋅f).
     *
     * @param reset Flag indicating whether to reset the graph after computation. Default is true.
     */
    void getFordFulkersonAllCities(bool reset = true);

    /* Exercise 2.2 */

    /**
     *  @brief Check if specified cities can get enough water using the Edmonds-Karp algorithm.
     *
     *  This function calculates the maximum flow in the network using the Edmonds-Karp algorithm and checks if the specified cities can get enough water based on the calculated flows. It then saves the results to a CSV file and resets the graph.
     *
     * time complecity: O(V+E)
     * @param cities A vector of city codes to check for water availability.
     */
    void canCityXGetEnoughWaterEK(vector<string>& cities);

    /**
     * @brief Check if specified cities can get enough water using the Ford-Fulkerson algorithm.
     *
     * This function calculates the maximum flow in the network using the Ford-Fulkerson algorithm and checks if the specified cities can get enough water based on the calculated flows. It then saves the results to a CSV file and resets the graph.
     *
     * Time complexity:  O(E * f), where E is the number of edges and f is the maximum flow.
     *
     * @param cities A vector of city codes to check for water availability.
     */
    void canCityXGetEnoughWaterFF(vector<string>& cities);

    /**
     * @brief Check if all cities can get enough water using the Edmonds-Karp algorithm.
     *
     * This function calculates the maximum flow in the network using the Edmonds-Karp algorithm and checks if all cities in the network can get enough water based on the calculated flows. It then saves the results to a CSV file and resets the graph.
     *
     * Time complexity: O(V + E)
     */
    void canAllCitiesGetEnoughWaterEK();

    /**
     * @brief Check if all cities can get enough water using the Ford-Fulkerson algorithm.
     *
     * This function calculates the maximum flow in the network using the Ford-Fulkerson algorithm and checks if all cities in the network can get enough water based on the calculated flows. It then saves the results to a CSV file and resets the graph.
     *
     * Time complexity:  O(V * E * f).
     */
    void canAllCitiesGetEnoughWaterFF();

    /* Exercise 2.3 */
    void improvePipesHeuristic();
    void improvePipesHeuristicEK();


    void improvePipesHeuristicFF();

    /* Exercise 3.1 */
    /**
     * @brief Disable each reservoir and check if it affects the network flow using Edmonds-Karp algorithm.
     *
     * This function disables each reservoir in the network one by one and checks if it affects the network flow using the Edmonds-Karp algorithm. It creates a CSV file with the results indicating whether each reservoir can be disabled without affecting the flow.
     *
     * Time complexity: O(VE^2).
     */
    void disableEachReservoirEdmondsKarp();
    void disableEachReservoirFordFulkerson();
    void disableSelectedReservoirsEdmondsKarp(vector<string>& reservoirs);
    void disableSelectedReservoirsFordFulkerson(vector<string>& reservoirs);

    /* Exercise 3.2 */

    /**
     * @brief Disable each station individually and measure network impact using Edmonds-Karp algorithm.
     *
     * This function iterates over each station in the network, disables it, and calculates the maximum flow in the network using the Edmonds-Karp algorithm. It then records whether the network was affected by the station's disablement and saves the results to a CSV file.
     *
     * Time complexity: O(n * f(n)).
     */
    void disableEachStationEdmondsKarp();

    /**
     * @brief Disable each station individually and measure network impact using Ford-Fulkerson algorithm.
     *
     * This function iterates over each station in the network, disables it, and calculates the maximum flow in the network using the Ford-Fulkerson algorithm. It then records whether the network was affected by the station's disablement and saves the results to a CSV file.
     *
     * Time complexity: O(n * f(n)).
     */
    void disableEachStationFordFulkerson();

    /**
     * @brief Disable selected stations and measure the decrease in flow rates using the Edmonds-Karp algorithm.
     *
     * This function takes a vector of station codes, disables each selected station individually, and calculates the decrease in flow rates for each city using the Edmonds-Karp algorithm. It then saves the results to a CSV file.
     *
     * Time complexity:  O(f(n)).
     *
     * @param stations A vector containing the codes of the stations to be disabled.
     */
    void disableSelectedStationsEdmondsKarp(vector<string>& stations);

    /**
     * @brief Disable selected stations and measure the decrease in flow rates using the Ford-Fulkerson algorithm.
     *
     * This function takes a vector of station codes, disables each selected station individually, and calculates the decrease in flow rates for each city using the Ford-Fulkerson algorithm. It then saves the results to a CSV file.
     *
     * Time complexity:  O(f(n)).
     *
     * @param stations A vector containing the codes of the stations to be disabled.
     */
    void disableSelectedStationsFordFulkerson(vector<string>& stations);

    /**
     * @brief Shutdown selected stations and measure the decrease in flow rates.
     *
     * This function takes a flow calculation function pointer and a vector of station codes, shuts down each selected station individually, calculates the decrease in flow rates for each city, and returns the results as a vector of pairs containing the city code and the percentage decrease in flow rate.
     *
     * Time complexity: O(n^2).
     *
     * @param function A pointer to a flow calculation function.
     * @param stations A vector containing the codes of the stations to be shut down.
     * @return A vector of pairs containing the city code and the percentage decrease in flow rate.
     */
    vector<pair<string,double>> shutdownStationsGettingDecreaseFlows(vector<pair<string,int>> (Manager::*function)(),vector<string>& stations);

    /* Exercise 3.3 */

    /**
     * @brief Disable each pipe individually and check if it affects the network flow using Edmonds-Karp algorithm.
     *
     * This function iterates through each pipe in the graph, disables it, and checks if the network flow is affected using the Edmonds-Karp algorithm. It records the result for each pipe and saves it to a CSV file.
     *
     * Time complexity: O(m^2)
     */
    void disableEachPipeEdmondsKarp();

    /**
     * @brief Disable each pipe individually and check if it affects the network flow using Ford-Fulkerson algorithm.
     *
     * This function iterates through each pipe in the graph, disables it, and checks if the network flow is affected using the Ford-Fulkerson algorithm. It records the result for each pipe and saves it to a CSV file.
     *
     * Time complexity: O(m^2)
     */
    void disableEachPipeFordFulkerson();

    /**
     * @brief Disable selected pipes and calculate the decrease rate in flow using Edmonds-Karp algorithm.
     *
     * This function disables the specified pipes and calculates the decrease rate in flow for each affected city using the Edmonds-Karp algorithm. It saves the results to a CSV file.
     *
     * Time complexity: O(p⋅m), where p is the number of selected pipes and m is the number of edges in the graph.
     *
     * @param pipes A vector of pointers to the pipes to be disabled.
     */
    void disableSelectedPipesEdmondsKarp(vector<Edge*> &pipes);

    /**
     * @brief Disable selected pipes and calculate the decrease rate in flow using Ford-Fulkerson algorithm.
     *
     * This function disables the specified pipes and calculates the decrease rate in flow for each affected city using the Ford-Fulkerson algorithm. It saves the results to a CSV file.
     *
     * Time complexity: O(p⋅m), where p is the number of selected pipes and m is the number of edges in the graph.
     *
     * @param pipes A vector of pointers to the pipes to be disabled.
     */
    void disableSelectedPipesFordFulkerson(vector<Edge*> &pipes);


    /* Extras */

    /**
     * @brief Find the top K cities with the highest flow using the Edmonds-Karp algorithm.
     *
     * This function calculates the maximum flow using the Edmonds-Karp algorithm and then identifies the top K cities with the highest flow. It saves the results to a CSV file.
     *
     * Time complexity: O(n^2.m)
     *
     * @param k The number of top cities to find.
     */
    void topKFlowEdmondsKarpCities(int k);

    /**
     * @brief Find the top K cities with the highest flow using the Ford-Fulkerson algorithm.
     *
     * This function calculates the maximum flow using the Ford-Fulkerson algorithm and then identifies the top K cities with the highest flow. It saves the results to a CSV file.
     *
     * Time complexity: O(V+E)
     *
     * @param k
     */
    void topKFlowFordFulkersonCities(int k);

    /**
     * @brief Calculate the flow rate per city using the Edmonds-Karp algorithm.
     *
     * This function calculates the flow rate for each city in the network using the Edmonds-Karp algorithm for finding the maximum flow. It saves the results to a CSV file.
     *
     * Time complexity: =(VE^2)
     */
    void flowRatePerCityEdmondsKarp();

    /**
     *  @brief Calculate the flow rate per city using the Ford-Fulkerson algorithm.
     *
     *  This function calculates the flow rate for each city in the network using the Ford-Fulkerson algorithm for finding the maximum flow. It saves the results to a CSV file.
     *
     *  Time complexity: O(E.f).
     */
    void flowRatePerCityFordFulkerson();

    vector<int> getMetrics();
};


#endif //PROJECTDA_MANAGER_H