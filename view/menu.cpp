#include "menu.h"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <bits/random.h>


Menu::~Menu() {
    delete manager;
}

Menu::Menu(Manager* manager) {
    this->manager = manager;
}

/* Auxiliary functions */
string Menu::removeLeadingTrailingSpaces(const string& input) {
    const auto start = input.find_first_not_of(" \n\r\t");
    const auto end = input.find_last_not_of(" \n\r\t");

    return (start != string::npos && end != string::npos) ? input.substr(start, end - start + 1) : "";
}

/* Valid input check for codes */
Edge* Menu::validPipe(string& code1, string& code2) {
    Vertex* v = manager->findVertexInMap(code1);
    for(auto e : v->getAdj()) {
        if (Graph::getCode(e->getDest()) == code2) {
            return e;
        }
    }
    return nullptr;
}
bool Menu::validCity(string &code) {
    auto cities = manager->getCities();
    const string trimmedInput = removeLeadingTrailingSpaces(code);
    return cities.find(trimmedInput) != cities.end();
}

bool Menu::validStation(string& code) {
    auto stations = manager->getStations();
    const string trimmedInput = removeLeadingTrailingSpaces(code);
    return stations.find(trimmedInput) != stations.end();
}

bool Menu::validReservoir(string& code) {
    auto reservoirs = manager->getReservoirs();
    const string trimmedInput = removeLeadingTrailingSpaces(code);
    return reservoirs.find(trimmedInput) != reservoirs.end();
}

bool Menu::isValidInterface(const string& type,string& code) {
    if (type == "city") {
        return validCity(code);
    }

    if (type == "station") {
        return validStation(code);
    }

    if (type == "reservoir") {
        return validReservoir(code);
    }
    return false;
}

/* Get Input functions */

vector<string> Menu::getVertex(const unordered_map<string,Vertex*>& existingItems, const string& itemType) {
    vector<string> items;
    string input;
    while (items.size() != existingItems.size()) {
        getExamplesInterface(itemType);
        cout << "Enter a valid " << itemType << " code or press 'STOP' to stop inserting: ";
        getline(cin, input);
        cout << endl;
        transform(input.begin(), input.end(), input.begin(), ::toupper); // Convert input to uppercase
        if (isValidInterface(itemType, input)) {
            bool alreadyExists = false;
            for (const string& item : items) {
                if (item == input) {
                    alreadyExists = true;
                    break;
                }
            }
            if (!alreadyExists) {
                items.push_back(input);
            }
        }
        else if (input == "STOP") {
            break;
        }
        else {
            cout << "Invalid input. Please enter a valid " << itemType << endl;
        }
    }
    return items;
}

vector<string> Menu::getStations() {
    return getVertex(manager->getStations(), "station");
}

vector<string> Menu::getCities() {
    return getVertex(manager->getCities(), "city");
}

vector<string> Menu::getReservoirs() {
    return getVertex(manager->getReservoirs(),"reservoir");
}

vector<Edge *> Menu::getPipes() {
    vector <Edge*> items;
    vector<string> pipes;
    string input;
    string input2;
    int size_of_edges = manager->getHowManyEdges();
    while (items.size() != size_of_edges ) {

        cout << "Enter a valid pipe code or press 'STOP' to stop inserting" << endl;
        getExamplesInterface("city");
        getExamplesInterface("station");
        getExamplesInterface("reservoir");
        cout << "Enter a valid origin vertex code: " << endl;
        getline(cin, input);
        transform(input.begin(), input.end(), input.begin(), ::toupper); // Convert input to uppercase

        if (validCity(input) || validStation(input) || validReservoir(input)) {
            getExamplesInterface("city");
            getExamplesInterface("station");
            getExamplesInterface("reservoir");
            cout << "Enter a valid destination vertex code: " << endl;
            getline(cin, input2);
            transform(input2.begin(), input2.end(), input2.begin(), ::toupper); // Convert input to uppercase
            if (validCity(input2) || validStation(input2) || validReservoir(input2)) {
                if (Edge* edge =validPipe(input,input2); edge != nullptr) {
                    bool alreadyExists = false;
                    for (auto item : items) {
                        if (edge == item) {
                            alreadyExists = true;
                            break;
                        }
                    }
                    if (!alreadyExists) {
                        items.push_back(edge);
                    }
                }
                else {
                    cout << "Invalid input. Please enter a valid pipe" << endl;
                }
            }
            else if (input == "STOP") {
                break;
            }
            else {
                cout << "Invalid input. Please enter a valid vertex" << endl;
            }
        }
        else if (input == "STOP") {
            break;
        }
        else {
            cout << "Invalid input. Please enter a valid vertex" << endl;
        }
    }
    return items;
}


