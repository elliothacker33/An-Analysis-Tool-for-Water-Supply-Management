//
// Created by up202205140 on 06/03/2024.
//

#ifndef PROJECTDA_MENU_H
#define PROJECTDA_MENU_H
#include <stack>
#include "../controller/manager.h"
using namespace std;

class Menu{
    stack<void(Menu::*)()> menuStack;
    Manager* manager;

    /* Auxiliary functions */
    string removeLeadingTrailingSpaces(const string& input);

    /* Validity of input */
    bool validCity(string& code);
    bool validStation(string& code);
    bool validReservoir(string& code);
    bool isValidInterface(const string& type,string& code);

    /* Get input */
    vector<string> getItems(const unordered_map<string,Vertex*>&, const string& itemType);
    vector<string> getCities();
    vector<string> getStations();
    vector<string> getReservoirs();
    bool getNumberInput(int minInput, int maxInput, int* option);

    /* Get examples of input */
    void getItemExamples(const unordered_map<string, Vertex*>& items, const string& label);
    void getCityExamples();
    void getStationExamples();
    void getReservoirExamples();
    void getExamplesInterface(const string& type);


public:
    ~Menu();
    explicit Menu(Manager* manager);
    void mainMenu();
    void exitMenu();
    void goBack();
    void algorithmMenu();
    void exercise21();
    void exercise22();
    void exercise32();
    void extraMenu();
};

#endif //PROJECTDA_MENU_H
