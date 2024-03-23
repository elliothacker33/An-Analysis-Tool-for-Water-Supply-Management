#include "manager.h"

#include <climits>
#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <vector>
#include <limits>
Manager::Manager() {
    graph = new Graph();
}
Manager::~Manager() {
    delete graph;
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
        stringstream ss(line);
        while(getline(ss,word,',')){
            stringstream wordStream(word);
            string trimmedWord;
            wordStream >> ws >> trimmedWord;

            if (!trimmedWord.empty()) {
                row.push_back(trimmedWord);
            }
        }
        if (row.empty()) {
            continue;
        }
        string populationStr = row[4] + row[5];
        cout << row[0] << " " << row[1] << " " <<  row[2] << " " << row[3] << " " << populationStr << endl;
        int population = parseInt(populationStr);
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
        stringstream ss(line);
        while(getline(ss,word,',')){
            stringstream wordStream(word);
            string trimmedWord;
            wordStream >> ws >> trimmedWord;

            if (!trimmedWord.empty()) {
                row.push_back(trimmedWord);
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
        stringstream ss(line);
        while(getline(ss,word,',')){
            stringstream wordStream(word);
            string trimmedWord;
            wordStream >> ws >> trimmedWord;

            if (!trimmedWord.empty()) {
                row.push_back(trimmedWord);
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
        stringstream ss(line);
        while(getline(ss,word,',')){
            stringstream wordStream(word);
            string trimmedWord;
            wordStream >> std::ws >> trimmedWord;

            if (!trimmedWord.empty()) {
                row.push_back(trimmedWord);
            }
        }
        Vertex* orig = findVertexInMap(row[0]);
        Vertex* dest = findVertexInMap(row[1]);

        if (stoi(row[3]) == 0) {
            graph->addEdge(orig,dest,stoi(row[2]));
        }
        else if (stoi(row[3]) == 1) {
            graph->addEdge(orig,dest,stoi(row[2]));
            graph->addEdge(dest,orig,stoi(row[3]));
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
        if (v->getType() == 'R') {
            const auto reservoir = dynamic_cast<Reservoir*>(v);
            graph->addEdge(superSource,v,reservoir->getMaxDelivery());
        }
    }
    return superSource;
}

Vertex* Manager::addSuperSink() {
    Vertex* superSink = new City("Super Sink",-1,"SS",INT_MAX,-1);
    graph->addVertex(superSink);
    cities.insert({"SS",superSink});
    for (const auto v : graph->getVertexSet()) {
        if (v->getType() == 'C') {
            const auto city = dynamic_cast<City*>(v);
            graph->addEdge(v,superSink,city->getDemand());
        }
    }
    return superSink;
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
                if (dynamic_cast<City*>(dest)->getCode() == "SS") {
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

int findMinEdge(vector<Edge*> path) {
    int flow = numeric_limits<int>::max();
    for (auto e: path) {
        if(e->getFlow() < flow) {
            flow = e->getFlow();
        }
    }
    return flow;
}
void Manager::maxFlowEdmondsKarp() {
    // Add super source and  super sink
    Vertex* superSource = addSuperSource();
    Vertex* superSink = addSuperSink();

    // Initialize graph
    for (const auto v: graph->getVertexSet()) {
        for (const auto e : v->getAdj()) {
            e->setFlow(e->getCapacity());
        }
    }

    // Find Paths
    vector<Edge*> path;
    while(!(path = bfs_flow(superSource, superSink)).empty()) {
            const int newFlow = findMinEdge(path);
            for (const auto e : path) {
                if(e->getResidualEdge() == nullptr) {
                    const auto edg = graph->addEdge(e->getDest(),e->getOrigin(),e->getCapacity());
                    edg->setFlow(0);
                    e->setResidualEdge(edg);
                    edg->setResidualEdge(e);
                }
                e->setFlow(e->getFlow()-newFlow);
                e->getResidualEdge()->setFlow(e->getResidualEdge()->getFlow() + newFlow);
            }
            path.clear();
    }

    for (const auto n : graph->getVertexSet()) {
        for (const auto e : n->getAdj()) {
            e->setFlow(e->getCapacity() - e->getFlow());
            if (e->getResidualEdge() != nullptr) {
                //  TODO: Remove Residual Edges
            }
        }
    }


}