bool Menu::getNumberInput(int minInput, int maxInput, int *option) {
    cout << "Enter your choice ("<< minInput <<"-" << maxInput <<"): ";
    string sinput;
    getline(cin, sinput);
    stringstream ss(sinput);
    int input;
    ss >> input;

    string trimmedInput = removeLeadingTrailingSpaces(sinput);

    if (trimmedInput.size() == 1 && input >= minInput && input <= maxInput) {
        *option = input;
        return true;
    }

    cout << "Invalid input. Please enter a number between " << minInput << " and " << maxInput << endl;
    return false;
}

/* Get input examples */

void Menu::getItemExamples(const unordered_map<string, Vertex*>& items, const string& label) {
    if (items.size() < 3) {
        cerr << "There are less than 3 " << label << " in the map." << endl;
        return;
    }

    cout << "Example " << label << " codes:" << endl;
    int count = 0;
    for (const auto& item : items) {
        cout << item.first << endl;
        count++;
        if (count >= 3) {
            break;
        }
    }
}

void Menu::getCityExamples() {
    getItemExamples(manager->getCities(), "city");
}

void Menu::getStationExamples() {
    getItemExamples(manager->getStations(), "station");
}

void Menu::getReservoirExamples() {
    getItemExamples(manager->getReservoirs(), "reservoir");
}


void Menu::getExamplesInterface(const string& type) {
    if (type == "city") {
        getCityExamples();
    }
    if (type == "station") {
        getStationExamples();
    }

    if (type == "reservoir") {
        getReservoirExamples();
    }
}





/* Menus */
void Menu::mainMenu() {
    // Display mainMenu
    int option = 0;

    do {
        cout << "------------------------------------------------" << endl;
        cout << "                Menu -> Main menu               " << endl;
        cout << "                                                " << endl;
        cout << "             1. Choose small data set           " << endl;
        cout << "             2. Choose large data set           " << endl;
        cout << "             3. Exit menu                       " << endl;
        cout << "                                                " << endl;
        cout << "------------------------------------------------" << endl;
    }
    while(!getNumberInput(1,3,&option));

    switch(option) {
        case 1:
            manager->importFiles("../data/Project1DataSetSmall/Cities_Madeira.csv","../data/Project1DataSetSmall/Reservoirs_Madeira.csv","../data/Project1DataSetSmall/Stations_Madeira.csv","../data/Project1DataSetSmall/Pipes_Madeira.csv");
            menuStack.push(&Menu::mainMenu);
            algorithmMenu();
            break;
        case 2:
            manager->importFiles("../data/Project1LargeDataSet/Cities.csv","../data/Project1LargeDataSet/Reservoir.csv","../data/Project1LargeDataSet/Stations.csv","../data/Project1LargeDataSet/Pipes.csv");
            menuStack.push(&Menu::mainMenu);
            algorithmMenu();
            break;
        case 3:
            exitMenu();
            break;
    }
}
void Menu::algorithmMenu() {
    int option = 0;
    do {
        cout << "----------------------------------------------" << endl;
        cout << "              Menu -> Algorithm               " << endl;
        cout << "                                              " << endl;
        cout << "                 0. Go back                   " << endl;
        cout << "                                              " << endl;
        cout << "             Basic Service Metrics            " << endl;
        cout << "                                              " << endl;
        cout << "               1. Exercise 2.1                " << endl;
        cout << "               2. Exercice 2.2                " << endl;
        cout << "               3. Exercise 2.3                " << endl;
        cout << "                                              " << endl;
        cout << "            Reliability for Failures          " << endl;
        cout << "                                              " << endl;
        cout << "               4. Exercise 3.1                " << endl;
        cout << "               5. Exercice 3.2                " << endl;
        cout << "               6. Exercise 3.3                " << endl;
        cout << "                                              " << endl;
        cout << "          Extra Algortithms and Metrics       " << endl;
        cout << "                                              " << endl;
        cout << "                   7. Extra                   " << endl;
        cout << "----------------------------------------------" << endl;
    }
    while(!getNumberInput(0,7,&option));

    switch(option) {
        case 0:
            goBack();
            break;
        case 1:
            menuStack.push(&Menu::algorithmMenu);
            exercise21();
            break;
        case 2:
            menuStack.push(&Menu::algorithmMenu);
            exercise22();
            break;
        case 5:
            menuStack.push(&Menu::algorithmMenu);
            exercise32();
            break;
        case 6:
            menuStack.push(&Menu::algorithmMenu);
            exercise33();
            break;
        case 7:
            menuStack.push(&Menu::algorithmMenu);
            extraMenu();
            break;
    }
}

