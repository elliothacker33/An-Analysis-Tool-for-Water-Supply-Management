#include <iostream>
#include "controller/manager.h"
int main() {
    auto* m = new Manager();
    //Menu* menu = new Menu();
    m->importFiles("../data/Project1DataSetSmall/Cities_Madeira.csv","../data/Project1DataSetSmall/Reservoirs_Madeira.csv","../data/Project1DataSetSmall/Stations_Madeira.csv","../data/Project1DataSetSmall/Pipes_Madeira.csv");
    #include <iostream>
    auto graph = m->getGraph();
    Vertex* k = nullptr;
    for (const auto v : graph->getVertexSet()) {
        k = v;
        while(!v->getAdj().empty())
            graph->removeEdge(v->getAdj().front());
        break;
    }
    cout << k->getAdj().size() << k->getOutDegree() << endl;
    graph->removeVertex(k);
    cout << "Nodes=" << m->getGraph()->getNumberOfVertexes() << endl;
    delete m;
    return 0;
}

