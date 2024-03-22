#include <iostream>
#include "controller/manager.h"
int main() {
    Manager m;
    m.importFiles("../data/Project1DataSetSmall/Cities_Madeira.csv","../data/Project1DataSetSmall/Reservoirs_Madeira.csv","../data/Project1DataSetSmall/Stations_Madeira.csv","../data/Project1DataSetSmall/Pipes_Madeira.csv");
    return 0;
}
