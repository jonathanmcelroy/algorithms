#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <algorithm>
#include <list>
#include <time.h>

#include "jm_ostream.h"
#include "jm_consts.h"
using namespace std;

//const int INF = 2000000000;

class Graph {
    private:
        int length;
        vector<int> nodesLengthToRoot;
        vector<vector<int> > graphTime;
        vector<vector<int> > graph;
        vector<int> cars;

        //vector<list<int> > graphTimeList;
        //vector<list<int> > graphList;

public:
    Graph(int length) {
        this->length = length;
        // make the graph have length+1 nodes, including the source, which is the first node
        this->graphTime.resize(length+1);
        this->graph.resize(length+1);
        this->nodesLengthToRoot.resize(length+1, INF);
        this->cars.resize(length+1, 0);
        for(int i=0; i<length+1; i++) {
            // initialize the time for each edge to be INF
            this->graphTime[i] = vector<int>(length+1, INF);
            // make 0 edges between the nodes
            this->graph[i] = vector<int>(length+1, 0);
        }
    }

    void addEdge(int node, int node2, int time) {
        // add the edge to both nodes

        // if we have not assigned this edge yet or the new time is smaller than the old one
        if (this->graph[node][node2] == 0 or time < this->graphTime[node][node2]) {
            this->graphTime[node][node2] = this->graphTime[node2][node] = time;
            this->graph[node][node2] = this->graph[node2][node] = 1;
        }
        // if we have assigned this edge and the times of the edges are the same
        else if(this->graphTime[node][node2] == time){
            this->graph[node][node2]++;
            this->graph[node2][node]++;
        }
    }

    void addCar(int node) {
        cars[node]++;
    }

    int solve() {

        int value = 0;

        // pq = [(dist to root, node num)]
        vector<pair<int, int> > pq;

        // add the root to the priority queue and get its distance to the root
        pq.push_back(make_pair(0, 1));
        int currentTime = 0;

        // the list of nodes the same distance to the root
        list<int> currentNodes;
        while(!pq.empty()) {
            // remove the node with the minimum time to the root
            pair<int, int> timeNode = pq.front();
            pop_heap(pq.begin(), pq.end(), greater<pair<int, int> >());
            pq.pop_back();

            // get the time an node number
            int time = timeNode.first;
            int node = timeNode.second;

            // if we have not explored this node; this means this is one of the shortest paths
            if (this->nodesLengthToRoot[node] == INF) {
                // set the length to the root
                this->nodesLengthToRoot[node] = time;

                // for each node
                for(int i=0; i<this->length; i++) {
                    // if there is a connection from another node to the current node
                    if(this->graph[i][node] > 0) {
                        // add the other node with a total time of this node's time plus the edge
                        pq.push_back(make_pair(time+graphTime[i][node], i));
                        push_heap(pq.begin(), pq.end(), greater<pair<int, int> >());

                        // remove the connection from this node to the other
                        // as we can be sure that this node is closer than the
                        // other to the root
                        this->graphTime[node][i] = INF;
                        this->graph[node][i] = 0;
                    }
                    // if there is a connection from this node to the other
                    // node and the edge plus the other time is greater than
                    // the current time, remove it
                    else if(this->graph[node][i] > 0 and this->graphTime[node][i]+this->nodesLengthToRoot[i] > time) {
                        this->graphTime[node][i] = INF;
                        this->graph[node][i] = 0;
                    }
                }

                // if node time to root != current time
                if(time != currentTime) {
                    // solve maximum flow problem with cars in current nodes
                    //value += this->maxFlow(currentNodes);
                    currentNodes.clear();
                    currentTime = time;
                }
                currentNodes.push_back(node);
            }
            else if (this->nodesLengthToRoot[node] > time) {
                exit(1);
            }
        }
        value += this->maxFlow(currentNodes);
        return value;
    }

