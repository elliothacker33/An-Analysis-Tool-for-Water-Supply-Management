#include "manager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

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


