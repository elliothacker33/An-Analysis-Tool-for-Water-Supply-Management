#include <iostream>
#include "controller/manager.h"
int main() {
    Manager* m = new Manager();
    //Menu* menu = new Menu();
    m->importFiles("../data/Project1DataSetSmall/Cities_Madeira.csv","../data/Project1DataSetSmall/Reservoirs_Madeira.csv","../data/Project1DataSetSmall/Stations_Madeira.csv","../data/Project1DataSetSmall/Pipes_Madeira.csv");
    delete m;
    return 0;
}
