#include <iostream>
#include "controller/manager.h"
int main() {
    auto* m = new Manager();
    //Menu* menu = new Menu();
    m->importFiles("../data/Project1DataSetSmall/Cities_Madeira.csv","../data/Project1DataSetSmall/Reservoirs_Madeira.csv","../data/Project1DataSetSmall/Stations_Madeira.csv","../data/Project1DataSetSmall/Pipes_Madeira.csv");
    #include <iostream>

for (const auto v : m->getGraph()->getVertexSet()) {
    if (const auto derivedCity = dynamic_cast<City*>(v)) {
        derivedCity->getCode();
        cout << derivedCity->getOutDegree() <<"out" <<endl;
        cout << derivedCity->getInDegree() << "in" << endl;
        for (const auto e : derivedCity->getIncoming()) {
            auto dest = e->getOrigin();
            if (const auto derivedCityDest = dynamic_cast<City*>(dest)) {
                // Handle City destination
                std::cout << derivedCity->getCode() << derivedCityDest->getCode() << std::endl;
            } else if (const auto derivedStationDest = dynamic_cast<Station*>(dest)) {
                // Handle Station destination
                std::cout << derivedCity->getCode() << derivedStationDest->getCode() << std::endl;
            } else if (const auto derivedReservoirDest = dynamic_cast<Reservoir*>(dest)) {
                // Handle Reservoir destination
                std::cout << derivedCity->getCode() << derivedReservoirDest->getCode() << std::endl;
            } else {
                std::cout << "Unknown destination type detected.\n";
            }
        }
        std::cout << "City vertex detected.\n";
    }
    // Test if the vertex is of type Station
    else if (auto derivedStation = dynamic_cast<Station*>(v)) {
        derivedStation->getCode();
        cout << derivedStation->getOutDegree() <<"out" <<endl;
        cout << derivedStation->getInDegree() << "in" << endl;
        for (const auto e : derivedStation->getIncoming()) {
            auto dest = e->getOrigin();
            if (const auto derivedCityDest = dynamic_cast<City*>(dest)) {
                // Handle City destination
                std::cout << derivedStation->getCode() << derivedCityDest->getCode() << std::endl;
            } else if (const auto derivedStationDest = dynamic_cast<Station*>(dest)) {
                // Handle Station destination
                std::cout << derivedStation->getCode() << derivedStationDest->getCode() << std::endl;
            } else if (const auto derivedReservoirDest = dynamic_cast<Reservoir*>(dest)) {
                // Handle Reservoir destination
                std::cout << derivedStation->getCode() << derivedReservoirDest->getCode() << std::endl;
            } else {
                std::cout << "Unknown destination type detected.\n";
            }
        }
        std::cout << "Station vertex detected.\n";
    }
    // Test if the vertex is of type Reservoir
    else if (auto derivedReservoir = dynamic_cast<Reservoir*>(v)) {
        derivedReservoir->getCode();
        cout << derivedReservoir->getOutDegree() <<"out" <<endl;
        cout << derivedReservoir->getInDegree() << "in" << endl;
        for (const auto e : derivedReservoir->getIncoming()) {
            auto dest = e->getOrigin();
            if (const auto derivedCityDest = dynamic_cast<City*>(dest)) {
                // Handle City destination
                std::cout << derivedReservoir->getCode() << derivedCityDest->getCode() << std::endl;
            } else if (const auto derivedStationDest = dynamic_cast<Station*>(dest)) {
                // Handle Station destination
                std::cout << derivedReservoir->getCode() << derivedStationDest->getCode() << std::endl;
            } else if (const auto derivedReservoirDest = dynamic_cast<Reservoir*>(dest)) {
                // Handle Reservoir destination
                std::cout << derivedReservoir->getCode() << derivedReservoirDest->getCode() << std::endl;
            } else {
                std::cout << "Unknown destination type detected.\n";
                }
            }
        }
    }

    delete m;
    return 0;
}

