//
// Created by up202205140 on 06/03/2024.
//

#ifndef PROJECTDA_CITY_H
#define PROJECTDA_CITY_H

#include <string>
#include <iostream>
#include "../vertexParent.h"

using namespace std;

class City : public vertexParent{
private:
    string name;
    string code;


    int demand;
    int population;
    int id;


public:

    City();
    City(string n, string m, int i, string c, int mD);

};

#endif //PROJECTDA_CITY_H
