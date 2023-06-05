# CN_CHomeworks_3

## How to run?

using the terminal cd to CN_CHomworks_3 directory and then:

```cpp
$ make
```

then run:

```cpp
$./main.out
```

## Project Description

This project compares the speed of different routing protocols including Border Gateway (BGP), Link State(LSRP), and Distance Vector (DVRP).

## Commands

### topology

Define a new topology:

```
topology s1-d1-c1 s2-d2-c2 ...
```

It means that there is a path from node s1 to node d1 with cost c1.

### show

Show the connection between different nodes along with the cost between them:

```
show
```

### remove

Removes the edge between two nodes:

```
remove s-d
```

### modify

Modifies the cost of an edge:

```
modify s-d-c
```

### lsrp

It takes the source node and prints the routing table and details of each iteration. The printed table shows the shortest path from given node to all nodes in network:

```
lsrp s
```

Where s is the source node.

### dvrp

It takes the source node and prints the routing table and details of each iteration. The printed table shows the shortest path from given node to all nodes in network:

```
dvrp s
```

where s is the source node.

## Definition of the Classes

We define two classes:

- Network  
  The Network class includes nodes, and edges of the topology; It also stores the weight of each edge with mapping the pairs of nodes to the weight between them.

```cpp
class Network
{
public:
    Network();
    void addEdge(string networkTopology);
    void show();
    int countDigit(int num);
    void DVRP(int src);
    void allSrcDVRP();
    void LSRP(int src);
    void allSrcLSRP();
    void removeEdge(int v, int u);
    void modifyEdge(int v, int u, int cost);
private:
    set<int> nodes;
    map<pair<int, int>, int> weights;
    void printDVRPres(int src, map<int, int> dist, map<int, int> nextHopFromSrc);
    int findMinDistNode(map<int, int> &dist, map <int, bool> &isMarked);
    void printLsrpIterationDists(const vector<vector<int>> &itersInfo);
    void printLsrpPaths(const int src, map<int, int> &dist, map<int, int> &nextHop);
};
```

- CommandHandler  
  The CommandHandler class includes an instance of the network class and calls different Network class's functions according to the user's command.

```cpp
class CommandHandler
{
public:
    CommandHandler();
    vector <string> tokenizeWithDelim(string cmdArgs);
    void run();
private:
    Network network;
};

```

- errors.hpp:  
   This header file includes definition and codes of different errors.

## Commands Functions

- topology  
  By entering this command, the commandHandler calls the addEdge function. In this function we split the commandArgs to get the nodes and edges; If two nodes of the same edge is the same, we clear all nodes and edges we have store until that time and throw the appropriate error.

```cpp
void Network::addEdge(std::string networkTopology)
{
    stringstream text(networkTopology);
    std::string intermediate;
    while(getline(text, intermediate, EDGE_DELIM))
    {
        stringstream edge(intermediate);
        std::string vertex;
        std::vector <std::string> edges;
        while(getline(edge, vertex, NODE_DELIM))
        {
            edges.push_back(vertex);
        }
        int v = stoi(edges[0]);
        int u = stoi(edges[1]);
        int w = stoi(edges[2]);
        if (v == u)
        {
            nodes.clear();
            weights.clear();
            throw (SELF_LOOP_CODE);
        }
        nodes.insert(v);
        nodes.insert(u);
        weights[make_pair(v, u)] = w;
        weights[make_pair(u, v)] = w;
    }
}
```

- show  
  By entering this command, the commandHandler calls the show function. This method prints every node and edge (including weight) of the network topology.

```cpp
void Network::show()
{
    if (!nodes.size())
        throw (NO_TOPOLOGY_CODE);
    cout << "  |" ;
    for (auto node: nodes)
    {
        cout << " " << node;
    }
    cout << endl;

    for (int i = 0; i < (nodes.size() + 1) * 5; i++)
        cout << "-";
    cout << endl;

    for (auto v: nodes)
    {
        cout << " " << v << "|";
        for (auto u: nodes)
        {
            auto edge = weights.find(make_pair(v, u));
            int w;
            if(u == v)
                w = 0;
            else if (edge == weights.end())
                w = -1;
            else
                w = edge->second;
            cout << " "<< w ;
        }
        cout << endl;
    }
}
```

- modify  
  By entering this command, the commandHandler calls the modifyEdge function. If the source and the destination node for the edge is the same, it throws self loop error and otherwise it changes the weight of the edge.

```cpp
void Network::modifyEdge(int v, int u, int newWeight)
{
    if (v == u)
        throw (SELF_LOOP_CODE);
    weights[make_pair(u,v)] = newWeight;
    weights[make_pair(v,u)] = newWeight;
}
```

