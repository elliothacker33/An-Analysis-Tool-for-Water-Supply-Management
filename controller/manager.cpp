#include "manager.h"
#include <algorithm>
#include <climits>
#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <vector>
#include <limits>
#include <ctime>

Manager::Manager() {
    graph = new Graph();
}
Manager::~Manager() {
    delete graph;
}
void Manager::resetManager() {
    delete this->graph;
    this->graph = new Graph();
    cities.clear();
    reservoirs.clear();
    stations.clear();
}
Graph* Manager::getGraph() const {
    return graph;
}

unordered_map<string,Vertex*> Manager::getCities() const{
    return cities;
}
unordered_map<string, Vertex *> Manager::getReservoirs() const {
    return reservoirs;
}
unordered_map<string, Vertex*> Manager::getStations() const {
    return stations;
}

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


void Manager::importFiles(const string& pathCities,const string& pathReservoirs,const string& pathStations,const string& pathPipes) {
    resetManager();
    importCities(pathCities);
    importStations(pathStations);
    importReservoirs(pathReservoirs);
    importPipes(pathPipes);
}

int Manager::parseInt(const string& text) {
    string number_string;
    for (const auto i : text) {
        if (i != '\"' && i != ',')
            number_string.push_back(i);
    }
    const int number_integer=stoi(number_string);
    return number_integer;
}

