#include "menu.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <ctime>


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
/* Timer functions */
void Menu::startTimer(timespec& start_real, timespec& start_cpu) const {
    clock_gettime(CLOCK_REALTIME, &start_real);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_cpu);
}

void Menu::stopTimer(timespec& start_real, timespec& start_cpu, double& elapsed_real, double& elapsed_cpu) const {
    timespec end_real, end_cpu;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_cpu);
    clock_gettime(CLOCK_REALTIME, &end_real);

    elapsed_real = (end_real.tv_sec - start_real.tv_sec) +
                   (end_real.tv_nsec - start_real.tv_nsec) / 1e9;

    elapsed_cpu = (end_cpu.tv_sec - start_cpu.tv_sec) +
                  (end_cpu.tv_nsec - start_cpu.tv_nsec) / 1e9;

    cout << "Elapsed real time: " << elapsed_real << " seconds" << endl;
    cout << "Elapsed CPU time: " << elapsed_cpu << " seconds" << endl;
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
        case 3:
            menuStack.push(&Menu::algorithmMenu);
            exercise23();
            break;
        case 4:
            menuStack.push(&Menu::algorithmMenu);
            exercise31();
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
    timespec start_real;
    timespec start_cpu;
    double elapsed_real, elapsed_cpu;

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
                startTimer(start_real,start_cpu);
                manager->getEdmondsKarpXCity(cities);
                stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            }
            exercise21();
            break;
        case 2:
            startTimer(start_real,start_cpu);
            manager->getEdmondsKarpAllCities();
            stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            exercise21();
            break;

        case 3:
            cities = getCities();
            if (cities.empty()) {
                cout << "No cities were selected" << endl;
            }
            else {
                startTimer(start_real,start_cpu);
                manager->getFordFulkersonXCity(cities);
                stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            }
            exercise21();
            break;

        case 4:
            startTimer(start_real,start_cpu);
            manager->getFordFulkersonAllCities();
            stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
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

    timespec start_real;
    timespec start_cpu;
    double elapsed_real, elapsed_cpu;

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
                startTimer(start_real,start_cpu);
                manager->canCityXGetEnoughWaterEK(cities);
                stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            }
            exercise22();
            break;
        case 2:
            cities = getCities();
            if (cities.empty()) {
                cout << "No cities were selected" << endl;
            }
            else {
                startTimer(start_real,start_cpu);
                manager->canCityXGetEnoughWaterFF(cities);
                stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            }
            exercise22();
            break;

        case 3:
            startTimer(start_real,start_cpu);
            manager->canAllCitiesGetEnoughWaterEK();
            stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            exercise22();
            break;

        case 4:
            startTimer(start_real,start_cpu);
            manager->canAllCitiesGetEnoughWaterFF();
            stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
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
        cout << "     0. Go back                                 " << endl;
        cout << "     1. First Use Edmonds Karp Algorithm for the Control case " << endl;
        cout << "     2. First Use Ford Fulkerson Algorithm for the Control case " << endl;
        cout << "------------------------------------------------" << endl;
    }
    while(!getNumberInput(0,2,&option));

    timespec start_real;
    timespec start_cpu;
    double elapsed_real, elapsed_cpu;

    switch(option) {
        case 0:
            goBack();
            break;

        case 1:
            menuStack.push(&Menu::exercise23);
            manager->getEdmondsKarpAllCities(false);
            break;
        case 2:
            menuStack.push(&Menu::exercise23);
            manager->getFordFulkersonAllCities(false);
            break;

    }
    startTimer(start_real,start_cpu);
    manager->improvePipesHeuristic();
    stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
    exercise23();

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
    timespec start_real;
    timespec start_cpu;
    double elapsed_real, elapsed_cpu;

    switch(option) {
        case 0:
            goBack();
            break;
        case 1:
            startTimer(start_real,start_cpu);
            manager->disableEachReservoirEdmondsKarp();
            stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            exercise31();
            break;
        case 2:
            startTimer(start_real,start_cpu);
            manager->disableEachReservoirFordFulkerson();
            stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            exercise31();
            break;
        case 3:
            reservoirs = getReservoirs();
            if (reservoirs.empty()) {
                cout << "No reservoirs were selected" << endl;
            }
            else {
                startTimer(start_real,start_cpu);
                manager->disableSelectedReservoirsEdmondsKarp(reservoirs);
                stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            }
            exercise31();
            break;
        case 4:
            reservoirs = getReservoirs();
            if (reservoirs.empty()) {
                cout << "No reservoirs were selected" << endl;
            }
            else {
                startTimer(start_real,start_cpu);
                manager->disableSelectedReservoirsFordFulkerson(reservoirs);
                stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
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
    timespec start_real;
    timespec start_cpu;
    double elapsed_real, elapsed_cpu;

    switch(option) {
        case 0:
            goBack();
            break;
        case 1:
            startTimer(start_real,start_cpu);
            manager->disableEachStationEdmondsKarp();
            stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            exercise32();
            break;
        case 2:
            startTimer(start_real,start_cpu);
            manager->disableEachStationFordFulkerson();
            stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            exercise32();
            break;
        case 3:
            stations = getStations();
            if (stations.empty()) {
                cout << "No stations were selected" << endl;
            }
            else {
                startTimer(start_real,start_cpu);
                manager->disableSelectedStationsEdmondsKarp(stations);
                stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            }
            exercise32();
            break;
        case 4:
            stations = getStations();
            if (stations.empty()) {
                cout << "No stations were selected" << endl;
            }
            else {
                startTimer(start_real,start_cpu);
                manager->disableSelectedStationsFordFulkerson(stations);
                stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            }
            exercise32();
            break;

    }
}

void Menu::exercise33() {
    int option = 0;
    do {
        cout << "------------------------------------------------" << endl;
        cout << "              Menu -> Exercice 3.3              " << endl;
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
    timespec start_real;
    timespec start_cpu;
    double elapsed_real, elapsed_cpu;

    switch(option) {
        case 0:
            goBack();
            break;
        case 1:
            startTimer(start_real,start_cpu);
            manager->disableEachPipeEdmondsKarp();
            stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            exercise33();
            break;
        case 2:
            startTimer(start_real,start_cpu);
            manager->disableEachPipeFordFulkerson();
            stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            exercise33();
            break;
        case 3:
            pipes = getPipes();
            if (pipes.empty()) {
                cout << "No pipes were selected" << endl;
            }
            else {
                startTimer(start_real,start_cpu);
                manager->disableSelectedPipesEdmondsKarp(pipes);
                stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            }
            exercise33();
            break;
        case 4:
            pipes = getPipes();
            if (pipes.empty()) {
                cout << "No pipes were selected" << endl;
            }
            else {
                startTimer(start_real,start_cpu);
                manager->disableSelectedPipesFordFulkerson(pipes);
                stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
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
    timespec start_real;
    timespec start_cpu;
    double elapsed_real, elapsed_cpu;

    switch(option) {
        case 0:
            goBack();
            break;

        case 1:
            maxK = manager->getCities().size();
            do {
                validResult = getNumberInput(1,maxK,&k);
            }
            while(!validResult);
            startTimer(start_real,start_cpu);
            manager->topKFlowEdmondsKarpCities(k);
            stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            extraMenu();
            break;
        case 2:
            maxK =manager->getCities().size();
            do {
                validResult = getNumberInput(1,maxK,&k);
            }
            while(!validResult);
            startTimer(start_real,start_cpu);
            manager->topKFlowFordFulkersonCities(k);
            stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            extraMenu();
            break;

        case 3:
            startTimer(start_real,start_cpu);
            manager->flowRatePerCityEdmondsKarp();
            stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
            extraMenu();
            break;
        case 4:
            startTimer(start_real,start_cpu);
            manager->flowRatePerCityFordFulkerson();
            stopTimer(start_real,start_cpu,elapsed_real,elapsed_cpu);
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

