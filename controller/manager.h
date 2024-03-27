//
// Created by up202205140 on 06/03/2024.
//

#ifndef PROJECTDA_MANAGER_H
#define PROJECTDA_MANAGER_H
#include <string>
#include <unordered_map>
#include "../model/Graph/graph.h"

using namespace std;
using FlowFunction = vector<Edge*> (*)(Vertex*, Vertex*);
// For complexity Work -> unorderd map complexity is O(1) to access and O(n) to build. Improving factor by 1/n
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
    Vertex* findVertexInMap(const string& identifier) const;
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
    void createCsvFileEnoughWater(const string &path, vector<pair<string, bool>> &enoughWater);
    void createCsvFileRates(const string& path,vector<pair<string,double>>& rates);
    void calculateFlowRates(const vector<pair<string, int>>& flows, const string& outputFile);
    void printTopKFlows(vector<pair<string, int>>& flows, int k, const string& outputFile);
    void printFlowMetrics(vector<pair<string, int>>& flows,vector<string>& chosenCities, const string& outputFile);
    void disableStations(vector<string>& stations);
    bool shutdownStations(vector<string>& codes);
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

    /* Exercise 2.1 */
    void getEdmondsKarpXCity(vector<string>& cities);
    void getFordFulkersonXCity(vector<string>& cities);
    void getEdmondsKarpAllCities();
    void getFordFulkersonAllCities();

    /* Exercise 2.2 */
    void canCityXGetEnoughWaterEK(vector<string>& cities);
    void canCityXGetEnoughWaterFF(vector<string>& cities);
    void canAllCitiesGetEnoughWaterEK();
    void canAllCitiesGetEnoughWaterFF();


    /* Exercise 3.2 */
    void disableEachOneEdmondsKarp();
    void disableSelectedOnes(vector<string>& stations);
    vector<pair<string,double>> shutdownStationsGettingDecreaseFlows(vector<string>& stations);

    /* Extras */
    void topKFlowEdmondsKarpCities(int k);
    void topKFlowFordFulkersonCities(int k);
    void flowRatePerCityEdmondsKarp();
    void flowRatePerCityFordFulkerson();

};


#endif //PROJECTDA_MANAGER_H