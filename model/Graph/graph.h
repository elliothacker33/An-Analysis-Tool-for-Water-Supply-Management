//
// Created by up202205140 on 06/03/2024.
//

#ifndef PROJECTDA_GRAPH_H
#define PROJECTDA_GRAPH_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include "../Custom Classes/vertexParent.h"

using namespace std;



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

            //int elementCount = -1;

            unordered_map<string, class Vertex*> vertexSet;
            int vertexCount = 0;

            unordered_map<string, class Edge*> edgeSet;
            int edgeCount = 0;

        public:

            Graph();

            unordered_map<string, class Vertex*> get_vertexSet(){
                return vertexSet;
            };

            unordered_map<string, class Edge*> get_edgeSet(){
                return edgeSet;
            };

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
