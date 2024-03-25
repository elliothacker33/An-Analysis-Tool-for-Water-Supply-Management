//
// Created by up202205140 on 06/03/2024.
//

#ifndef PROJECTDA_MANAGER_H
#define PROJECTDA_MANAGER_H
#include <string>
#include <unordered_map>
#include "../model/Graph/graph.h"

using namespace std;

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
    vector<pair<string,int>> maxFlowEdmondsKarp();
    vector<pair<string,int>> maxFlowFordFulkerson();
    void createCsvFileFlows(const string& path,vector<pair<string,int>>& flows);
    void createCsvFileRates(const string& path,vector<pair<string,double>>& rates);
    void calculateFlowRates(const vector<pair<string, int>>& flows, const string& outputFile);
    void printTopKFlows(vector<pair<string, int>>& flows, int k, const string& outputFile);
    void printFlowMetrics(vector<pair<string, int>>& flows, const string& outputFile);
    void printFlowMetricsOneCity(vector<pair<string, int>>& flows, const string& code, const string& outputFile);
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
    void getEdmondsKarpOneCity(string& code);
    void getEdmondsKarpAllCities();
    void getFordFulkersonOneCity(string& code);
    void getFordFulkersonAllCities();
    void topKFlowEdmondsKarpCities(const int k);
    void topKFlowFordFulkersonCities(const int k);
    void flowRatePerCityEdmondsKarp();
    void flowRatePerCityFordFulkerson();

};


#endif //PROJECTDA_MANAGER_H