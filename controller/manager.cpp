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


#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

/* ----- Auxiliary functions and constructors/destructors ---- */

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
int Manager::getHowManyEdges() const {
    int sum = 0;
    for (auto v : graph->getVertexSet()) {
        sum+= v->getAdj().size();
    }
    return sum;
}

void Manager::resetGraph() {
    Vertex* superSource = findVertexInMap("SR");
    Vertex* superSink = findVertexInMap("SS");

    vector<Edge*> deleteEdges; // Delete Residual Edges

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

    for(const auto e : deleteEdges) {
        graph->removeEdge(e);
    }
    if (superSource != nullptr) {
        graph->removeVertex(superSource);
        reservoirs.erase("SR");
    }
    if(superSink != nullptr) {
        graph->removeVertex(superSink);
        cities.erase("SS");
    }

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
/*------------- Csv methods -----------------------------*/

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

    outputCSV.flush();
    outputCSV.close();
}

void Manager::createCsvFileDisable(const string &path, vector<pair<string, bool> > &disable) {
    ofstream outputCSV(path);

    if (!outputCSV.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        exit(EXIT_FAILURE); // Exit the program with a custom error message
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
        exit(EXIT_FAILURE); // Exit the program with a custom error message
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
        exit(EXIT_FAILURE); // Exit the program with a custom error message
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
            if(const auto dest = e->getDest();!dest->isVisited() && dest->isEnabled() && e->isEnabled()  && e->getFlow() > 0) {
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
    timespec start_real, end_real;
    timespec start_cpu, end_cpu;

    clock_gettime(CLOCK_REALTIME, &start_real);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_cpu);
    vector<pair<string, int>> results;
    for (const auto& flow : flows) {
        if (find(chosenCities.begin(),chosenCities.end(),flow.first) != chosenCities.end()) {
            cout << "Flow for city with code " << flow.first << ": " << flow.second << endl;
            results.push_back(make_pair(flow.first,flow.second));
        }
    }

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_cpu);
    clock_gettime(CLOCK_REALTIME, &end_real);

    double elapsed_real = (end_real.tv_sec - start_real.tv_sec) +
                          (end_real.tv_nsec - start_real.tv_nsec) / 1e9;

    double elapsed_cpu = (end_cpu.tv_sec - start_cpu.tv_sec) +
                         (end_cpu.tv_nsec - start_cpu.tv_nsec) / 1e9;

    cout << "Elapsed real time: " << elapsed_real << " seconds" << endl;
    cout << "Elapsed CPU time: " << elapsed_cpu << " seconds" << endl;

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


void Manager::getEdmondsKarpAllCities() {
    auto flows = maxFlowEdmondsKarp();
    vector<string> cities;
    for (auto flow: flows) {
        cities.push_back(flow.first);
    }
    printFlowMetrics(flows,cities, "../data/results/results_21_EK.csv");
    resetGraph();
}

void Manager::getFordFulkersonAllCities() {
    auto flows = maxFlowFordFulkerson();
    vector<string> cities;
    for (auto flow: flows) {
        cities.push_back(flow.first);
    }
    printFlowMetrics(flows,cities,"../data/results/results_21_FF.csv");
    resetGraph();
}


/*---------------------Exercise 2.2 -----------------------*/

vector<pair<string,bool>> Manager::canCityGetEnoughWater(vector<string>& codes,vector<pair<string,int>>& flows) {
    vector<pair<string,bool>> results;
    for (const auto& flow : flows) {
        if (find(codes.begin(),codes.end(),flow.first) != codes.end()) {
            int demand = dynamic_cast<City*>(findVertexInMap(flow.first))->getDemand();
            if (demand > flow.second) {
                cout << ANSI_COLOR_RED << "The city with code " << flow.first << " can't get enough flow. flow : " << flow.second << " demand: " << demand << ANSI_COLOR_RESET << endl;
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

/* -------------------Exercise 3.1----------------------------- */

void Manager::disableReservoirs(vector<string> &reservoirs) {
    for(string code : reservoirs) {
        findVertexInMap(code)->setEnabled(false);
    }
}

void Manager::dfs_disable(Vertex* reservoir) {
    reservoir->setVisited(true);
    for (auto e : reservoir->getAdj()) {
        if (Vertex* dest = e->getDest(); !dest->isVisited() && e->getFlow() > 0) {
            e->setFlow(0);
        }
    }
}


vector<pair<string,int>> Manager::graphChangeFlowsAfterReservoirsDisabled(vector<string> &reservoirs) {

    for (auto v : graph->getVertexSet()) {
        v->setVisited(false);
    }

    // Change graph doing a dfs visit
    for (auto r : reservoirs) {
        Vertex* v = findVertexInMap(r);
        dfs_disable(v);
    }

    // Calculate Results of flow.
}

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
    if (beforeTotalFlow <= totalDemand) {
        afterFlows = graphChangeFlowsAfterReservoirsDisabled(reservoirs);
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
        cout << "The network was not affected after removing: ";
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

    cout << "Percentage decline in flow for each city after removing stations:" << endl;
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


/* -------------------Exercise 3.2----------------------------- */

void Manager::disableStations(vector<string>& stations) {
    for(string code : stations) {
        findVertexInMap(code)->setEnabled(false);
    }
}

bool Manager::shutdownStations(vector<pair<string,int>> (Manager::*flowfunction)(),vector<string>& codes) {
    // Calculate total flow before removing the stations
    auto beforeFlows = (this->*flowfunction)();
    int beforeTotalFlow = 0;
    for (const auto& flow : beforeFlows)
        beforeTotalFlow += flow.second;

    // Reset graph and disable stations
    resetGraph();
    disableStations(codes);

    // Calculate total flow after removing the stations
    auto afterFlows = (this->*flowfunction)();
    int afterTotalFlow = 0;
    for (const auto& flow : afterFlows)
        afterTotalFlow += flow.second;

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

    cout << "Percentage decline in flow for each city after removing stations:" << endl;
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

    // Reset graph and disable stations
    resetGraph();
    disableStations(codes);

    // Calculate total flow after removing the stations
    auto afterFlows = (this->*flowfunction)();
    int afterTotalFlow = 0;
    for (const auto& flow : afterFlows)
        afterTotalFlow += flow.second;

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






