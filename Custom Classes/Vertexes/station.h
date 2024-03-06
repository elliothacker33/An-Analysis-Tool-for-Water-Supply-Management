//
// Created by up202205140 on 06/03/2024.
//

#ifndef PROJECTDA_STATION_H
#define PROJECTDA_STATION_H

#include <string>
#include <iostream>
#include "../vertexParent.h"

using namespace std;

class Station : public vertexParent{
private:
    int id;
    string code;

public:

    Station();
    Station(string n, string m, int i, string c, int mD);

};

#endif //PROJECTDA_STATION_H
