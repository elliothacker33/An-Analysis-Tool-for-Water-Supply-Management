//
// Created by up202205140 on 06/03/2024.
//

#ifndef PROJECTDA_RESERVOIR_H
#define PROJECTDA_RESERVOIR_H
#include <string>
#include <iostream>
#include "../vertexParent.h"

using namespace std;

class Reservoir : public vertexParent{
    private:
        string name;
        string municipality;
        int id;
        string code;
        int maxDelivery;

    public:

        Reservoir();
        Reservoir(string n, string m, int i, string c, int mD);

};


#endif //PROJECTDA_RESERVOIR_H