void Manager::createCsvFileFlows(const string &path,vector<pair<string,int>>& flows) {
    ofstream outputCSV(path);

    if (!outputCSV.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        exit(EXIT_FAILURE); // Exit the program with a custom error message
    }

    outputCSV << "Name,Code,Flow" << endl;

    for (const auto flow : flows) {
        outputCSV << Graph::getName(findVertexInMap(flow.first)) << "," << flow.first << "," << flow.second << endl;
    }

    outputCSV.close();
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
        exit(EXIT_FAILURE); // Exit the program with a custom error message
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
        if (Vertex* dest = e->getDest(); !dest->isVisited() && e->getFlow() > 0) {
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
            if(const auto dest = e->getDest();!dest->isVisited() && e->getFlow() > 0) {
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

int Manager::findMinEdge(const vector<Edge*>& path) {
    int flow = numeric_limits<int>::max();
    for (auto e: path) {
        if(e->getFlow() < flow) {
            flow = e->getFlow();
        }
    }
    return flow;
}
vector<pair<string,int>> Manager::maxFlowFordFulkerson() {
    Vertex* superSource = addSuperSource();
    Vertex* superSink = addSuperSink();

    for (const auto v: graph->getVertexSet()) {
        for (const auto e : v->getAdj()) {
            e->setFlow(e->getCapacity());
        }
    }

    vector<Edge*> path;
    while(!(path = dfs_flow(superSource, superSink)).empty()) {
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
    Vertex* superSource = addSuperSource();
    Vertex* superSink = addSuperSink();


    for (const auto v: graph->getVertexSet()) {
        for (const auto e : v->getAdj()) {
            e->setFlow(e->getCapacity());
        }
    }

    vector<Edge*> path;
    while(!(path = bfs_flow(superSource, superSink)).empty()) {
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

void Manager::resetGraph() {
    Vertex* superSource= findVertexInMap("SR");
    Vertex* superSink = findVertexInMap("SS");

    vector<Edge*> deleteEdges; // Delete Residual Edges

    for (const auto v : graph->getVertexSet()) {
        v->setPath(nullptr);
        v->setVisited(false);
        for (auto e : v->getAdj()) {
            e->setFlow(0);
            e->setReverseEdge(nullptr);
            if (e->getType() == "residual")
                deleteEdges.push_back(e);
        }
    }

    for(const auto e : deleteEdges) {
        graph->removeEdge(e);
    }

    graph->removeVertex(superSource);
    graph->removeVertex(superSink);

    reservoirs.erase("SR");
    reservoirs.erase("SS");

}

void Manager::getEdmondsKarpOneCity(string& code) {
    timespec start_real, end_real;
    timespec start_cpu, end_cpu;

    clock_gettime(CLOCK_REALTIME, &start_real);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_cpu);

    auto flows = maxFlowEdmondsKarp();

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_cpu);
    clock_gettime(CLOCK_REALTIME, &end_real);

    double elapsed_real = (end_real.tv_sec - start_real.tv_sec) +
                          (end_real.tv_nsec - start_real.tv_nsec) / 1e9;


    double elapsed_cpu = (end_cpu.tv_sec - start_cpu.tv_sec) +
                         (end_cpu.tv_nsec - start_cpu.tv_nsec) / 1e9;

    auto it = find_if(flows.begin(), flows.end(), [&](const auto& flow) {
        return flow.first == code;
    });

    if (it != flows.end()) {
        cout << "Flow for city with code " << code << ": " << it->second << endl;
    }
    else {
        cerr << "Error calculating max flow" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Elapsed real time: " << elapsed_real << " seconds" << endl;
    cout << "Elapsed CPU time: " << elapsed_cpu << " seconds" << endl;

    resetGraph();
}

#include <ctime>

void Manager::getEdmondsKarpAllCities() {
    timespec start_real, end_real;
    timespec start_cpu, end_cpu;

    clock_gettime(CLOCK_REALTIME, &start_real);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_cpu);

    auto flows = maxFlowEdmondsKarp();

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_cpu);
    clock_gettime(CLOCK_REALTIME, &end_real);


    double elapsed_real = (end_real.tv_sec - start_real.tv_sec) +
                          (end_real.tv_nsec - start_real.tv_nsec) / 1e9;


    double elapsed_cpu = (end_cpu.tv_sec - start_cpu.tv_sec) +
                         (end_cpu.tv_nsec - start_cpu.tv_nsec) / 1e9;

    for (const auto& flow : flows) {
        cout << "Flow for city with code " << flow.first << ": " << flow.second << endl;
    }

    cout << "Elapsed real time: " << elapsed_real << " seconds" << endl;
    cout << "Elapsed CPU time: " << elapsed_cpu << " seconds" << endl;

    string filename = "../data/results/results_21_EK.csv";
    createCsvFileFlows(filename, flows);

    resetGraph();
}


void Manager::getFordFulkersonOneCity(string& code) {
    timespec start_real, end_real;
    timespec start_cpu, end_cpu;

    clock_gettime(CLOCK_REALTIME, &start_real);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_cpu);

    auto flows = maxFlowFordFulkerson();

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_cpu);
    clock_gettime(CLOCK_REALTIME, &end_real);

    double elapsed_real = (end_real.tv_sec - start_real.tv_sec) +
                          (end_real.tv_nsec - start_real.tv_nsec) / 1e9;


    double elapsed_cpu = (end_cpu.tv_sec - start_cpu.tv_sec) +
                         (end_cpu.tv_nsec - start_cpu.tv_nsec) / 1e9;

    auto it = find_if(flows.begin(), flows.end(), [&](const auto& flow) {
        return flow.first == code;
    });

    if (it != flows.end()) {
        cout << "Flow for city with code " << code << ": " << it->second << endl;
    }
    else {
        cerr << "Error calculating max flow" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Elapsed real time: " << elapsed_real << " seconds" << endl;
    cout << "Elapsed CPU time: " << elapsed_cpu << " seconds" << endl;

    resetGraph();
}


void Manager::getFordFulkersonAllCities() {
    timespec start_real, end_real;
    timespec start_cpu, end_cpu;

    clock_gettime(CLOCK_REALTIME, &start_real);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_cpu);

    auto flows = maxFlowFordFulkerson();

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_cpu);
    clock_gettime(CLOCK_REALTIME, &end_real);


    double elapsed_real = (end_real.tv_sec - start_real.tv_sec) +
                          (end_real.tv_nsec - start_real.tv_nsec) / 1e9;


    double elapsed_cpu = (end_cpu.tv_sec - start_cpu.tv_sec) +
                         (end_cpu.tv_nsec - start_cpu.tv_nsec) / 1e9;

    for (const auto& flow : flows) {
        cout << "Flow for city with code " << flow.first << ": " << flow.second << endl;
    }

    cout << "Elapsed real time: " << elapsed_real << " seconds" << endl;
    cout << "Elapsed CPU time: " << elapsed_cpu << " seconds" << endl;

    string filename = "../data/results/results_21_FF.csv";
    createCsvFileFlows(filename, flows);

    resetGraph();
}


