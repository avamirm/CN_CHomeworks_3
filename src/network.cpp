#include "network.hpp"
#include "errors.hpp"

#include <sstream>
#include <iostream>

Network::Network()
{
    // nodes = set<int>();
    // weights = map<pair<int, int>, int>();
}

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
            // throw std::invalid_argument("Self loop detected");
            throw (SELF_LOOP_CODE);
        }
        nodes.insert(v);
        nodes.insert(u);
        weights[make_pair(v, u)] = w;
        weights[make_pair(u, v)] = w;
    }
}

int Network::countDigit(int number)
{
   int count = 0;
   while(number != 0)
   {
      number = number / 10;
      count++;
   }
   return count;
}

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

void Network::printDVRPres(int src, map<int, int> dist, map<int, int> nextHopFromSrc)
{
    int prev_p;
    cout << "Dest | NextHop | Dist | Shortest-Path" << endl;
    cout << "---------------------------------------------------------" << endl;
    for (auto node: nodes)
    {
        if (node == src)
        {
            cout << node << " | " << src << " | " << dist[node] << " | " << "[" + to_string(src) + "]" << endl;
            continue;
        }
        string shortestPath = "";
        int p = node;
        while (nextHopFromSrc[p] != -1)
        {
            shortestPath=" -> "+to_string(p) + shortestPath;
            prev_p = p;
            p = nextHopFromSrc[p];
        }
        if (dist[node] == INF)
            cout << node << " | " << '-' << " | " << "INF" << " | " << "[ ]" << endl;
        else
            cout << node << " | " << prev_p << " | " <<dist[node] << " | " << "[" + to_string(src) << shortestPath + "]" << endl;
    }
}

void Network::DVRP(int src)
{
    if (!nodes.size())
        throw (NO_TOPOLOGY_CODE);

    map<int, int> dist;
    map<int, int> nextHopFromSrc;
    map <int, bool> isMarked;
    for (auto node: nodes)
    {
        dist[node] = INF;
        nextHopFromSrc[node] = -1;
        isMarked[node] = false;
    }
    set <pair<int, int>> unvisited;
    dist[src] = 0;
    unvisited.insert(make_pair(dist[src], src));
    while (!unvisited.empty())
    {
        int v = unvisited.begin()->second;
        unvisited.erase(unvisited.begin());
        isMarked[v] = true;
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
            if (w == -1)
                continue;
            if (dist[v] + w < dist[u])
            {
                unvisited.erase(make_pair(dist[u], u));
                dist[u] = dist[v] + w;
                nextHopFromSrc[u] = v;
                unvisited.insert(make_pair(dist[u], u));
            }
        }
    }
    printDVRPres(src, dist, nextHopFromSrc);
}

void Network::allDVRP()
{
    for (auto node: nodes)
    {
        cout << "\nDVRP with source: " << node << endl;
        DVRP(node);
    }
}