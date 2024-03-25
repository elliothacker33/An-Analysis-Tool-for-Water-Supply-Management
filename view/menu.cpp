#include "menu.h"
#include <cmath>
#include <bits/random.h>

Menu::~Menu() {
    delete manager;
}

Menu::Menu(Manager* manager) {
    this->manager = manager;
}

bool Menu::getNumberInput(int minInput, int maxInput, int *option) {
    int input;
    cout << "Enter your choice ("<< minInput <<"-" << maxInput <<"): ";
    cin >> input;

    if (input >= minInput && input <= maxInput) {
        *option = input;
        return true;
    }

    cout << "Invalid input. Please enter a number between " << minInput << " and " << maxInput << endl;
    return false;
}

void Menu::getCityExamples() {
    auto cities = manager->getCities();
    if (cities.size() < 3) {
        cerr << "There are less than 3 cities in the map." << endl;
        return;
    }

    cout << "Example city codes:" << endl;
    int count = 0;
    for (const auto city : cities) {
        cout << city.first << endl;
        count++;
        if (count >= 3) {
            break;
        }
    }
}

bool Menu::validCity(string &code) {
    auto cities = manager->getCities();
    const auto start = code.find_first_not_of(" \t");
    const auto end = code.find_last_not_of(" \t");
    string trimmedInput;
    if (start != string::npos && end != string::npos)
        trimmedInput = code.substr(start, end - start + 1);
    else
        trimmedInput = "";

    return cities.find(trimmedInput) != cities.end();
}

bool Menu::getCity(string *city) {
    string input;
    getCityExamples();
    cout << "Enter a valid city code: ";
    cin >> input;
    cout << endl;

    for(char& c : input) {
        c = toupper(c);
    }

    if (validCity(input)) {
        *city = input;
        return true;
    }

    cout << "Invalid input. Please enter a valid city" << endl;
    return false;

}


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
        case 5:
            menuStack.push(&Menu::algorithmMenu);
            exercise32();
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
    string code;
    bool validResult;

    switch(option) {
        case 0:
            goBack();
            break;

        case 1:
            code.clear();
            do {

                validResult = getCity(&code);
            }
            while(!validResult);
            manager->getEdmondsKarpOneCity(code);
            exercise21();
            break;
        case 2:
            manager->getEdmondsKarpAllCities();
            exercise21();
            break;

        case 3:
            code.clear();
            do {
                validResult = getCity(&code);
            }
            while(!validResult);
            manager->getEdmondsKarpOneCity(code);
            exercise21();
            break;

        case 4:
            manager->getFordFulkersonAllCities();
            exercise21();
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
        cout << "             1. Disable each one (EK)           " << endl;
        cout << "             2. Disable each one (FF)           " << endl;
        cout << "             3. Disable selected ones (EK)      " << endl;
        cout << "             4. Disable selected ones (FF)      " << endl;
        cout << "                                                " << endl;
        cout << "------------------------------------------------" << endl;
    }
    while(!getNumberInput(0,4,&option));
    string code;
    bool validResult;

    switch(option) {
        case 0:
            goBack();
            break;

        case 1:
            manager->disableEachOneEdmondsKarp();
            exercise32();
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