    int maxFlow(list<int> nodes) {
        // using the edge weights matrix, we will solve the max flow over the whole graph, going from node 0 to node 1
        //
        // first connect all the nodes with cars to the source node with an edge weight equal to the number of cars
        // then solve the max flow problem
        int value = 0;
        bool anyCars = false;
        for(auto i=nodes.begin(); i!=nodes.end(); i++) {
            if((this->graph[0][*i] = cars[*i])) {
                anyCars = true;
            }
        }
        // path represents a path from the source to the sink. Every index points to the next node on the path, or to itself
        if (anyCars) {
            vector<int> path(this->length);
            while(this->findPath(0, 1, path)) {
                int flow = INF;
                for(int node = 0; node != 1; node = path[node]) {
                    flow = min(flow, this->graph[node][path[node]]);
                }
                for(int node = 0; node != 1; node = path[node]) {
                    this->graph[node][path[node]] -= flow;
                    this->graph[path[node]][node] += flow;
                }
                value += flow;
            }
        }
        resetGraph();
        return value;
    }

    bool findPath(int source, int sink, vector<int>& path) {
        // fills path with the path from node source to node sink
        for(unsigned int i=0; i<path.size(); i++) {
            path[i] = i;
        }
        vector<bool> visited(this->length, false);
        return findPath(source, sink, path, visited);
    }

    bool findPath(int current, int sink, vector<int>& path, vector<bool>& visited) {
        // fill path with the path from current to node sink
        visited[current] = true;
        if(this->graph[current][sink] > 0) {
            path[current] = sink;
            return true;
        }
        // we start at 2 because 0 has already been visited and 1 has been checked
        for(int i=0; i < this->length; i++) {
            if(visited[i] == false and this->graph[current][i] > 0) {
                path[current] = i;
                if(findPath(i, sink, path, visited)) {
                    return true;
                }
            }
        }
        path[current] = current;
        return false;
    }

    void resetGraph() {
        // this will find all paths from the sink to the source and reverse them
        vector<int> path(this->length);
        while(this->findPath(1, 0, path)) {
            int flow = INF;
            for(int node = 1; node != 0; node = path[node]) {
                flow = min(flow, this->graph[node][path[node]]);
            }
            for(int node = 1; node != 0; node = path[node]) {
                this->graph[node][path[node]] -= flow;
                this->graph[path[node]][node] += flow;
            }
        }
        this->graph[0] = vector<int>(this->length, 0);
    }
};

struct Info {
    int node1;
    int node2;
    int time;
    int weight;
    list<Info>::iterator reverseEdge;
};

class GraphList {
    private:
        int length;
        vector<int> nodesLengthToRoot;
        vector<int> cars;

        // vector of list of (node, time, weight, otherEdge, graphEdge, graphOtherEdge)
        vector<list<Info> > graphList;

public:
    GraphList(int length) {
        this->length = length;
        // make the graph have length+1 nodes, including the source, which is the first node
        this->nodesLengthToRoot.resize(length+1, INF);
        this->cars.resize(length+1, 0);
    }

    list<Info>::iterator findEdge(int node1, int node2) {
        auto index = this->graphList[node1].begin();
        for(; index!=this->graphList[node1].end(); index++) {
            if(index->node2 == node2) {
                break;
            }
        }
        return index;
    }

    void addEdge(int node1, int node2, int time) {
        // add the edge to both nodes

        // look for the node edge
        auto edge = this->findEdge(node1, node2);
        // if we find it
        if (edge!=this->graphList[node1].end()) {
            // if the new time is smaller than the old one
            if (time < edge->time) {
                // change my time and weight
                edge->time = time;
                edge->weight = 1;
                // change the reverse time and weight
                edge->reverseEdge->time = time;
                edge->reverseEdge->weight = 1;
            }
            // if the new time is the same as the old one
            else if(time == edge->time) {
                // increment my and my reverse weight
                edge->weight++;
                edge->reverseEdge->weight++;
            }
        }
        // if we have not assigned this edge yet
        else {
            Info edgeInfo = {node1, node2, time, 1, this->graphList[node2].end()};
            this->graphList[node1].push_back(edgeInfo);
            auto edgeIter = --this->graphList[node1].end();

            Info edgeReverse = {node2, node1, time, 1, edgeIter};
            this->graphList[node2].push_back(edgeReverse);

            edgeIter->reverseEdge = --this->graphList[node2].end();
        }
    }

