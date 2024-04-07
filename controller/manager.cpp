/**
* @file manager.cppp
* @brief This file contains the manager implementation.
 */

// Project headers
#include "manager.h"
// Standard Library Headers
#include <algorithm>
#include <climits>
#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <vector>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

/* ----- Auxiliary functions and constructors/destructors ---- */
/**
 * @brief Manager constructor
 */
Manager::Manager() {
    graph = new Graph();
}

/**
 * @brief Manager destructor
 * Deletes graph associated with manager
 */
Manager::~Manager() {
    delete graph;
}
/**
 * @brief Deletes graph, and creates a new one, to reset manager in Menu.
 */
void Manager::resetManager() {
    delete this->graph;
    this->graph = new Graph();
    cities.clear();
    reservoirs.clear();
    stations.clear();
}

/**
 * @brief Gets the number of edges in the graph
 */
int Manager::getHowManyEdges() const {
    int sum = 0;
    for (auto v : graph->getVertexSet()) {
        sum+= v->getAdj().size();
    }
    return sum;
}

/**
 * @brief Resets the graph to it's initial state
 */
void Manager::resetGraph() {
    Vertex* superSource = findVertexInMap("SR");
    Vertex* superSink = findVertexInMap("SS");

    vector<Edge*> deleteEdges;

    // Reset normal edges
    for (const auto v : graph->getVertexSet()) {
        v->setPath(nullptr);
        v->setVisited(false);
        v->setEnabled(true);
        for (auto e : v->getAdj()) {
            e->setFlow(0);
            e->setEnabled(true);
            e->setReverseEdge(nullptr);
            if (e->getType() == "residual")
                deleteEdges.push_back(e);
        }
    }

    // Delete residual edges in the graph
    for(const auto e : deleteEdges) {
        graph->removeEdge(e);
    }
    // Delete superSource
    if (superSource != nullptr) {
        graph->removeVertex(superSource);
        reservoirs.erase("SR");
    }
    // Delete superSink
    if(superSink != nullptr) {
        graph->removeVertex(superSink);
        cities.erase("SS");
    }

}
/**
 * @brief Getter function to access graph outside of manager
 */
Graph* Manager::getGraph() const {
    return graph;
}

/**
 * @brief Get access to cities faster in O(1) time.
 * @return unordered_map with (city,vertex pointer)
 */
unordered_map<string,Vertex*> Manager::getCities() const{
    return cities;
}
/**
 * @brief Get access to reservoirs faster in O(1) time.
 * @return unordered_map with (reservoir,vertex pointer)
 */
unordered_map<string, Vertex *> Manager::getReservoirs() const {
    return reservoirs;
}
/**
 * @brief Get access to stations faster in O(1) time.
 * @return unordered_map with (stations,vertex pointer)
 */
unordered_map<string, Vertex*> Manager::getStations() const {
    return stations;
}

/**
 * @brief Retrieve the Vertex* associated with code of vertex.
 * @param identifier is a code of a Vertex that can be of 3 types (city,reservoir,station)
 * @return (Vertex*) pointer to vertex in the graph.
 */
Vertex* Manager::findVertexInMap(const string& identifier) const {
    if (const auto cityIt = cities.find(identifier); cityIt != cities.end()) {
        return cityIt->second;
    }
    if (const auto reservoirIt = reservoirs.find(identifier); reservoirIt!= reservoirs.end()) {
        return reservoirIt->second;
    }
    if (const auto stationIt = stations.find(identifier); stationIt!= stations.end()) {
        return stationIt->second;
    }
    return nullptr;
}
/*------------- Csv methods -----------------------------*/

/**
 * Get the
 * @param path
 * @param flows
 */
void Manager::createCsvFileFlows(const string &path,vector<pair<string,int>>& flows) {
    ofstream outputCSV(path);

    if (!outputCSV.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        exit(EXIT_FAILURE);
    }

    outputCSV << "Name,Code,Flow" << endl;

    for (const auto flow : flows) {
        outputCSV << Graph::getName(findVertexInMap(flow.first)) << "," << flow.first << "," << flow.second << endl;
    }

    outputCSV.flush();
    outputCSV.close();
}

void Manager::createCsvFileDisable(const string &path, vector<pair<string, bool> > &disable) {
    ofstream outputCSV(path);

    if (!outputCSV.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        exit(EXIT_FAILURE);
    }

    outputCSV << "Code,CanBeDisabled" << endl;

    for (const auto station : disable) {
        if (station.second)
            outputCSV << station.first << "," << "yes" << endl;
        else outputCSV << station.first << "," << "no" << endl;
    }

    outputCSV.flush();
    outputCSV.close();
}

void Manager::createCsvFilePipesDisable(const string &path, vector<pair<Edge*, bool>> &disable) {
    ofstream outputCSV(path);

    if (!outputCSV.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        exit(EXIT_FAILURE);
    }

    outputCSV << "Origin,Destination,CanBeDisabled" << endl;

    for (const auto pipe : disable) {
        if (pipe.second)
            outputCSV << Graph::getCode(pipe.first->getOrigin()) << "," << Graph::getCode(pipe.first->getDest()) << "," << "yes" << endl;
        else outputCSV << Graph::getCode(pipe.first->getOrigin()) << "," << Graph::getCode(pipe.first->getDest()) << "," << "no" << endl;
    }

    outputCSV.flush();
    outputCSV.close();
}



void Manager::createCsvFileEnoughWater(const string &path, vector<pair<string, bool>>& enoughWater) {
    ofstream outputCSV(path);

    if (!outputCSV.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        exit(EXIT_FAILURE);
    }

    outputCSV << "Code,EnoughWater" << endl;

    for (const auto city : enoughWater) {
        if (city.second)
            outputCSV << city.first << "," << "yes" << endl;
        else outputCSV << city.first << "," << "no" << endl;
    }

    outputCSV.flush();
    outputCSV.close();
}


