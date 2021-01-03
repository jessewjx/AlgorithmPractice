//
//  FordFulkerson.cpp
//  MaxFlow-MinCut
//

#include <iostream>
#include <stdio.h>
#include <queue>
#include <list>
#include <map>
#include <algorithm>
using namespace std;

/**
 * Impementation of Ford-Fulkerson algorithm to find maxflow-mincut on a flow network
 * Network graph is represented in adjacency-lists
 * Edmonds-Karp method has time complexity O(E^2V)
 * Capacity scaling method has time complexity O(E^2logCmax)
 * Reference: http://algs4.cs.princeton.edu/
 */

class Edge{
    public:
        int capacity;
        int flow;
        int startId;
        int endId;
        
        Edge(int v, int u, int c, int f){
            startId = v;
            endId = u;
            capacity = c;
            flow = f;
        }
        
        // Returns the residual capacity of the edge in the direction to the given vertex
        int residualCapacityto(int vId){
            if (vId == startId) return flow; // backward edge
            else if(vId == endId) return capacity - flow; // forward edge
            return -1;
        }
    
        //Increases the flow on the edge in the direction to the given vertex
        void addFlowto(int vId, int f){
            if (vId == startId) flow  -= f; // backward edge
            else if (vId == endId) flow += f; // forward edge
        }
    
        //Returns the endpoint of the edge that is different from the given vertex
        int otherId(int vId){
            if(vId == startId) return endId;
            else if (vId == endId) return startId;
            else return -1;
        }
    
};


class Graph{
    public:
        int numVertice;
        int numEdges;
        int startVertice;
        int endVertice;

        vector<vector<Edge>> adjEdges; // adjacent list of edges
        map<int,Edge*> augmentingEdgeto; // augmenting path with collection of pointers to edges
    
        void createEdge(int v, int w, int c){
            Edge e(v,w,c,0);
            adjEdges[v].push_back(e);
        }
    
        Graph(int V, int E, int s, int t){
            numVertice = V;
            numEdges = E;
            startVertice = s;
            endVertice = t;
            
            for(int i = 0; i < V; i ++){
                vector<Edge> lst;
                adjEdges.push_back(lst);
            }
        }
    
        void clearGraph(){
            for(int i = 0; i < numVertice; i ++){
                adjEdges[i].clear();
            }
            augmentingEdgeto.clear();
        }
};


class MaxFlowSolver{
public:
    int maxCapacity;
    int delta;
    Graph g;
    MaxFlowSolver(int V, int E, int maxCapacity, int s, int t):g(V,E,s,t),maxCapacity(maxCapacity){}

    bool BFS(){
        // BFS to find the augmenting path
            bool visited[g.numVertice];
            queue<int> q;
            q.push(g.startVertice);
            visited[g.startVertice] = true;

            while (!q.empty()){
                int u = q.front();
                q.pop();
                // iterate through the adjacent edges to each vertice
                for (auto it = begin (g.adjEdges[u]); it != end (g.adjEdges[u]); ++it) {
                    int w = it->otherId(u);
                    if(it->residualCapacityto(w) > delta && !visited[w]){
                        q.push(w);
                        visited[w] = true;
                        g.augmentingEdgeto[w] = &(*it);
                    }
                }
          }
        // whether there exists an augmenting path
        return visited[g.endVertice];
    }

    int EdmondsKarp(){
        // Shortest augmenting path heuristics
        int maxflow = 0;
        delta = 0;
        
        while (BFS()){
            // find bottleneck capacity
            int bottleneckCapacity = INT_MAX;
            for (int v = g.endVertice; v != g.startVertice; v = g.augmentingEdgeto.find(v)->second->otherId(v)) {
                int curEdgeCap = g.augmentingEdgeto.find(v)->second->residualCapacityto(v);
                bottleneckCapacity = min(bottleneckCapacity, curEdgeCap);
            }
            std::cout << bottleneckCapacity<<" unit bottleneck\n";

            // augment path in the residual graph by bottleneck capacity
            for (int v = g.endVertice; v != g.startVertice; v = g.augmentingEdgeto[v]->otherId(v)) {
                g.augmentingEdgeto.find(v)->second->addFlowto(v, bottleneckCapacity);
            }
            maxflow += bottleneckCapacity;
        }
        std::cout << "maxflow is: "<<maxflow << " \n";
        return maxflow;

    }

    int CapacityScaling(){
    // Augment path with residual capacity above the threshold in the residual graph
        int maxflow = 0;
        delta = maxCapacity;
        
        while (delta >=1){
                while (BFS()){
                    // find bottleneck capacity
                    int bottleneckCapacity = INT_MAX;
                    for (int v = g.endVertice; v != g.startVertice; v = g.augmentingEdgeto.find(v)->second->otherId(v)) {
                        int curEdgeCap = g.augmentingEdgeto.find(v)->second->residualCapacityto(v);
                        bottleneckCapacity = min(bottleneckCapacity, curEdgeCap);
                    }
                    std::cout << bottleneckCapacity<<" unit bottleneck\n";

                    // augment path in the residual graph by bottleneck capacity
                    for (int v = g.endVertice; v != g.startVertice; v = g.augmentingEdgeto[v]->otherId(v)) {
                        g.augmentingEdgeto.find(v)->second->addFlowto(v, bottleneckCapacity);
                    }
                    maxflow += bottleneckCapacity;
                }
                
            delta = delta / 2;
        }
        std::cout << "maxflow is: "<<maxflow << " \n";
        return maxflow;
    }
};


int main(int argc, const char * argv[]) {
    int V = 4; int E = 4;
    int s = 0; int t = 3;
    int maxCapacity = 8;
    
    MaxFlowSolver solver(V,E,maxCapacity,s,t);
    std::cout << "Capacity Scaling:\n";
    solver.g.createEdge(0,1,8);
    solver.g.createEdge(1,2,3);
    solver.g.createEdge(1,3,4);
    solver.g.createEdge(2,3,5);
    solver.CapacityScaling();
    
    solver.g.clearGraph();
    std::cout << "\nEdmonds Karp:\n";
    solver.g.createEdge(0,1,8);
    solver.g.createEdge(1,2,3);
    solver.g.createEdge(1,3,4);
    solver.g.createEdge(2,3,5);
    solver.EdmondsKarp();
    
};
