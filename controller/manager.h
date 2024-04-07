
#ifndef PROJECTDA_MANAGER_H
#define PROJECTDA_MANAGER_H
#include <string>
#include <unordered_map>
#include "../model/Graph.h"

using namespace std;

// For complexity Work -> unordered map complexity is O(1) to access and O(n) to build. Improving factor by 1/n
class Manager{

    Graph* graph;
    void importCities(const string& pathCities); // Import cities csv
    void importReservoirs(const string& pathReservoirs); // Import reservoirs csv
    void importStations(const string& pathStations); // Import stations csv
    void importPipes(const string& pathPipes) const; // Import pipes csv
    unordered_map<string,Vertex*> stations; // Identifier that maps a string to a Station on the graph
    unordered_map<string,Vertex*> reservoirs; // Identifier that maps a string to a Reservoir on the graph
    unordered_map<string,Vertex*> cities; // Identifier that maps a string to a City on the graph
    static int parseInt(const string& text);
    Vertex* addSuperSink();
    Vertex* addSuperSource();
    vector<Edge*> bfs_flow(Vertex* superSource,Vertex* superSink);
    vector<Edge*> dfs_flow(Vertex* superSource,Vertex* superSink);
    bool dfs_helper(Vertex *currentVertex, Vertex *superSink,vector<Edge*>& path);
    int findMinEdge(const vector<Edge*>& path);
    vector<pair<string,int>> maxFlow(vector<Edge*> (Manager::*function)(Vertex*,Vertex*));
    vector<pair<string,int>> maxFlowEdmondsKarp();
    vector<pair<string,int>> maxFlowFordFulkerson();
    void createCsvFileFlows(const string& path,vector<pair<string,int>>& flows);
    void createCsvFileDisable(const string& path, vector<pair<string,bool>>& disable);
    void createCsvFilePipesDisable(const string &path, vector<pair<Edge*, bool> > &disable);
    void createCsvFileEnoughWater(const string &path, vector<pair<string, bool>> &enoughWater);
    void createCsvFileRates(const string& path,vector<pair<string,double>>& rates);
    void calculateFlowRates(const vector<pair<string, int>>& flows, const string& outputFile);
    void printTopKFlows(vector<pair<string, int>>& flows, int k, const string& outputFile);
    void printFlowMetrics(vector<pair<string, int>>& flows,vector<string>& chosenCities, const string& outputFile);
    void disableStations(vector<string>& stations);
    void disablePipes(vector<Edge*>& pipes);
    void disableReservoirs(vector<string>& reservoirs);
    void dfs_disable(Vertex* reservoir);
    bool shutdownReservoirs(vector<pair<string,int>> (Manager::*flowfunction)(),vector<string>& reservoirs);
    bool shutdownPipes(vector<pair<string,int>> (Manager::*flowfunction)(),vector<Edge*>& pipes);
    bool shutdownStations(vector<pair<string,int>> (Manager::*function)(),vector<string>& stations);
    vector<pair<string,int>> graphChangeFlowsAfterReservoirsDisabled(vector<string>& reservoirs);
    vector<pair<string,double>> shutdownPipesWithDecrease(vector<pair<string,int>> (Manager::*flowfunction)(),vector<Edge*>& pipes);
    vector<pair<string,bool>> canCityGetEnoughWater(vector<string>& codes,vector<pair<string,int>>& flows);
public:
    Manager();
    ~Manager();
    Graph* getGraph() const;
    void importFiles(const string& pathCities,const string& pathReservoirs,const string& pathStations,const string& pathPipes);
    void resetGraph();
    void resetManager();
    unordered_map<string,Vertex*> getStations() const; // get unordered map of stations
    unordered_map<string,Vertex*> getReservoirs() const; // get unordered map of reservoir
    unordered_map<string,Vertex*> getCities() const; // get unordered map of cities
    Vertex* findVertexInMap(const string& identifier) const;
    int getHowManyEdges() const;

    /* Exercise 2.1 */
    void getEdmondsKarpXCity(vector<string>& cities);
    void getFordFulkersonXCity(vector<string>& cities);
    void getEdmondsKarpAllCities(bool reset = true);
    void getFordFulkersonAllCities(bool reset = true);

    /* Exercise 2.2 */
    void canCityXGetEnoughWaterEK(vector<string>& cities);
    void canCityXGetEnoughWaterFF(vector<string>& cities);
    void canAllCitiesGetEnoughWaterEK();
    void canAllCitiesGetEnoughWaterFF();

    /* Exercise 2.3 */
    void improvePipesHeuristic();
    void improvePipesHeuristicEK();
    void improvePipesHeuristicFF();

    /* Exercise 3.1 */
    void disableEachReservoirEdmondsKarp();
    void disableEachReservoirFordFulkerson();
    void disableSelectedReservoirsEdmondsKarp(vector<string>& reservoirs);
    void disableSelectedReservoirsFordFulkerson(vector<string>& reservoirs);

    /* Exercise 3.2 */
    void disableEachStationEdmondsKarp();
    void disableEachStationFordFulkerson();
    void disableSelectedStationsEdmondsKarp(vector<string>& stations);
    void disableSelectedStationsFordFulkerson(vector<string>& stations);
    vector<pair<string,double>> shutdownStationsGettingDecreaseFlows(vector<pair<string,int>> (Manager::*function)(),vector<string>& stations);

    /* Exercise 3.3 */
    void disableEachPipeEdmondsKarp();
    void disableEachPipeFordFulkerson();
    void disableSelectedPipesEdmondsKarp(vector<Edge*> &pipes);
    void disableSelectedPipesFordFulkerson(vector<Edge*> &pipes);


    /* Extras */
    void topKFlowEdmondsKarpCities(int k);
    void topKFlowFordFulkersonCities(int k);
    void flowRatePerCityEdmondsKarp();
    void flowRatePerCityFordFulkerson();

    vector<int> getMetrics();
};


#endif //PROJECTDA_MANAGER_H