void Manager::createCsvFileRates(const string &path, vector<pair<string, double> > &rates) {
    ofstream outputCSV(path);

    if (!outputCSV.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        exit(EXIT_FAILURE);
    }

    outputCSV << "Name,Code,Rate" << endl;

    for (const auto rate : rates) {
        if (rate.second < 0) {
            outputCSV << Graph::getName(findVertexInMap(rate.first)) << "," << rate.first << "," << rate.second << "%" << endl;
        }
        else {
            outputCSV << Graph::getName(findVertexInMap(rate.first)) << "," << rate.first << "," << rate.second << "%" << endl;
        }
    }

    outputCSV.flush();
    outputCSV.close();
}

/*------------- Parsing methods ------------------------ */

void Manager::importFiles(const string& pathCities,const string& pathReservoirs,const string& pathStations,const string& pathPipes) {
    resetManager();
    importCities(pathCities);
    importStations(pathStations);
    importReservoirs(pathReservoirs);
    importPipes(pathPipes);
}

int Manager::parseInt(const string& text) const {
    string number_string;
    for (const auto i : text) {
        if (i != '\"' && i != ',')
            number_string.push_back(i);
    }
    const int number_integer=stoi(number_string);
    return number_integer;
}

void Manager::importCities(const string& pathCities){
    fstream fin;
    fin.open(pathCities,ios::in);

    if (!fin.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        exit(EXIT_FAILURE);
    }

    vector<string> row;
    string line;
    string word;
    getline(fin,line);
    while (getline(fin,line)) {
        row.clear(); // Empty last line
        if (line.at(line.size()-1) == '\r') {
            line = line.substr(0,line.size()-1);
        }
        stringstream ss(line);
        while(getline(ss,word,',')){

            if (!word.empty()) {
                row.push_back(word);
            }
        }
        if (row.empty()) {
            continue;
        }
        int population;
        if (row[4].at(0) == '\"') {
            string populationStr = row[4] + row[5];
            population = parseInt(populationStr);
        }
        else {
            population = stoi(row[4]);
        }

        Vertex* city = new City(row[0],stoi(row[1]),row[2],stoi(row[3]),population);
        if (auto [_, success] = cities.insert({row[2],city});success) {
            graph->addVertex(city);
        }

    }

    fin.close();
}

void Manager::importReservoirs(const string& pathReservoirs)  {
    fstream fin;
    fin.open(pathReservoirs,ios::in);
    // Check if the file is open
    if (!fin.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        exit(EXIT_FAILURE);
    }

    vector<string> row;
    string line;
    string word;
    getline(fin,line);
    while (getline(fin,line)) {
        row.clear(); // Empty last line
        if (line.at(line.size()-1) == '\r') {
            line = line.substr(0,line.size()-1);
        }
        stringstream ss(line);
        while(getline(ss,word,',')){

            if (!word.empty()) {
                row.push_back(word);
            }
        }
        if (row.empty()) {
            continue;
        }

        Vertex* reservoir = new Reservoir(row[0],row[1],stoi(row[2]),row[3],stoi(row[4]));
        if (auto [_, success] = reservoirs.insert({row[3], reservoir});success) {
           graph->addVertex(reservoir);
        }
    }
    fin.close();

}

void Manager::importStations(const string& pathStations) {
    fstream fin;
    fin.open(pathStations,ios::in);
    if (!fin.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        exit(EXIT_FAILURE);
    }
    vector<string> row;
    string line;
    string word;
    getline(fin,line);
    while (getline(fin,line)) {
        row.clear(); // Empty last line
        if (line.at(line.size()-1) == '\r') {
            line = line.substr(0,line.size()-1);
        }
        stringstream ss(line);

        while(getline(ss,word,',')){

            if (!word.empty()) {
                row.push_back(word);
            }
        }
        if (row.empty()) {
            continue;
        }
        Vertex* station = new Station(stoi(row[0]),row[1]);
        if (auto [_, success] = stations.insert({row[1],station});success) {
            graph->addVertex(station);
        }


    }
    fin.close();
}

void Manager::importPipes(const string& pathPipes) const {
    fstream fin;
    fin.open(pathPipes,ios::in);
    if (!fin.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        exit(EXIT_FAILURE);
    }
    vector<string> row;
    string line;
    string word;
    getline(fin,line);
    while (getline(fin,line)) {
        row.clear();
        if (line.at(line.size()-1) == '\r') {
            line = line.substr(0,line.size()-1);
        }
        stringstream ss(line);
        while(getline(ss,word,',')){
            if (!word.empty()) {
                row.push_back(word);
            }
        }

        Vertex* orig = findVertexInMap(row[0]);
        Vertex* dest = findVertexInMap(row[1]);

        if (stoi(row[3]) == 0) {
            graph->addEdge(orig,dest,stoi(row[2]),"normal");
        }
        else if (stoi(row[3]) == 1) {
            graph->addEdge(orig,dest,stoi(row[2]),"normal");
            graph->addEdge(dest,orig,stoi(row[2]),"normal");
        }
        else {
            cerr << "This value is not accepted for direction of edges" << endl;
            exit(EXIT_FAILURE);
        }

    }
    fin.close();
}
/*-----------------Max Flow Algorithms---------------------- */

Vertex* Manager::addSuperSource() {
    Vertex* superSource = new Reservoir("Super Reservoir","_",-1,"SR",-1);
    graph->addVertex(superSource);
    reservoirs.insert({"SR",superSource});
    for (const auto v : graph->getVertexSet()) {
        if (v->getType() == 'R' && dynamic_cast<Reservoir*>(v)->getCode() != "SR") {
            const auto reservoir = dynamic_cast<Reservoir*>(v);
            graph->addEdge(superSource,v,reservoir->getMaxDelivery(),"residual");
        }
    }
    return superSource;
}

