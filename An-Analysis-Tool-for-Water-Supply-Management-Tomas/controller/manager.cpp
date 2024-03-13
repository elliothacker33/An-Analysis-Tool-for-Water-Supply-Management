//
// Created by up202205140 on 13/03/2024.
//

#include <sstream>
#include "fstream"
#include "manager.h"
using namespace std;

void Manager::importCities(){
    fstream fin;

    // Open an existing file
    fin.open("../DataSet/Cities_Madeira.csv", ios::in);

    // Read the Data from the file
    // as String Vector
    vector<string> row;
    string line, word, temp;
    l = 0;

    while (fin >> temp) {

        row.clear();

        // read an entire row and
        // store it in a string variable 'line'
        getline(fin, line);

        // used for breaking words
        stringstream s(line);

        // read every column data of a row and
        // store it in a string variable, 'word'
        while(getline(s, word, ',')){

            // add all the column data
            // of a row to a vector
            row.push_back(word);
        }
        l += 1;
        if(l == 1)continue;

        mainGraph.add_vertex(row[2], new City(row[0], stoi(row[1]), row[2], stof(row[3]), row[4]));
    }
}

void Manager::importReservoirs() {

    fstream fin;

    // Open an existing file
    fin.open("../DataSet/Reservoirs_Madeira.csv", ios::in);

    // Read the Data from the file
    // as String Vector
    vector<string> row;
    string line, word, temp;
    l = 0;

    while (fin >> temp) {

        row.clear();

        // read an entire row and
        // store it in a string variable 'line'
        getline(fin, line);

        // used for breaking words
        stringstream s(line);

        // read every column data of a row and
        // store it in a string variable, 'word'
        while(getline(s, word, ',')){

            // add all the column data
            // of a row to a vector
            row.push_back(word);
        }
        l += 1;
        if(l == 1)continue;

        mainGraph.add_vertex(row[3], new Reservoir(row[0], row[1], row[3], stoi(row[4]), stoi(row[2])));
    }

}

void Manager::importStations() {
    fstream fin;

    // Open an existing file
    fin.open("../DataSet/Stations_Madeira.csv", ios::in);

    // Read the Data from the file
    // as String Vector
    vector<string> row;
    string line, word, temp;
    l = 0;

    while (fin >> temp) {

        row.clear();

        // read an entire row and
        // store it in a string variable 'line'
        getline(fin, line);

        // used for breaking words
        stringstream s(line);

        // read every column data of a row and
        // store it in a string variable, 'word'
        while(getline(s, word, ',')){

            // add all the column data
            // of a row to a vector
            row.push_back(word);
        }
        l += 1;
        if(l == 1)continue;

        mainGraph.add_vertex(row[1], new Station(row[1], stoi(row[0])));
    }
}


void Manager::importPipes() {

    fstream fin;

    // Open an existing file
    fin.open("../DataSet/Pipes_Madeira.csv", ios::in);

    // Read the Data from the file
    // as String Vector
    vector<string> row;
    string line, word, temp;
    l = 0;

    while (fin >> temp) {

        row.clear();

        // read an entire row and
        // store it in a string variable 'line'
        getline(fin, line);

        // used for breaking words
        stringstream s(line);

        // read every column data of a row and
        // store it in a string variable, 'word'
        while(getline(s, word, ',')){

            // add all the column data
            // of a row to a vector
            row.push_back(word);
        }
        l += 1;
        if(l == 1)continue;

        mainGraph.add_edge(row[2], new Edge(stoi(row[2]), mainGraph.get_vertexSet().find(row[0])->second, mainGraph.get_vertexSet().find(row[1])->second));
    }

}

void Manager::importFiles(){

    importCities();
    importReservoirs();
    importStations();
}
