//
// Created by up202205140 on 06/03/2024.
//

#ifndef PROJECTDA_GRAPH_H
#define PROJECTDA_GRAPH_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include "../Custom Classes/vertexParent.h"
#include "../Custom Classes/pipes.h"

using namespace std;


class Vertex;
class Edge;
class Graph;


////EDGE
class Edge{

private:

    Pipe *info;
    Vertex *dest;
    Vertex *origin;


public:

    Edge();
    Edge(Vertex *d, Vertex *o){
        dest = d;
        origin = o;
    };

    Pipe* getInfo(){
        return info;
    }

    Vertex* getDest(){
        return dest;
    }

    Vertex* getOrigin(){
        return origin;
    }
};

////VERTEX
    class Vertex{

        private:
            class vertexParent *info;
            int inDegree = 0;
            int outDegree = 0;

            vector<class Edge*> adjacentEdges;


        public:
            
            Vertex();
        
            vector<class Edge*> getAdj() {
                return adjacentEdges;
            }

            int getInDegree(){
                return inDegree;
            }

            void addIndegree(int i) {
                inDegree += i;
            }

            void addEdge(Edge *e){
                outDegree += 1;

                e->getDest()->addIndegree(1);

                adjacentEdges.push_back(e);
            }

            void removeEdge(Edge *e){
                outDegree += -1;

                e->getDest()->addIndegree(-1);

                for(int i = 0; i < outDegree; i++){
                    Edge* d = adjacentEdges.at(i);

                    if(d == e){
                        adjacentEdges.erase(next(adjacentEdges.begin(), i));
                        break;
                    }
                };
            }
    };

////GRAPH

    class Graph{

    private:

        int elementCount = -1;

        unordered_map<string, class Vertex*> vertexSet;
        unordered_map<string, class Edge*> edgeSet;

    public:

        Graph();
        vector<class Vertex> get_vertexSet();

        int getElementCount(){
            elementCount += 1;
            return elementCount;
        }
    };



#endif //PROJECTDA_GRAPH_H