Vertex* Manager::addSuperSink() {
    Vertex* superSink = new City("Super Sink",-1,"SS",INT_MAX,-1);
    graph->addVertex(superSink);
    cities.insert({"SS",superSink});

    for (const auto v : graph->getVertexSet()) {
        if (v->getType() == 'C'  && dynamic_cast<City*>(v)->getCode() != "SS") {
            const auto city = dynamic_cast<City*>(v);
            graph->addEdge(v,superSink,city->getDemand(),"residual");
        }
    }
    return superSink;
}

int Manager::findMinEdge(const vector<Edge*>& path) {
    int flow = numeric_limits<int>::max();
    for (auto e: path) {
        if(e->getFlow() < flow) {
            flow = e->getFlow();
        }
    }
    return flow;
}

vector<Edge*> Manager::dfs_flow(Vertex *superSource, Vertex *superSink) {
    if (superSource == nullptr || superSink == nullptr) {
        cerr << "Super Source or/and Super Sink not found";
        exit(EXIT_FAILURE);
    }

    for (const auto v : graph->getVertexSet()) {
        v->setVisited(false);
    }

    vector<Edge*> path;
    dfs_helper(superSource, superSink, path);
    return path;
}

bool Manager::dfs_helper(Vertex *currentVertex, Vertex *superSink, vector<Edge*> &path) {
    currentVertex->setVisited(true);

    if (currentVertex == superSink) {
        return true;
    }

    for (const auto e : currentVertex->getAdj()) {
        if (Vertex* dest = e->getDest(); !dest->isVisited() && dest->isEnabled() && e->isEnabled() && e->getFlow() > 0) {
            dest->setPath(e);
            if (dfs_helper(dest, superSink, path)) {
                path.push_back(e);
                return true;
            }
            dest->setPath(nullptr);
        }
    }
    currentVertex->setVisited(false);
    return false;
}

vector<Edge*> Manager::bfs_flow(Vertex* superSource, Vertex* superSink) {
    vector<Edge*> path;
    if (superSource == nullptr || superSink == nullptr) {
        cerr << "Super Source or/and Super Sink not found";
    }

    for (const auto v : graph->getVertexSet()) {
        v->setVisited(false);
    }

    queue<Vertex*> q;
    q.push(superSource);
    superSource->setVisited(true);

    while(!q.empty()) {
        const auto v = q.front();
        q.pop();
        for (const auto e : v->getAdj()) {
            if(const auto dest = e->getDest();e->getOrigin()->isEnabled() && !dest->isVisited() && dest->isEnabled() && e->isEnabled()  && (e->getFlow() > 0)) {
                q.push(dest);
                dest->setVisited(true);
                dest->setPath(e);
                if (Graph::getCode(dest) == "SS") {
                    Vertex* vpath = dest;
                    while(vpath->getPath() != nullptr) {
                        path.push_back(vpath->getPath());
                        vpath = vpath->getPath()->getOrigin();
                    }
                    return path;
                }

            }
        }
    }
    return path;
}

vector<pair<string,int>> Manager::maxFlow(vector<Edge*> (Manager::*explore_paths)(Vertex*,Vertex*)) {
    Vertex* superSource = addSuperSource();
    Vertex* superSink = addSuperSink();

    for (const auto v: graph->getVertexSet()) {
        for (const auto e : v->getAdj()) {
            e->setFlow(e->getCapacity());
        }
    }

    vector<Edge*> path;
    while(!(path = (this->*explore_paths)(superSource, superSink)).empty()) {
        const int newFlow = findMinEdge(path);
        for (const auto e : path) {
            if(e->getReverseEdge() == nullptr) {
                // Residual (BI)
                const auto dest = findVertexInMap(Graph::getCode(e->getDest()));
                bool value = false;
                for (const auto i : dest->getAdj()) {
                    if (Graph::getCode(i->getDest()) == Graph::getCode(e->getOrigin())) {
                        i->setReverseEdge(e);
                        e->setReverseEdge(i);
                        value = true;
                        break;
                    }
                }
                if (!value) {
                    const auto edg = graph->addEdge(e->getDest(),e->getOrigin(),e->getCapacity(),"residual");
                    edg->setFlow(0);
                    edg->setReverseEdge(e);
                    e->setReverseEdge(edg);

                }
            }
            e->setFlow(e->getFlow()-newFlow);
            e->getReverseEdge()->setFlow(e->getReverseEdge()->getFlow() + newFlow);

        }
        path.clear();
    }
    // Final Calculation of paths
    vector<pair<string,int>> result;
    for (const auto n : graph->getVertexSet()) {
        if (n->getType() == 'C' && Graph::getCode(n) != "SS") {
            int sumFlow = 0;
            for (const auto e : n->getIncoming()) {
                if (e->getType() == "residual")
                    sumFlow+= e->getFlow();
            }
            result.push_back(make_pair(Graph::getCode(n), sumFlow));
        }
    }
    return result;
}

vector<pair<string,int>> Manager::maxFlowEdmondsKarp() {
    return maxFlow(&Manager::bfs_flow);
}

vector<pair<string,int>> Manager::maxFlowFordFulkerson() {
    return maxFlow(&Manager::dfs_flow);
}
/*---------------------Exercise 2.1 -----------------------*/

void Manager::printFlowMetrics(vector<pair<string, int>>& flows,vector<string>& chosenCities, const string& outputFile) {

    vector<pair<string, int>> results;
    int total_flow = 0;
    for (const auto& flow : flows) {
        if (find(chosenCities.begin(),chosenCities.end(),flow.first) != chosenCities.end()) {
            total_flow += flow.second;
            cout << "Flow for city with code " << flow.first << ": " << flow.second << endl;
            results.push_back(make_pair(flow.first,flow.second));
        }
    }
    cout << "The total flow is: " << total_flow << endl;

    string filename = outputFile;
    createCsvFileFlows(filename, results);
}

