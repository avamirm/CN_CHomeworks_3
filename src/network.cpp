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
    for (auto node: nodes)
    {
        dist[node] = INF;
        nextHopFromSrc[node] = -1;
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
                nextHopFromSrc[u] = v;
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

    printDVRPres(src, dist, nextHopFromSrc);
}

void Network::allSrcDVRP()
{
    for (auto node: nodes)
    {
        cout << "\nDVRP with source: " << node << endl;
        DVRP(node);
    }
}

void Network::allSrcLSRP()
{
    for (auto node: nodes)
    {
        cout << "\nLSRP with source: " << node << endl;
        LSRP(node);
    }
}

int Network::findMinDistNode(map<int, int> &dist, map <int, bool> &isMarked)
{
    int minDist = INF;
    int minDistNode;
    for (auto node: nodes)
    {
        if (!isMarked[node] && dist[node] < minDist)
        {
            minDist = dist[node];
            minDistNode = node;
        }
   }
   return minDistNode;
}

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

void Network::printLsrpPaths(const int src, map<int, int> &dist, map<int, int> &nextHop)
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
        while (nextHop[p] != -1)
        {
            shortestPath = "->" +to_string(p) + shortestPath;
            prev = p;
            p = nextHop[p];
        }
        if (dist[node] == INF)
            cout << src << "->" << node << " | " << "INF" << " | " << "-" << endl;
        else
            cout << src << "->" << node << " | "  << dist[node] << " | " << to_string(src) << shortestPath << endl;
    }
}

void Network::LSRP(int src)
{
    if (!nodes.size())
        throw (NO_TOPOLOGY_CODE);

    map<int, int> dist;
    map<int, bool> isMarked;
    map<int, int> nextHop;

    for (auto node: nodes)
    {
        isMarked[node] = false;
        auto edge = make_pair(src, node);
        if (weights.find(edge) != weights.end())
        {
            dist[node] = weights[edge];
            nextHop[node] = src;
            continue;
        }
        dist[node] = INF;
        nextHop[node] = -1;
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
                    nextHop[node] = minDistNode;
                }
            }
        }
    }
    printLsrpIterationDists(itersInfo);
    printLsrpPaths(src, dist, nextHop);
}

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

void Network::modifyEdge(int v, int u, int newWeight)
{
    if (v == u)
        throw (SELF_LOOP_CODE);
    weights[make_pair(u,v)] = newWeight;
    weights[make_pair(v,u)] = newWeight;
}