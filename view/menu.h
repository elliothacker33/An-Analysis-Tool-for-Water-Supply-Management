//
// Created by up202205140 on 06/03/2024.
//

#ifndef PROJECTDA_MENU_H
#define PROJECTDA_MENU_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "manager.h"
using namespace std;

class Menu{

    private:
        Manager mainManager;

    public:

        Manager getMainManager(){
            return mainManager;
        }

};

#endif //PROJECTDA_MENU_H