void Manager::getEdmondsKarpXCity(vector<string>& cities) {
    auto flows = maxFlowEdmondsKarp();
    printFlowMetrics(flows, cities, "../data/results/results_21_EK.csv");
    resetGraph();
}

void Manager::getFordFulkersonXCity(vector<string>& cities) {
    auto flows = maxFlowFordFulkerson();
    printFlowMetrics(flows, cities, "../data/results/results_21_FF.csv");
    resetGraph();
}


void Manager::getEdmondsKarpAllCities(bool reset) {
    auto flows = maxFlowEdmondsKarp();
    vector<string> cities;
    for (auto flow: flows) {
        cities.push_back(flow.first);
    }
    if(reset) {
        printFlowMetrics(flows, cities, "../data/results/results_21_EK.csv");
        resetGraph();
    }
}

void Manager::getFordFulkersonAllCities(bool reset) {
    auto flows = maxFlowFordFulkerson();
    vector<string> cities;
    for (auto flow: flows) {
        cities.push_back(flow.first);
    }
    if(reset) {
        printFlowMetrics(flows, cities, "../data/results/results_21_FF.csv");
        resetGraph();
    }
}


/*---------------------Exercise 2.2 -----------------------*/

vector<pair<string,bool>> Manager::canCityGetEnoughWater(vector<string>& codes,vector<pair<string,int>>& flows) {
    vector<pair<string,bool>> results;
    for (const auto& flow : flows) {
        if (find(codes.begin(),codes.end(),flow.first) != codes.end()) {
            int demand = dynamic_cast<City*>(findVertexInMap(flow.first))->getDemand();
            if (demand > flow.second) {
                int deficit = demand - flow.second;
                cout << ANSI_COLOR_RED << "The city with code " << flow.first << " can't get enough flow. flow : " << flow.second << " demand: " << demand << " deficit: " << deficit << ANSI_COLOR_RESET << endl;
                results.push_back(make_pair(flow.first,false));
            }
            else {
                cout << "The city with code " << flow.first << " can get enough flow. flow : " << flow.second << " demand: " << demand << endl;
                results.push_back(make_pair(flow.first,true));
            }
        }
    }
    return results;
}

void Manager::canCityXGetEnoughWaterEK(vector<string>& cities) {
    auto flows = maxFlowEdmondsKarp();
    auto results = canCityGetEnoughWater(cities,flows);

    string path = "../data/results/results_cityXEnoughWaterEK.csv";
    createCsvFileEnoughWater(path,results);
    resetGraph();
}

void Manager::canCityXGetEnoughWaterFF(vector<string>& cities) {
    auto flows = maxFlowFordFulkerson();
    auto results = canCityGetEnoughWater(cities,flows);
    string path = "../data/results/results_cityXEnoughWaterFF.csv";
    createCsvFileEnoughWater(path,results);
    resetGraph();
}

void Manager::canAllCitiesGetEnoughWaterEK() {
    auto flows = maxFlowEdmondsKarp();
    vector<string> cities;
    for (auto n : getCities()) {
        cities.push_back(n.first);
    }
    auto results = canCityGetEnoughWater(cities,flows);
    string path = "../data/results/results_allCitiesEnoughWaterEK.csv";
    createCsvFileEnoughWater(path,results);
    resetGraph();
}

void Manager::canAllCitiesGetEnoughWaterFF() {
    auto flows = maxFlowFordFulkerson();
    vector<string> cities;
    for (auto n : getCities()) {
        cities.push_back(n.first);
    }
    auto results = canCityGetEnoughWater(cities,flows);
    string path = "../data/results/results_allCitiesEnoughWaterFF.csv";
    createCsvFileEnoughWater(path,results);
    resetGraph();
}

/* -------------------Exercise 2.3----------------------------- */

void Manager::improvePipesHeuristic() {

    int mn = -1;
    int n_mn = -1;
    int n_mx = -1;

    vector<int> control_metrics = getMetrics();

    int sum = 0;

    for(auto a: graph->getVertexSet()){

        if(a->isVisited())continue;
        a->setVisited(true);

        if(a->getType() == 'R'){
            sum += dynamic_cast<Reservoir*>(a)->getMaxDelivery();
        }

        for(auto b: a->getAdj()){

            int val = b->getCapacity();

            if(mn == -1){
                mn = val;
            }else if(mn > val){
                mn = val;
            }
        }
    }
    graph->resetVisited();
    n_mn = mn;
    n_mx = mn;

    for(auto a: graph->getVertexSet()) {

        if (a->isVisited())continue;
        a->setVisited(true);
        for(auto b: a->getAdj()){

            int cap = b->getCapacity();

            if(cap < mn){

                if(sum < cap){
                    b->setFlow(sum);
                    if(n_mn > sum){n_mn = sum;}
                    sum = 0;
                    break;
                }else{
                    b->setFlow(cap);
                    if(n_mn > cap){n_mn = cap;}
                    sum -= cap;
                }

            }
            else {
                if(sum < mn){
                    b->setFlow(sum);
                    n_mn = sum;
                    sum = 0;
                    break;
                }else{
                    b->setFlow(mn);
                    sum -= mn;
                }
            }

        }

        if(sum == 0)break;
    }

    while(sum != 0) {

        graph->resetVisited();
        n_mx += 1;

        for (auto a: graph->getVertexSet()) {

            if (a->isVisited())continue;
            a->setVisited(true);
            for (auto b: a->getAdj()) {

                if(b->getCapacity() > b->getFlow()) {
                    b->setFlow(b->getFlow() + 1);
                    sum -= 1;
                }
                if(sum == 0)break;
            }
            if(sum == 0)break;
        }
    }

    vector<int> new_metrics = getMetrics();

    cout << endl;
    cout << "Control Metrics:" << endl;
    cout << "Min Flow: " << control_metrics.at(0) << endl;
    cout << "Max Flow: " << control_metrics.at(1) << endl;
    cout << "Average Flow: " << control_metrics.at(2) << endl;
    cout << "Max Difference between Flow and Capacity: " << control_metrics.at(3) << endl;
    cout << "Average Difference between Flow and Capacity: " << control_metrics.at(4) << endl;
    cout << "Variance of the difference between Flow and Capacity : " << control_metrics.at(5) << endl;

    cout << endl;
    cout << "New Metrics:" << endl;
    cout << "Min Flow: " << new_metrics.at(0) << endl;
    cout << "Max Flow: " << new_metrics.at(1) << endl;
    cout << "Average Flow: " << new_metrics.at(2) << endl;
    cout << "Max Difference between Flow and Capacity: " << new_metrics.at(3) << endl;
    cout << "Average Difference between Flow and Capacity: " << new_metrics.at(4) << endl;
    cout << "Variance of the difference between Flow and Capacity : " << new_metrics.at(5) << endl;

    return;
}