void Menu::exercise21() {
    int option = 0;
    do {
        cout << "------------------------------------------------" << endl;
        cout << "              Menu -> Exercice 2.1              " << endl;
        cout << "                                                " << endl;
        cout << "             0. Go back                         " << endl;
        cout << "             1. Edmonds Karp -> One city        " << endl;
        cout << "             2. Edmonds Karp -> All cities      " << endl;
        cout << "             3. Ford Fulkerson -> One city      " << endl;
        cout << "             4. Ford Fulkerson -> All cities    " << endl;
        cout << "                                                " << endl;
        cout << "------------------------------------------------" << endl;
    }
    while(!getNumberInput(0,4,&option));

    vector<string> cities;
    switch(option) {
        case 0:
            goBack();
            break;

        case 1:
            cities = getCities();
            if (cities.empty()) {
                cout << "No cities were selected" << endl;
            }
            else {
                manager->getEdmondsKarpXCity(cities);
            }
            exercise21();
            break;
        case 2:
            manager->getEdmondsKarpAllCities();
            exercise21();
            break;

        case 3:
            cities = getCities();
            if (cities.empty()) {
                cout << "No cities were selected" << endl;
            }
            else {
                manager->getFordFulkersonXCity(cities);
            }
            exercise21();
            break;

        case 4:
            manager->getFordFulkersonAllCities();
            exercise21();
            break;
    }
}

void Menu::exercise22() {
    vector<string> cities;
    int option = 0;
    do {
        cout << "------------------------------------------------" << endl;
        cout << "              Menu -> Exercice 2.2              " << endl;
        cout << "                                                " << endl;
        cout << "           0. Go back                           " << endl;
        cout << "           1. Can city x get enough water? (EK) " << endl;
        cout << "           2. Can city x get enough water? (FF) " << endl;
        cout << "           3. Can all cities get enough water? (EK) " << endl;
        cout << "           4. Can all cities get enough water? (FF) " << endl;
        cout << "                                                " << endl;
        cout << "------------------------------------------------" << endl;
    }
    while(!getNumberInput(0,4,&option));


    switch(option) {
        case 0:
            goBack();
            break;

        case 1:
            cities = getCities();
            if (cities.empty()) {
                cout << "No cities were selected" << endl;
            }
            else {
                manager->canCityXGetEnoughWaterEK(cities);
            }
            exercise22();
            break;
        case 2:
            cities = getCities();
            if (cities.empty()) {
                cout << "No cities were selected" << endl;
            }
            else {
                manager->canCityXGetEnoughWaterFF(cities);
            }
            exercise22();
            break;

        case 3:
            manager->canAllCitiesGetEnoughWaterEK();
            exercise22();
            break;

        case 4:
            manager->canAllCitiesGetEnoughWaterFF();
            exercise22();
            break;
    }
}
void Menu::exercise23(){
    int option = 0;
    do {
        cout << "------------------------------------------------" << endl;
        cout << "              Menu -> Exercice 2.3              " << endl;
        cout << "                                                " << endl;
        cout << "          0. Go back                            " << endl;
        cout << "          1. Better distribution of water (EK)  " << endl;
        cout << "          2. Better distribution of water (FF)  " << endl;
        cout << "------------------------------------------------" << endl;
    }
    while(0,2,&option);

    switch()
}
void Menu::exercise31() {
    int option = 0;
    do {
        cout << "------------------------------------------------" << endl;
        cout << "              Menu -> Exercice 3.1              " << endl;
        cout << "                                                " << endl;
        cout << "             0. Go back                         " << endl;
        cout << "             1. Disable each resevoir (EK)       " << endl;
        cout << "             2. Disable each resevoir (FF)       " << endl;
        cout << "             3. Disable selected reservoirs (EK)  " << endl;
        cout << "             4. Disable selected reservoirs (FF)  " << endl;
        cout << "                                                " << endl;
        cout << "------------------------------------------------" << endl;
    }
    while(!getNumberInput(0,4,&option));
    string code;
    vector<string> reservoirs;

    switch(option) {
        case 0:
            goBack();
            break;
        case 1:
            manager->disableEachReservoirEdmondsKarp();
            exercise31();
        break;
        case 2:
            manager->disableEachStationFordFulkerson();
            exercise31();
        break;
        case 3:
            reservoirs = getReservoirs();
            if (reservoirs.empty()) {
                cout << "No reservoirs were selected" << endl;
            }
            else {
                manager->disableSelectedStationsEdmondsKarp(reservoirs);
            }
            exercise31();
            break;
        case 4:
            reservoirs = getReservoirs();
            if (reservoirs.empty()) {
                cout << "No reservoirs were selected" << endl;
            }
            else {
                manager->disableSelectedStationsFordFulkerson(reservoirs);
            }
            exercise31();
            break;

    }
}
void Menu::exercise32() {
    int option = 0;
    do {
        cout << "------------------------------------------------" << endl;
        cout << "              Menu -> Exercice 3.2              " << endl;
        cout << "                                                " << endl;
        cout << "             0. Go back                         " << endl;
        cout << "             1. Disable each station (EK)       " << endl;
        cout << "             2. Disable each station (FF)       " << endl;
        cout << "             3. Disable selected stations (EK)  " << endl;
        cout << "             4. Disable selected stations (FF)  " << endl;
        cout << "                                                " << endl;
        cout << "------------------------------------------------" << endl;
    }
    while(!getNumberInput(0,4,&option));
    string code;
    vector<string> stations;

    switch(option) {
        case 0:
            goBack();
            break;
        case 1:
            manager->disableEachStationEdmondsKarp();
            exercise32();
            break;
        case 2:
            manager->disableEachStationFordFulkerson();
            exercise32();
            break;
        case 3:
            stations = getStations();
            if (stations.empty()) {
                cout << "No stations were selected" << endl;
            }
            else {
                manager->disableSelectedStationsEdmondsKarp(stations);
            }
            exercise32();
            break;
        case 4:
            stations = getStations();
            if (stations.empty()) {
                cout << "No stations were selected" << endl;
            }
            else {
                manager->disableSelectedStationsFordFulkerson(stations);
            }
            exercise32();
            break;

    }
}

