#include <iostream>
#include "controller/manager.h"
int main(){
    const auto m = new Manager();
    //Menu* menu = new Menu();
    m->importFiles("../data/Project1LargeDataSet/Cities.csv","../data/Project1LargeDataSet/Reservoir.csv","../data/Project1LargeDataSet/Stations.csv","../data/Project1LargeDataSet/Pipes.csv");
    m->maxFlowEdmondsKarp();
    delete m;
    return 0;
}

