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
        cout << row[0] << endl;
        string populationStr = row[4] + row[5];
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

//TODO: Correct error in names of Vertexes
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
            graph->addEdge(superSource,v,reservoir->getMaxDelivery(),"tmp");
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
            graph->addEdge(v,superSink,city->getDemand(),"tmp");
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
void Manager::maxFlowFordFulkerson() {
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
    int totalFlow = 0;
    for (const auto n : graph->getVertexSet()) {
        if (n->getType() == 'C' && Graph::getCode(n) != "SS") {
            cout << Graph::getCode(n) << endl;
            int sumFlow = 0;
            for (const auto e : n->getIncoming()) {
                if (e->getType() == "residual")
                    sumFlow+= e->getFlow();
            }
            if (sumFlow >= dynamic_cast<City*>(n)->getDemand())
                cout << "True" << endl;
            else {
                cout << "False" << endl;
            }
            cout << sumFlow << endl;
            totalFlow += sumFlow;
        }
    }
    cout << "Total: " << totalFlow << endl;
}

void Manager::maxFlowEdmondsKarp() {
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
    int totalFlow = 0;
    for (const auto n : graph->getVertexSet()) {
        if (n->getType() == 'C' && Graph::getCode(n) != "SS") {
            cout << Graph::getCode(n) << endl;
            int sumFlow = 0;
            for (const auto e : n->getIncoming()) {
                if (e->getType() == "residual")
                    sumFlow+= e->getFlow();
            }
                if (sumFlow >= dynamic_cast<City*>(n)->getDemand())
                    cout << "True" << endl;
                else {
                    cout << "False" << endl;
                }
                cout << sumFlow << endl;
                totalFlow += sumFlow;
            }
        }
    cout << "Total: " << totalFlow << endl;
}

void Manager::resetGraph() {
    Vertex* superSource= findVertexInMap("SR");
    Vertex* superSink = findVertexInMap("SS");
    graph->removeVertex(superSource);
    graph->removeVertex(superSink);

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

    for (const auto v : graph->getVertexSet()) {
        v->setPath(nullptr);
        v->setVisited(false);
        for (auto e : v->getIncoming()) {
            e->setFlow(0);
            e->setReverseEdge(nullptr);
            if (e->getType() == "residual")
                deleteEdges.push_back(e);
        }
    }

    for(const auto e : deleteEdges) {
        graph->removeEdge(e);
    }

}
