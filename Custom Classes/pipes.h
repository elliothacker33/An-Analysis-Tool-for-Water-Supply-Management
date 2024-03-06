//
// Created by up202205140 on 06/03/2024.
//

#ifndef PROJECTDA_PIPES_H
#define PROJECTDA_PIPES_H
#include <string>
#include <iostream>
#include "element.h"

using namespace std;

class Pipe{
private:
    string origin;
    string dest;
    int capacity;
    bool direction;

public:

    Pipe();
    Pipe(string n, string m, int i, string c, int mD);

};
#endif //PROJECTDA_PIPES_H
