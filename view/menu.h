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

    /* Timer functions */
    void startTimer(timespec& start_real, timespec& start_cpu) const;
    void stopTimer(timespec& start_real, timespec& start_cpu, double& elapsed_real, double& elapsed_cpu) const;

    /* Validity of input */
    Edge* validPipe(string& code1, string& code2);
    bool validCity(string& code);
    bool validStation(string& code);
    bool validReservoir(string& code);
    bool isValidInterface(const string& type,string& code);

    /* Get input */
    vector<string> getVertex(const unordered_map<string,Vertex*>&, const string& itemType);
    vector<string> getCities();
    vector<string> getStations();
    vector<string> getReservoirs();
    vector<Edge*> getPipes();
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

    /* Generic Menus */
    void mainMenu();
    void exitMenu();
    void goBack();

    /* Project menus */
    void algorithmMenu();
    void exercise21();
    void exercise22();
    void exercise23();
    void exercise31();
    void exercise32();
    void exercise33();

    /* Extra menu with other algorithms */
    void extraMenu();
};

#endif //PROJECTDA_MENU_H
