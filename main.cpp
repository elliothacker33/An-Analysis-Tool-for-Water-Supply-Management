#include <iostream>
#include "controller/manager.h"
int main(){
    const auto m = new Manager();
    //Menu* menu = new Menu(m);
    //menu->startMenu(m);
    m->importFiles("../data/Project1LargeDataSet/Cities.csv","../data/Project1LargeDataSet/Reservoir.csv","../data/Project1LargeDataSet/Stations.csv","../data/Project1LargeDataSet/Pipes.csv");
    cout << m->getGraph()->getNumberOfVertexes() << endl;
    m->maxFlowEdmondsKarp();
    cout << m->getGraph()->getNumberOfVertexes() << endl;
    m->resetGraph();
    cout << m->getGraph()->getNumberOfVertexes() << endl;

    for (const auto v: m->getGraph()->getVertexSet()) {
        for (const auto e : v->getAdj()) {
            if (e->getType() == "residual") cout << "True" << endl;
        }
    }
    delete m;
    return 0;
}