void Manager::improvePipesHeuristicEK() {
    return;
}

void Manager::improvePipesHeuristicFF() {
    return;
}
/* -------------------Exercise 3.1----------------------------- */

void Manager::disableReservoirs(vector<string> &reservoirs) {
    for(string code : reservoirs) {
        findVertexInMap(code)->setEnabled(false);
    }
}
// TODO: Under Construction
/*
void Manager::dfs_disable_helper(Vertex* reservoir) {
    reservoir->setVisited(true);
    for (auto e : reservoir->getAdj()) {
        if (Vertex* dest = e->getDest(); !dest->isVisited() && e->getFlow() < e->getCapacity()) {
            e->getReverseEdge()->setFlow(e->getReverseEdge()->getFlow() - e->getCapacity());
            e->setFlow(e->getFlow() + e->getCapacity());
            dfs_disable(dest);
        }
    }
}
void Manager::dfs_disable(Vertex *reservoir) {
    for (auto v : graph->getVertexSet()) {
        v->setVisited(false);
    }

    dfs_disable_helper(reservoir);
}

vector<pair<string,int>> Manager::graphChangeFlowsAfterReservoirsDisabled(vector<string> &reservoirs) {

    // Change graph doing a dfs visit
    for (auto r : reservoirs) {
        Vertex* v = findVertexInMap(r);
        dfs_disable(v);
    }

    // Calculate Results of flow.
    vector<pair<string,int>> result;
    for (const auto n : graph->getVertexSet()) {
        if (n->getType() == 'C' && Graph::getCode(n) != "SS") {
            int sumFlow = 0;
            for (const auto e : n->getIncoming()) {
                if (e->getType() == "residual")
                    sumFlow+= e->getFlow();
            }
            result.push_back(make_pair(Graph::getCode(n), sumFlow));
        }
    }
    return result;
}
*/

bool Manager::shutdownReservoirs(vector<pair<string, int>> (Manager::*flowfunction)(), vector<string> &reservoirs) {
    auto beforeFlows = (this->*flowfunction)();

    // Calculate flow before disable
    int beforeTotalFlow = 0;
    for (const auto& flow : beforeFlows)
        beforeTotalFlow += flow.second;

    // Calculate total demand
    int totalDemand = 0;
    for (const auto& v: getCities()) {
        totalDemand+=dynamic_cast<City*>(v.second)->getDemand();
    }
    vector<pair<string,int>> afterFlows;
    if (totalDemand <= beforeTotalFlow){
        // Just a need a dfs to deacrease values
        //afterFlows = graphChangeFlowsAfterReservoirsDisabled(reservoirs);
    }
    else {
        resetGraph();
        disableReservoirs(reservoirs);
        afterFlows = (this->*flowfunction)();
    }


    // Calculate flow after disable
    int afterTotalFlow = 0;
    for (const auto& flow : afterFlows)
        afterTotalFlow += flow.second;

    // Check if the network was affected
    if (afterTotalFlow == beforeTotalFlow) {
        cout << "The network was not affected after removing reservoirs: ";
        for (const string& code : reservoirs) {
            cout << code << ", ";
        }
        cout << endl;
        resetGraph();
        return true;
    }

    // Calculate the percentage decline for each city
    vector<pair<string, double>> percentageDecline;
    for (const auto& beforeFlow : beforeFlows) {
        string cityCode = beforeFlow.first;
        int beforeFlowAmount = beforeFlow.second;
        double afterFlowAmount = 0;

        // Find the flow amount after removing the stations
        for (const auto& afterFlow : afterFlows) {
            if (afterFlow.first == cityCode) {
                afterFlowAmount = afterFlow.second;
                break;
            }

        }

        // Calculate the percentage decline in flow
        double declinePercentage = (beforeFlowAmount - afterFlowAmount) / beforeFlowAmount * 100;
        percentageDecline.push_back(make_pair(cityCode, declinePercentage));
    }

    cout << "Percentage decline in flow for each city after removing reservoirs:" << endl;
    for (const auto& entry : percentageDecline) {
        if(entry.second == 0.0) {
            cout << "City code: " << entry.first << ", Percentage Decline: " << entry.second << "%" << endl;
        }
        else {
            cout << "City code: " << entry.first << ", Percentage Decline: -" << entry.second << "%" << endl;
        }
    }
    resetGraph();
    return false;

}

void Manager::disableEachReservoirEdmondsKarp() {
    vector<pair<string,bool>> can_be_disabled;
    auto it = reservoirs.begin();
    for (; it != reservoirs.end(); ++it) {
        vector<string> codes;
        codes.push_back(it->first);
        can_be_disabled.push_back(make_pair(it->first, shutdownReservoirs(&Manager::maxFlowEdmondsKarp,codes)));
    }
    string path = "../data/results/results_disabled_reservoirs_EK.csv";
    createCsvFileDisable(path,can_be_disabled);
}

