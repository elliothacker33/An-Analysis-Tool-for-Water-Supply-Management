//
// Created by up202205140 on 06/03/2024.
//

#ifndef PROJECTDA_MANAGER_H
#define PROJECTDA_MANAGER_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Graph/graph.h"

using namespace std;

class Manager{

    private:
        Graph mainGraph;

    public:

        Graph getMainGraph(){
            return mainGraph;
        }

};


#endif //PROJECTDA_MANAGER_H