- remove  
  By entering this command, the commandHandler calls the removeEdge function. If the source or the destination node does not exist, it throws node not exist error. If the edge does not exist, it throws edge not exist error and otherwise it erases the edge. Because the edges are two way, we erase both edge with v as the destination and v as the source node.

```cpp
void Network::removeEdge(int v, int u)
{
    bool doesVexist = false;
    bool doesUexist = false;
    for (auto node: nodes)
    {
        if (node == v)
            doesVexist = true;
        if (node == u)
            doesUexist = true;
    }
    if (!doesUexist | !doesVexist)
        throw (NODE_NOT_EXIST_CODE);
    auto edge = weights.find(make_pair(v,u));
    if (edge == weights.end())
        throw (EDGE_NOT_EXIST_CODE);
    weights.erase(make_pair(u,v));
    weights.erase(make_pair(v,u));
}
```

- lsrp  
  By entering this command with arguments, the commandHandler calls the LSRP function. LSRP algorithm makes use of Bellman-Ford algorithm. We make a map of the nodes to their distances from source named dist, a map of the nodes to their state that if they are marked or not named isMarked, a map of the nodes to their previous hop, which is the neighbor that the source use to get to the destination named prevHop. At first we set the dist of each node that has a direct edge to source to the weight of that edge and also set the preHop of that node to source; Otherwise we set dist to infinity and prevHop to -1. We set the isMarked of the source to true and dist to 0. The outer loop is performed V-1 times (V is the number of nodes in the topology). In the outer loop we first find the node that is closest to source and set the isMarked of this node to true. The inner node is performed for each node. We find the edge between these nodes and the node that has the minimum distance to the source. (Mention that this node should be unmarked). if the sum of the distance of that min distance node from source and the weight of that edge is less than the distance of the node, we update its distance and set the prevHop to that neighbor. At last we print the result.

```cpp
void Network::LSRP(int src)
{
    if (!nodes.size())
        throw (NO_TOPOLOGY_CODE);

    map<int, int> dist;
    map<int, bool> isMarked;
    map<int, int> prevHop;

    for (auto node: nodes)
    {
        isMarked[node] = false;
        auto edge = make_pair(src, node);
        if (weights.find(edge) != weights.end())
        {
            dist[node] = weights[edge];
            prevHop[node] = src;
            continue;
        }
        dist[node] = INF;
        prevHop[node] = -1;
    }

    isMarked[src] = true;
    dist[src] = 0;
    vector<vector<int>>  itersInfo;
    for (int itr = 1; itr < nodes.size(); itr++)
    {
        int minDistNode = findMinDistNode(dist, isMarked);
        isMarked[minDistNode] = true;

        vector <int> itrDistances;
        for (auto v: nodes)
            itrDistances.push_back(dist[v]);
        itersInfo.push_back(itrDistances);

        for (auto node: nodes)
        {
            auto edge = weights.find(make_pair(minDistNode, node));
            if (!isMarked[node] && edge != weights.end())
            {
                int w = edge->second;
                if (dist[minDistNode] + w < dist[node])
                {
                    dist[node] = dist[minDistNode] + w;
                    prevHop[node] = minDistNode;
                }
            }
        }
    }
    printLsrpIterationDists(itersInfo);
    printLsrpPaths(src, dist, prevHop);
}
```

We print LSRP distances to each node in each iteration using the function below.

```cpp
void Network::printLsrpIterationDists(const vector<vector<int>> &itersInfo)
{
    for (int i = 0; i < itersInfo.size(); i++)
    {
        cout << "Iter " << to_string(i+1) + ":" << endl;
        cout << "Dest ";
        for (auto node: nodes)
            cout << node << " | ";
        cout << endl;
        cout << "cost ";
        for (int j = 0; j < itersInfo[i].size(); j++)
        {
            if (itersInfo[i][j] == INF)
                cout << -1 << " | ";
            else
                cout << itersInfo[i][j] << " | ";
        }
        cout << "\n---------------------------------------------------------" << endl;
    }
}
```

To print the path that we reach the destination node from source node, we use prevHop map that we made. For each node we set p to that node number and while we don't reach the source node, we add p to the shortest path and set previous to p and also set the p to prevHop of p.

```cpp
void Network::printLsrpPaths(const int src, map<int, int> &dist, map<int, int> &prevHop)
{
    string shortestPath;
    int prev;
    cout << "Path [s] -> [d] | Min-Cost | Shortest-Path\n";
    for (auto node: nodes)
    {
        if (node == src)
            continue;

        string shortestPath = "";
        int p = node;
        while (prevHop[p] != -1)
        {
            shortestPath = "->" +to_string(p) + shortestPath;
            prev = p;
            p = prevHop[p];
        }
        if (dist[node] == INF)
            cout << src << "->" << node << " | " << "INF" << " | " << "-" << endl;
        else
            cout << src << "->" << node << " | "  << dist[node] << " | " << to_string(src) << shortestPath << endl;
    }
}
```

