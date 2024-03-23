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
    unordered_map<string,Vertex*> getStations() const; // get unordered map of stations
    unordered_map<string,Vertex*> getReservoirs() const; // get unordered map of reservoir
    unordered_map<string,Vertex*> getCities() const; // get unordered map of cities
    static int parseInt(const string& text);
    Vertex* findVertexInMap(const string& identifier) const;
    Vertex* addSuperSink();
    Vertex* addSuperSource();
    vector<Edge*> bfs_flow(Vertex* superSource,Vertex* superSink);
    int findMinEdge(vector<Edge*> path);


public:
    Manager();
    ~Manager();
    Graph* getGraph() const;
    void importFiles(const string& pathCities,const string& pathReservoirs,const string& pathStations,const string& pathPipes);
    void maxFlowEdmondsKarp();
};


#endif //PROJECTDA_MANAGER_H