    void addCar(int node) {
        cars[node]++;
    }

    int solve() {

        int value = 0;

        // pq = [(dist to root, node num)]
        vector<pair<int, int> > pq;

        // add the root to the priority queue and get its distance to the root
        pq.push_back(make_pair(0, 1));
        int currentTime = 0;

        // the list of nodes the same distance to the root
        list<int> currentNodes;
        while(!pq.empty()) {
            // remove the node with the minimum time to the root
            pair<int, int> timeNode = pq.front();
            pop_heap(pq.begin(), pq.end(), greater<pair<int, int> >());
            pq.pop_back();

            // get the time and node number
            int time = timeNode.first;
            int node1 = timeNode.second;

            // if we have not explored this node; this means this is one of the shortest paths
            if (this->nodesLengthToRoot[node1] == INF) {
                // set the length to the root
                this->nodesLengthToRoot[node1] = time;

                // for each node connected to this node
                for(auto edge = this->graphList[node1].begin(); edge != this->graphList[node1].end(); edge++) {

                    // if there is a connection from another node to the current node
                    // add the other node with a total time of this node's time plus the edge
                    if (edge->weight > 0) {
                        pq.push_back(make_pair(time+edge->time, edge->node2));
                        push_heap(pq.begin(), pq.end(), greater<pair<int, int> >());

                        // remove the connection from this node to the other
                        // as we can be sure that this node is closer than the
                        // other to the root
                        edge->reverseEdge->time = INF;
                        edge->reverseEdge->weight = 0;
                    }
                    // if there is a connection from this node to the other
                    // node and the edge plus the other time is greater than
                    // the current time, remove it
                    else if(edge->weight > 0 and edge->time+this->nodesLengthToRoot[node1] > time) {
                        auto node2 = edge->node2;
                        auto otherIter = edge->reverseEdge;
                        this->graphList[node1].erase(edge);
                        this->graphList[node2].erase(otherIter);
                    }
                }

                // if node time to root != current time
                if(time != currentTime) {
                    // solve maximum flow problem with cars in current nodes
                    value += this->maxFlow(currentNodes);
                    currentNodes.clear();
                    currentTime = time;
                }
                currentNodes.push_back(node1);
            }
            else if (this->nodesLengthToRoot[node1] > time) {
                exit(1);
            }
        }
        value += this->maxFlow(currentNodes);
        return value;
    }

    int maxFlow(list<int> nodes) {
        // using the edge weights, we will solve the max flow over the whole graph, going from node 0 to node 1
        //
        // first connect all the nodes with cars to the source node with an edge weight equal to the number of cars
        // then solve the max flow problem
        int value = 0;
        bool anyCars = false;
        for(auto i=nodes.begin(); i!=nodes.end(); i++) {
            if(cars[*i]) {
                anyCars = true;
                Info edgeInfo = {1, *i, 1, cars[*i], this->graphList[*i].end()};
                this->graphList[1].push_back(edgeInfo);
                auto edgeIter = --this->graphList[1].end();

                Info edgeReverse = {*i, 1, INF, 0, edgeIter};
                this->graphList[*i].push_back(edgeReverse);

                edgeIter->reverseEdge = --this->graphList[*i].end();
            }
        }
        // path represents a path from the source to the sink. Every index points to the next node on the path, or to itself
        list<list<Info>::iterator> edges;
        if (anyCars) {
            vector<int> path(this->length);
            while(this->findPath(0, 1, path)) {
                int flow = INF;
                for(int node = 0; node != 1; node = path[node]) {
                    auto edge = findEdge(node, path[node]);
                    flow = min(flow, edge->weight);
                    edges.push_back(edge);
                }
                for(auto edge = edges.begin(); edge != edges.end(); edge++) {
                    (*edge)->weight -= flow;
                    (*edge)->reverseEdge->weight += flow;
                }
                value += flow;
            }
        }
        resetGraph(edges);
        return value;
    }

