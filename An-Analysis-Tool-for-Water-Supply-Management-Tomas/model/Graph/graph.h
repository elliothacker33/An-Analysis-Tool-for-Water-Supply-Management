//
// Created by up202205140 on 06/03/2024.
//

#ifndef PROJECTDA_GRAPH_H
#define PROJECTDA_GRAPH_H
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Edge;
class Vertex;
class Graph;

class Edge{

    public:
        Edge(int capacity, Vertex *d, Vertex *o);
        Edge(Edge& other); // Ideal to use for reverse edges.
        void setReverseEdge(Edge* e);
        void setFlow(int flow);
        double getFlow() const;
        Edge* getReverseEdge() const;
        int getCapacity() const;
        Vertex* getDest() const;
        Vertex* getOrigin() const;

    private:
        Vertex *dest;
        Vertex *origin;
        int capacity;
        Edge* reverse = nullptr;
        double flow;

    };

class Vertex {

    public:
        virtual const char getType() const = 0;
};

// Derived class representing a City
    class City : public Vertex {
    public:
        City(const string& name, int id, const string& code, const float demand, const string population)
            : name(name), code(code), demand(demand), population(population), id(id) {}

        const char getType() const override;

        // Getters
        string getName() const;
        string getCode() const;
        string getPopulation() const;
        float getDemand() const;
        int getId() const;


    private:
        string name;
        string code;
        float demand;
        string population;
        int id;
    };

// Derived class representing a Station
    class Station : public Vertex {
    public:
        Station(const string& code, const int id) : code(code),id(id){}

        const char getType() const override;

        // Getters
        string getCode() const;
        int getId() const;


    private:
        string code;
        int id;
    };

// Derived class representing a Reservoir
    class Reservoir : public Vertex {
    public:
        Reservoir(const string& name,const string& municipality,const string& code,const int maxDelivery,const int id)
            : name(name), code(code), municipality(municipality), maxDelivery(maxDelivery), id(id) {}

        const char getType() const override;
        // Getters
        string getName() const;
        string getMunicipality() const;
        string getCode() const;
        int getMaxDelivery() const;
        int getId() const;


    private:
        string name;
        string municipality;
        string code;
        int maxDelivery;
        int id;
    };


////GRAPH
    class Graph{

        private:

            //int elementCount = -1;

            unordered_map<string, class Vertex*> vertexSet;
            int vertexCount = 0;

            unordered_map<string, class Edge*> edgeSet;
            int edgeCount = 0;

        public:

            Graph() = default;

            unordered_map<string, class Vertex*> get_vertexSet(){
                return vertexSet;
            };

            unordered_map<string, class Edge*> get_edgeSet(){
                return edgeSet;
            };

            void add_vertex(string st, Vertex* vt){
                vertexSet.insert({st, vt});
            }

            void add_edge(string st, Edge* vt){
                edgeSet.insert({st, vt});
            }

            int getVertexCount(){
                return vertexCount;
            }

            int getEdgeCount(){
                return edgeCount;
            }

            /*int getElementCount(){
                elementCount += 1;
                return elementCount;
            }*/
    };



#endif //PROJECTDA_GRAPH_H