void Manager::disableEachReservoirFordFulkerson() {
    return;
}

void Manager::disableSelectedReservoirsEdmondsKarp(vector<std::string> &reservoirs) {
    return;
}
void Manager::disableSelectedReservoirsFordFulkerson(vector<std::string> &reservoirs) {
    return;
}


/* -------------------Exercise 3.2----------------------------- */

void Manager::disableStations(vector<string>& stations) {
    for(string code : stations) {
        Vertex* v = findVertexInMap(code);
        v->setEnabled(false);
    }
}

bool Manager::shutdownStations(vector<pair<string,int>> (Manager::*flowfunction)(),vector<string>& codes) {
    // Calculate total flow before removing the stations
    auto beforeFlows = (this->*flowfunction)();
    int beforeTotalFlow = 0;
    for (const auto& flow : beforeFlows)
        beforeTotalFlow += flow.second;
    cout << "Total flow before removing stations: " <<  beforeTotalFlow << endl;

    // Reset graph and disable stations
    resetGraph();
    disableStations(codes);

    // Calculate total flow after removing the stations
    auto afterFlows = (this->*flowfunction)();
    int afterTotalFlow = 0;
    for (const auto& flow : afterFlows)
        afterTotalFlow += flow.second;
    cout << "Total flow after removing stations: " << afterTotalFlow << endl;

    // Check if the network was affected
    if (afterTotalFlow == beforeTotalFlow) {
        cout << "The network was not affected after removing: ";
        for (const string& code : codes) {
            cout << code << ", ";
        }
        cout << endl;
        resetGraph();
        return true;
    }

    cout << "Percentage decline in flow for each city after removing stations:" << endl;

    // Calculate the percentage decline for each city
    vector<pair<string, double>> percentageDecline;
    for (const auto& beforeFlow : beforeFlows) {
        string cityCode = beforeFlow.first;
        int beforeFlowAmount = beforeFlow.second;
        double afterFlowAmount = 0;

        // Find the flow amount after removing the stations
        for (const auto& afterFlow : afterFlows) {
            if (afterFlow.first == cityCode) {
                afterFlowAmount = afterFlow.second;
                break;
            }
        }

        // Calculate the percentage decline in flow
        double declinePercentage = (beforeFlowAmount - afterFlowAmount) / beforeFlowAmount * 100;
        percentageDecline.push_back(make_pair(cityCode, declinePercentage));
        cout << "City code: " << cityCode << "Before flow : " << beforeFlowAmount << ", After flow : " << afterFlowAmount << ", Decline percentage : -" << declinePercentage << "%" << endl;

    }
    resetGraph();
    return false;
}

void Manager::disableEachStationEdmondsKarp() {
    vector<pair<string,bool>> can_be_disabled;
    auto it = stations.begin();
    for (; it != stations.end(); ++it) {
        vector<string> codes;
        codes.push_back(it->first);
        can_be_disabled.push_back(make_pair(it->first, shutdownStations(&Manager::maxFlowEdmondsKarp,codes)));
    }
    string path = "../data/results/results_disabled_stations_EK.csv";
    createCsvFileDisable(path,can_be_disabled);

}

void Manager::disableEachStationFordFulkerson() {
    vector<pair<string,bool>> can_be_disabled;
    auto it = stations.begin();
    for (; it != stations.end(); ++it) {
        vector<string> codes;
        codes.push_back(it->first);
        can_be_disabled.push_back(make_pair(it->first, shutdownStations(&Manager::maxFlowFordFulkerson,codes)));
    }
    string path = "../data/results/results_disabled_stations_FF.csv";
    createCsvFileDisable(path,can_be_disabled);
}

vector<pair<string, double>> Manager::shutdownStationsGettingDecreaseFlows(vector<pair<string,int>> (Manager::*flowfunction)(),vector<string>& codes) {
    vector<pair<string, double>> percentageDecline;
    // Calculate total flow before removing the stations
    auto beforeFlows = (this->*flowfunction)();
    int beforeTotalFlow = 0;
    for (const auto& flow : beforeFlows)
        beforeTotalFlow += flow.second;
    cout << "Total flow before removing stations: " <<  beforeTotalFlow << endl;

    // Reset graph and disable stations
    resetGraph();
    disableStations(codes);

    // Calculate total flow after removing the stations
    auto afterFlows = (this->*flowfunction)();
    int afterTotalFlow = 0;
    for (const auto& flow : afterFlows)
        afterTotalFlow += flow.second;
    cout << "Total flow after removing stations: " << afterTotalFlow << endl;

    // Check if the network was affected
    if (afterTotalFlow == beforeTotalFlow) {
        cout << "The network was not affected after removing: ";
        for (const string& code : codes) {
            percentageDecline.push_back(make_pair(code,0.0));
            cout << code << ", ";
        }
        cout << endl;
        resetGraph();
    }

    // Calculate the percentage decline for each city
    cout << "Percentage decline in flow for each city after removing stations:" << endl;

    for (const auto& beforeFlow : beforeFlows) {
        string cityCode = beforeFlow.first;
        int beforeFlowAmount = beforeFlow.second;
        double afterFlowAmount = 0;

        // Find the flow amount after removing the stations
        for (const auto& afterFlow : afterFlows) {
            if (afterFlow.first == cityCode) {
                afterFlowAmount = afterFlow.second;
                break;
            }
        }

        // Calculate the percentage decline in flow
        double declinePercentage = -(beforeFlowAmount - afterFlowAmount) / beforeFlowAmount * 100;
        percentageDecline.push_back(make_pair(cityCode, declinePercentage));
        cout << "City code: " << cityCode << "Before flow : " << beforeFlowAmount << ", After flow : " << afterFlowAmount << ", Decline percentage : -" << declinePercentage << "%" << endl;

    }

    resetGraph();

    return percentageDecline;
}

