//
// Created by up202205140 on 06/03/2024.
//

#ifndef PROJECTDA_MANAGER_H
#define PROJECTDA_MANAGER_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include "../model/Graph/graph.h"

using namespace std;

class Manager{

    private:
        Graph mainGraph;
        int l = 0;

    public:

        void importFiles();
        void importCities();

        Manager(){
            importFiles();
        };

        Graph getMainGraph(){
            return mainGraph;
        }

    void importPipes();

    void importReservoirs();

    void importStations();
};


#endif //PROJECTDA_MANAGER_H
