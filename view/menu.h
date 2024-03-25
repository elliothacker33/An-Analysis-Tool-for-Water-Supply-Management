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
    bool getNumberInput(int minInput, int maxInput, int* option);
    bool getCity(string* city);
    bool validCity(string& code);
    void getCityExamples();
public:
    ~Menu();
    Menu(Manager* manager);
    void mainMenu();
    void exitMenu();
    void goBack();
    void algorithmMenu();
    void exercise21();
    void extraMenu();
};

#endif //PROJECTDA_MENU_H
