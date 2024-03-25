
#ifndef PROJECTDA_GRAPH_H
#define PROJECTDA_GRAPH_H


class Vertex;
class Edge;
using namespace std;
#include <string>
#include <vector>
#include <iostream>
class Edge{

public:
    Edge(Vertex* orig, Vertex* dest, int capacity,const string& type);
    void setReverseEdge(Edge* e);
    void setFlow(int flow);
    int getFlow() const;
    Edge* getReverseEdge() const;
    int getCapacity() const;
    const string& getType() const;
    void setType(const string& type);
    Vertex* getDest() const;
    Vertex* getOrigin() const;

protected:
    Vertex* orig;
    Vertex* dest;
    int capacity;
    string type;
    Edge* reverse = nullptr; // Reverse is a new Edge, not a reference or copy.
    int flow = 0;
};


class Vertex {
protected:
    bool visited = false;
    int outDegree = 0;
    int inDegree = 0;
    vector<Edge*> adj;
    vector<Edge*> incoming;
    Edge* path = nullptr;
public:
    virtual char getType() const = 0;
    ~Vertex();
    int getOutDegree() const;
    void setOutDegree(int outDegree);
    int getInDegree() const;
    void setInDegree(int inDegree);
    bool isVisited() const;
    void setVisited(bool isVisited);
    Edge* addEdge(Vertex* t,int capacity,const string& type);
    void removeEdge(const Edge* e);
    void setPath(Edge* e);
    Edge* getPath() const;
    void removeEdgeIncoming(const Edge* e);
    bool addIncoming(Edge* e);
    vector<Edge*> getIncoming();
    vector<Edge*> getAdj();

};

// Derived class representing a City
class City : public Vertex {
public:
    City(const string& name,const int id,const string& code,const int demand,const int population)
        : name(name), code(code), demand(demand), population(population), id(id) {}

    char getType() const override;

    // Getters
    string getName() const;
    string getCode() const;
    int getDemand() const;
    int getPopulation() const;
    int getId() const;


private:
    string name;
    string code;
    int demand;
    int population;
    int id;
};

// Derived class representing a Station
class Station : public Vertex {
public:
    Station(const int id,const string& code) : code(code),id(id){}
    char getType() const override;
    // Getters
    string getCode() const;
    int getId() const;


private:
    string code;
    int id;
};

// Derived class representing a Reservoir
class Reservoir : public Vertex {
    // TODO: cout for the derived classes and add to Vertex common functions as virtual
public:
    Reservoir(const string& name,const string& municipality,const int id, const string& code,const int maxDelivery)
        : name(name), code(code), municipality(municipality), maxDelivery(maxDelivery), id(id) {}

    char getType() const override;
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
public:
    ~Graph();
    vector<Vertex*> getVertexSet() const;
    int getNumberOfVertexes() const;
    bool addVertex(Vertex* v);
    void removeVertex(Vertex* v);
    void removeEdge(const Edge* e);
    Edge* addEdge(Vertex* orig, Vertex* dest,int capacity,const string& type);
    static string getCode(Vertex* v);
protected:
    vector<Vertex*> vertexSet;
    int n = 0; // Size of vertexSet dynamic.
};



#endif //PROJECTDA_GRAPH_H