void Manager::disableSelectedStationsEdmondsKarp(vector<string>& stations) {
    vector<pair<string,double>> decreased = shutdownStationsGettingDecreaseFlows(&Manager::maxFlowEdmondsKarp,stations);
    string path = "../data/results/results_decrease_after_disabled_stationsEK.csv";
    createCsvFileRates(path,decreased);
}

void Manager::disableSelectedStationsFordFulkerson(vector<string> &stations) {
    vector<pair<string,double>> decreased = shutdownStationsGettingDecreaseFlows(&Manager::maxFlowFordFulkerson,stations);
    string path = "../data/results/results_decrease_after_disabled_stationsFF.csv";
    createCsvFileRates(path,decreased);
}

/* ------------------- Exercise 3.3 ----------------------------- */

void Manager::disablePipes(vector<Edge*>& pipes) {
    for(auto p: pipes) {
        p->setEnabled(false);
        if (p->getReverseEdge() != nullptr){
            p->getReverseEdge()->setEnabled(false);
        }
    }
}

bool Manager::shutdownPipes(vector<pair<string,int>> (Manager::*flowfunction)(),vector<Edge*> &pipes) {
      // Calculate total flow before removing the stations
    auto beforeFlows = (this->*flowfunction)();
    int beforeTotalFlow = 0;
    for (const auto& flow : beforeFlows)
        beforeTotalFlow += flow.second;

    // Reset graph and disable stations
    resetGraph();
    disablePipes(pipes);

    // Calculate total flow after removing the stations
    auto afterFlows = (this->*flowfunction)();
    int afterTotalFlow = 0;
    for (const auto& flow : afterFlows)
        afterTotalFlow += flow.second;

    // Check if the network was affected
    if (afterTotalFlow == beforeTotalFlow) {
        cout << "The network was not affected after removing pipes: " << endl;
        for (const auto pipe : pipes) {
            cout << "Origin: " <<  Graph::getCode(pipe->getOrigin()) << " Destination: " << Graph::getCode(pipe->getDest()) << endl;
        }
        cout << endl;
        resetGraph();
        return true;
    }

    // Calculate the percentage decline for each city
    vector<pair<string, double>> percentageDecline;
    for (const auto& beforeFlow : beforeFlows) {
        string cityCode = beforeFlow.first;
        int beforeFlowAmount = beforeFlow.second;
        double afterFlowAmount = 0;

        // Find the flow amount after removing the stations
        for (const auto& afterFlow : afterFlows) {
            if (afterFlow.first == cityCode) {
                afterFlowAmount = afterFlow.second;
                break;
            }
        }

        // Calculate the percentage decline in flow
        double declinePercentage = (beforeFlowAmount - afterFlowAmount) / beforeFlowAmount * 100;
        percentageDecline.push_back(make_pair(cityCode, declinePercentage));
    }

    cout << "Percentage decline in flow for each city after removing pipes: " << endl;
    for (const auto pipe : pipes) {
        cout << "Origin: " <<  Graph::getCode(pipe->getOrigin()) << " Destination: " << Graph::getCode(pipe->getDest()) << endl;

    }
    for (const auto& entry : percentageDecline) {
        if(entry.second == 0.0) {
            cout << "City code: " << entry.first << ", Percentage Decline: " << entry.second << "%" << endl;
        }
        else {
            cout << "City code: " << entry.first << ", Percentage Decline: -" << entry.second << "%" << endl;
        }
    }
    cout << endl;
    resetGraph();

    return false;
}

void Manager::disableEachPipeEdmondsKarp() {
    vector<pair<Edge*,bool>> can_be_disabled;
    for (auto v : graph->getVertexSet()) {
        for (auto e : v->getAdj()) {
            vector<Edge*> pipes;
            pipes.push_back(e);
            can_be_disabled.push_back(make_pair(e, shutdownPipes(&Manager::maxFlowEdmondsKarp,pipes)));
        }
    }
    string path = "../data/results/results_can_pipe_be_disabled_EK.csv";
    createCsvFilePipesDisable(path,can_be_disabled);
}

void Manager::disableEachPipeFordFulkerson() {
    vector<pair<Edge*,bool>> can_be_disabled;
    for (auto v : graph->getVertexSet()) {
        for (auto e : v->getAdj()) {
            vector<Edge*> pipes;
            pipes.push_back(e);
            can_be_disabled.push_back(make_pair(e, shutdownPipes(&Manager::maxFlowFordFulkerson,pipes)));
        }
    }
    string path = "../data/results/results_can_pipe_be_disabled_FF.csv";
    createCsvFilePipesDisable(path,can_be_disabled);
}