void Menu::exercise33() {
    int option = 0;
    do {
        cout << "------------------------------------------------" << endl;
        cout << "              Menu -> Exercice 3.2              " << endl;
        cout << "                                                " << endl;
        cout << "             0. Go back                         " << endl;
        cout << "             1. Disable each pipe (EK)          " << endl;
        cout << "             2. Disable each pipe (FF)          " << endl;
        cout << "             3. Disable selected pipes (EK)     " << endl;
        cout << "             4. Disable selected pipes (FF)     " << endl;
        cout << "                                                " << endl;
        cout << "------------------------------------------------" << endl;
    }
    while(!getNumberInput(0,4,&option));
    string code;
    vector<Edge*> pipes;
    switch(option) {
        case 0:
            goBack();
        break;

        case 1:
            manager->disableEachPipeEdmondsKarp();
            exercise33();
            break;

        case 2:
            manager->disableEachPipeFordFulkerson();
            exercise33();
            break;
        case 3:
            pipes = getPipes();
            if (pipes.empty()) {
                cout << "No pipes were selected" << endl;
            }
            else {
                manager->disableSelectedPipesEdmondsKarp(pipes);
            }
            exercise33();
            break;
        case 4:
            pipes = getPipes();
            if (pipes.empty()) {
                cout << "No pipes were selected" << endl;
            }
            else {
                manager->disableSelectedPipesFordFulkerson(pipes);
            }
            exercise33();
            break;
    }
}

void Menu::extraMenu() {
    int option = 0;
    do {
        cout << "------------------------------------------------" << endl;
        cout << "                Menu -> Extra                   " << endl;
        cout << "                                                " << endl;
        cout << "             0. Go back                         " << endl;
        cout << "             1. TopK cities with max-flow (EK)  " << endl;
        cout << "             2. TopK cities with max-flow (FF)  " << endl;
        cout << "             3. Average Flow rate per city (EK) " << endl;
        cout << "             4. Average Flow rate per city (FF) " << endl;
        cout << "                                                " << endl;
        cout << "------------------------------------------------" << endl;
    }
    while(!getNumberInput(0,4,&option));
    int k;
    bool validResult;
    int maxK;
    switch(option) {
        case 0:
            goBack();
            break;

        case 1:
            maxK =manager->getCities().size();
            do {
                validResult = getNumberInput(1,maxK,&k);
            }
            while(!validResult);
            manager->topKFlowEdmondsKarpCities(k);
            extraMenu();
            break;
        case 2:
            maxK =manager->getCities().size();
            do {
                validResult = getNumberInput(1,maxK,&k);
            }
            while(!validResult);
            manager->topKFlowFordFulkersonCities(k);
            extraMenu();
            break;

        case 3:
            manager->flowRatePerCityEdmondsKarp();
            extraMenu();
            break;
        case 4:
            manager->flowRatePerCityFordFulkerson();
            extraMenu();
            break;


    }
}


void Menu::exitMenu() {
    while (!menuStack.empty()) {
        menuStack.pop();
    }
    delete this;
}

void Menu::goBack() {
    if (!menuStack.empty()) {
        void(Menu::*previousMenu)() = menuStack.top();
        menuStack.pop();
        (this->*previousMenu)();
    } else {
        cerr << "Cant find last menu" << endl;
        exit(EXIT_FAILURE);
    }
}