To print LSRP result for all nodes as the source node, the commandHandler calls the below function. The function calls LSRP for each node as the source node.

```cpp
void Network::allSrcLSRP()
{
    for (auto node: nodes)
    {
        cout << "\nLSRP with source: " << node << endl;
        LSRP(node);
    }
}
```

- dvrp  
  By entering this command with arguments, the commandHandler calls the DVRP function. DVRP algorithm makes use of dijkstra algorithm. We make a map of the nodes to their distances from source named dist, and a map of the nodes to their previous hop, which is the neighbor that the source use to get to the destination named prevHop. At first we set the dist of each node to infinity and prevHop to -1. We set the dist of the source node to 0. The outer loop is performed V-1 times (V is the number of nodes in the topology). The inner loop is performed E times (E is the number of edges). For each edge if the source node of the edge's distance is not infinity and the sum of it and the edge's weight is less that the distance of it's neighbor, we set the distance of the neighbor to the sum the edge's weight and the distance of the node; We also set the prevHop of the neighbor's node to node. In the end we check if any edge has the condition mentioned earlier, We notice that the topology has a negative loop because every edge should be checked V-1 times and the Vth time the condition should not be true. we print the result of DVRP with the function below.

```cpp
void Network::DVRP(int src)
{
    if (!nodes.size())
        throw (NO_TOPOLOGY_CODE);

    map<int, int> dist;
    map<int, int> prevHop;
    for (auto node: nodes)
    {
        dist[node] = INF;
        prevHop[node] = -1;
    }
    dist[src] = 0;

    for (int i = 0; i < nodes.size() - 1; i++)
    {
        for (auto edge: weights)
        {
            int v = edge.first.first;
            int u = edge.first.second;
            int w = edge.second;
            if (dist[v] != INF && dist[v] + w < dist[u])
            {
                dist[u] = dist[v] + w;
                prevHop[u] = v;
            }
        }
    }

    for (auto edge: weights)
    {
        int v = edge.first.first;
        int u = edge.first.second;
        int w = edge.second;
        if (dist[v] != INF && dist[v] + w < dist[u])
        {
            throw (NEGATIVE_LOOP_CODE);
        }
    }

    printDVRPres(src, dist, prevHop);
}
```

Like what we did to get the shortest path in LSRP, the below function prints gets the shortest path and print's also the node that helps the source to get to the destination.

```cpp
void Network::printDVRPres(int src, map<int, int> dist, map<int, int> prevHop)
{
    int prev;
    std::cout << "Dest | NextHop | Dist | Shortest-Path" << endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    for (auto node: nodes)
    {
        if (node == src)
        {
            std::cout << node << " | " << src << " | " << dist[node] << " | " << "[" + to_string(src) + "]" << std::endl;
            continue;
        }
        std::string shortestPath = "";
        int p = node;
        while (prevHop[p] != -1)
        {
            shortestPath=" -> "+to_string(p) + shortestPath;
            prev = p;
            p = prevHop[p];
        }
        if (dist[node] == INF)
            std::cout << node << " | " << '-' << " | " << "INF" << " | " << "[ ]" << std::endl;
        else
            std::cout << node << " | " << prev << " | " <<dist[node] << " | " << "[" + to_string(src) << shortestPath + "]" << std::endl;
    }
}
```

To print DVRP result for all nodes as the source node, the commandHandler calls the below function. The function calls DVRP for each node as the source node.

```cpp
void Network::allSrcDVRP()
{
    for (auto node: nodes)
    {
        cout << "\nDVRP with source: " << node << endl;
        DVRP(node);
    }
}
```

## Results

- lsrp

  ![lsrp](/images/LSRP.png "lsrp")

- dvrp

  ![dvrp](/images/DVRP.png "dvrp")

Time complexity of Dijkstra's algorithm is $O(V.E)$ and The time complexity of Bellman-Ford algorithm is $O(V^2)$. As we have 13 vertices and 18 edges, the elapsed time for dvrp should be more and according to the photos the output is expected.

Remove the edge between node 4 and 10, then run the algorithms:

- lsrp

  ![lsrp_remove](/images/LSRP_Remove.png "lsrp_remove")

- dvrp

  ![dvrp_remove](/images/DVRP_Remove.png "dvrp_remove")

Removing an edge shouldn't change the result, because the number of edges still exceeds the number of vertices. According to the photos the output is expected.