    bool findPath(int source, int sink, vector<int>& path) {
        // fills path with the path from node source to node sink
        for(unsigned int i=0; i<path.size(); i++) {
            path[i] = i;
        }
        vector<bool> visited(this->length, false);
        return findPath(source, sink, path, visited);
    }

    bool findPath(int current, int sink, vector<int>& path, vector<bool>& visited) {
        // fill path with the path from current to node sink
        visited[current] = true;
        for(auto edge = graphList[current].begin(); edge != graphList[current].end(); edge++) {
            if (edge->node2 == sink) {
                path[current] = sink;
                return true;
            }
            else if (visited[edge->node2] == false and edge->weight > 0) {
                path[current] = edge->node1;
                if(findPath(edge->node1, sink, path, visited)) {
                    return true;
                }
            }
        }
        path[current] = current;
        return false;
    }

    void resetGraph(list<list<Info>::iterator> edges) {
        // this will find all paths from the sink to the source and reverse them
        vector<int> path(this->length);
        while(this->findPath(1, 0, path)) {
            int flow = INF;
            for(int node = 1; node != 0; node = path[node]) {
                flow = min(flow, this->graph[node][path[node]]);
            }
            for(int node = 1; node != 0; node = path[node]) {
                this->graph[node][path[node]] -= flow;
                this->graph[path[node]][node] += flow;
            }
        }
        this->graph[0] = vector<int>(this->length, 0);
    }
};

int main() {
    // get the number of nodes, edges and cars
    auto start = time(NULL);
    int nodes;
    while(cin >> nodes) {
        int edges, cars;
        cin >> edges >> cars;
        Graph graph(nodes);

        // get the list of edges. We will only keep the edges that give min
        // distances
        int nodenum1, nodenum2, time;
        for(int i=0; i<edges;i++) {
            cin >> nodenum1 >> nodenum2 >> time;
            graph.addEdge(nodenum1, nodenum2, time);
        }

        // get the nodes the cars are in
        int car;
        for(int i=0; i<cars; i++) {
            cin >> car;
            graph.addCar(car);
        }

        cout << graph.solve() << endl;
    }
    cout << time(NULL)-start << endl;

    return 0;
}

/* Pseudo-code
 *
 * downtown cars = 0
 * Build Graph
 * Place cars in graph
 * Create heap of vertexes ordered by distance to root with root in it
 * CurrentCars = []
 * CurrentDistance = 0
 * While there are nodes in the heap
 *      node = heap.first()
 *      minDistance = node.distToRoot()
 *      If minDistance < currentDistance:
 *          If currentCars.size() == 1:
 *              downtown cars++
 *          else if currentCars.size() > 1:
 *              Create a copy of the graph besides the current node
 *              newGraph = graph.copy()
 *              newGraph.trim() // Remove all unnecessary edges from the copy; meaning nodes not in any of the cars' paths
 *
 *              sort(newGraph.nodes, by=node.distToRoot())
 *              sort(newGraph.cars, by=cars.nodes) // Sort the cars by the nodes they can travel to; longer number of nodes go first
 *              while a change occurs:
     *              Compress connector nodes
     *              For each car in current cars:
     *                  If the car is next to the root or \
     *                          there is only one path to the root or \
     *                          all the paths contain the same nodes
     *                      downtown cars++
     *                      remove car from current cars
     *                      remove that path from the graph; all the edges in that path
     *                      if that reduce the number of outgoing edges in one of the nodes to 0:
     *                          remove the node and all the nodes that can only go to it
     *                  Else if the car has only one node to go to:
     *                      Move the car to the next node
     *                      Remove one of the edges to the new node from the old one
     *                      If the number of edges is now 0:
     *                          If the node has cars:
     *                              remove the cars
     *                          Remove the node and the edges connected to it
     *                  Else:
     *                      While we have not reached the root
 *              For each car in current cars reversed:
 *                  If car.paths().size() != 0
 *                      Chose the path such that each choice takes the lest merges
 *                      Remove the edges in that path
 *                      downtown cars++
 *                  Simplify the graph
     *          brute force
 *          currentDistance = minDistance
 *      Remove edges to already found nodes that result in a higher distance
 *      If cars exist in this node:
 *          Add cars to current car
 */