vector<pair<string,double>> Manager::shutdownPipesWithDecrease(vector<pair<string,int>> (Manager::*flowfunction)(),vector<Edge*>& pipes) {

    vector<pair<string, double>> percentageDecline;
    // Calculate total flow before removing the stations
    auto beforeFlows = (this->*flowfunction)();
    int beforeTotalFlow = 0;
    for (const auto& flow : beforeFlows)
        beforeTotalFlow += flow.second;

    // Reset graph and disable stations

    disablePipes(pipes);

    // Calculate total flow after removing the stations
    auto afterFlows = (this->*flowfunction)();
    int afterTotalFlow = 0;
    for (const auto& flow : afterFlows)
        afterTotalFlow += flow.second;

    // Check if the network was affected
    if (afterTotalFlow == beforeTotalFlow) {
        cout << "The network was not affected after removing pipes: " << endl;
        for (auto pipe:pipes) {
            cout << "Origin: " <<  Graph::getCode(pipe->getOrigin()) << " Destination: " << Graph::getCode(pipe->getDest()) << endl;
        }
        for (auto flow : afterFlows) {
            percentageDecline.push_back(make_pair(flow.first,0.0));
        }
        cout << endl;
        resetGraph();
        return percentageDecline;
    }

    // Calculate the percentage decline for each city

    for (const auto& beforeFlow : beforeFlows) {
        string cityCode = beforeFlow.first;
        int beforeFlowAmount = beforeFlow.second;
        double afterFlowAmount = 0;

        // Find the flow amount after removing the stations
        for (const auto& afterFlow : afterFlows) {
            if (afterFlow.first == cityCode) {
                afterFlowAmount = afterFlow.second;
                break;
            }
        }

        // Calculate the percentage decline in flow
        double declinePercentage = -(beforeFlowAmount - afterFlowAmount) / beforeFlowAmount * 100;
        percentageDecline.push_back(make_pair(cityCode, declinePercentage));
    }

    cout << "Percentage decline in flow for each city after removing stations:" << endl;
    for (const auto& entry : percentageDecline) {
        if(entry.second == 0.0) {
            cout << "City code: " << entry.first << ", Percentage Decline: " << entry.second << "%" << endl;
        }
        else {
            cout << "City code: " << entry.first << ", Percentage Decline: " << entry.second << "%" << endl;
        }
    }
    resetGraph();

    return percentageDecline;
}

void Manager::disableSelectedPipesEdmondsKarp(vector<Edge*> &pipes) {
    for (auto e : pipes){
        cout << Graph::getCode(e->getOrigin()) << Graph::getCode(e->getDest());
    }
    vector<pair<string,double>> decreased = shutdownPipesWithDecrease(&Manager::maxFlowEdmondsKarp,pipes);

    string path = "../data/results/results_decrease_rate_pipe_disabled_EK.csv";
    createCsvFileRates(path,decreased);

}

void Manager::disableSelectedPipesFordFulkerson(vector<Edge *> &pipes) {
    vector<pair<string,double>> decreased = shutdownPipesWithDecrease(&Manager::maxFlowFordFulkerson,pipes);
    string path = "../data/results/results_decrease_rate_pipe_disabled_FF.csv";
    createCsvFileRates(path,decreased);
}



/* ------------------- Extras ----------------------------- */

bool cmpFlows(const pair<string, int>& a, const pair<string, int>& b) {
    return a.second > b.second;
}

void Manager::printTopKFlows(vector<pair<string, int>>& flows, int k, const string& outputFile) {
    sort(flows.begin(), flows.end(), cmpFlows);

    vector<pair<string, int>> topK;
    for (int i = 0; i < k && i < flows.size(); ++i) {
        topK.push_back(flows[i]);
        cout << "Top " << i + 1 << " : code -> " << flows[i].first << " flow -> " << flows[i].second << endl;
    }

    createCsvFileFlows(outputFile, topK);
}

void Manager::topKFlowEdmondsKarpCities(const int k) {
    auto flows = maxFlowEdmondsKarp();
    printTopKFlows(flows, k, "../data/results/results_TopK_EK.csv");
    resetGraph();
}

void Manager::topKFlowFordFulkersonCities(const int k) {
    auto flows = maxFlowFordFulkerson();
    printTopKFlows(flows, k, "../data/results/results_TopK_FF.csv");
    resetGraph();
}

void Manager::calculateFlowRates(const vector<pair<string, int>>& flows, const string& outputFile) {
    double totalFlow = 0;
    for (const auto& flow : flows) {
        totalFlow += static_cast<double>(flow.second);
    }

    vector<pair<string, double>> rates;
    for (const auto& flow : flows) {
        double rate = static_cast<double>(flow.second) / totalFlow * 100;
        rates.push_back({flow.first, rate});
        cout << "City code: " << flow.first << " receives " << rate << "% of all the flow" << endl;
    }

    createCsvFileRates(outputFile, rates);
}

void Manager::flowRatePerCityEdmondsKarp() {
    auto flows = maxFlowEdmondsKarp();
    calculateFlowRates(flows, "../data/results/results_rateFlows_EK.csv");
    resetGraph();
}

void Manager::flowRatePerCityFordFulkerson() {
    auto flows = maxFlowFordFulkerson();
    calculateFlowRates(flows, "../data/results/results_rateFlows_FF.csv");
    resetGraph();
}

vector<int> Manager::getMetrics() {

    vector<int> metrics;

    graph->resetVisited();

    int mn = -1;
    int mx = 0;
    int mx_dif = 0;
    int sum = 0;
    int sum_dif = 0;
    int n = 0;

    for(auto a : graph->getVertexSet()){

        if(a->isVisited())continue;
        a->setVisited(true);

        for(auto b: a->getAdj()){

            int f = b->getFlow();
            int c = b->getCapacity();
            int dif = c - f;

            sum += f;
            sum_dif += dif;
            if(mn == -1 or mn > f)mn = f;
            if(mx == -1 or mx < f)mx = f;
            if(dif > mx_dif)mx_dif = dif;

            n += 1;
        }

    }

    graph->resetVisited();

    int mean = sum_dif/n;
    int variance = 0;

    for(auto a : graph->getVertexSet()){

        if(a->isVisited())continue;
        a->setVisited(true);

        for(auto b: a->getAdj()){

            int f = b->getFlow();
            int c = b->getCapacity();
            int dif = c - f;
            int d = dif - mean;

            variance += (d * d);
        }

    }

    metrics.emplace_back(mn);
    metrics.emplace_back(mx);
    metrics.emplace_back(sum/n);
    metrics.emplace_back(mx_dif);
    metrics.emplace_back(mean);
    metrics.emplace_back(variance/n);

    resetGraph();

    return metrics;
}






