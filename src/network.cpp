#include "network.hpp"
#include "errors.hpp"
#include <chrono>
#include <sstream>
#include <iostream>
#include <iomanip>

Network::Network()
{
    // nodes = set<int>();
    // weights = map<pair<int, int>, int>();
}

int getDigitCount(int x)
{
    if (x == 0)
        return 1;
    int res = 0;
    while (x)
    {
        x /= 10;
        res++;
    }
    return res;
}

void Network::addEdge(std::string networkTopology)
{
    stringstream text(networkTopology);
    std::string intermediate;
    while (getline(text, intermediate, EDGE_DELIM))
    {
        stringstream edge(intermediate);
        std::string vertex;
        std::vector<std::string> edges;
        while (getline(edge, vertex, NODE_DELIM))
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
            throw(SELF_LOOP_CODE);
        }
        nodes.insert(v);
        nodes.insert(u);
        weights[make_pair(v, u)] = w;
        weights[make_pair(u, v)] = w;
    }
}

void Network::show()
{
    if (!nodes.size())
        throw(NO_TOPOLOGY_CODE);
    std::cout << "  |";
    for (auto node : nodes)
    {
        std::cout << " " << node;
    }
    std::cout << std::endl;

    for (int i = 0; i < (nodes.size() + 1) * 5; i++)
        std::cout << "-";
    std::cout << endl;

    for (auto v : nodes)
    {
        std::cout << " " << v << "|";
        for (auto u : nodes)
        {
            auto edge = weights.find(make_pair(v, u));
            int w;
            if (u == v)
                w = 0;
            else if (edge == weights.end())
                w = -1;
            else
                w = edge->second;
            std::cout << " " << w;
        }
        std::cout << endl;
    }
}

void Network::printDVRPres(int src, map<int, int> dist, map<int, int> prevHop)
{
    int prev;
    std::cout << "Dest | NextHop | Dist | Shortest-Path" << endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    for (auto node : nodes)
    {

        if (node == src)
        {
            std::cout << std::left << setw(5) << node << "|" << setw(9) << src << "|" << setw(6) << dist[node] << "|"
                      << "[" + to_string(src) + "]" << std::endl;
            continue;
        }
        std::string shortestPath = "";
        int p = node;
        while (prevHop[p] != -1)
        {
            shortestPath = " -> " + to_string(p) + shortestPath;
            prev = p;
            p = prevHop[p];
        }
        if (dist[node] == INF)
            std::cout << setw(5) << node << "|" << '-' << "|"
                      << "INF"
                      << " | "
                      << "[ ]" << std::endl;
        else
            std::cout << setw(5) << node << "|" << setw(9) << prev << "|" << setw(6) << dist[node] << "|"
                      << "[" + to_string(src) << shortestPath + "]" << std::endl;
    }
}

void Network::DVRP(int src)
{
    auto start = std::chrono::steady_clock::now();

    if (!nodes.size())
        throw(NO_TOPOLOGY_CODE);

    map<int, int> dist;
    map<int, int> prevHop;
    for (auto node : nodes)
    {
        dist[node] = INF;
        prevHop[node] = -1;
    }
    dist[src] = 0;

    for (int i = 0; i < nodes.size() - 1; i++)
    {
        for (auto edge : weights)
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

    for (auto edge : weights)
    {
        int v = edge.first.first;
        int u = edge.first.second;
        int w = edge.second;
        if (dist[v] != INF && dist[v] + w < dist[u])
        {
            throw(NEGATIVE_LOOP_CODE);
        }
    }

    printDVRPres(src, dist, prevHop);
    auto finish = std::chrono::steady_clock::now();
    double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
    cout << "Elapsed: " << elapsed_seconds * 1000000 << endl;
}

void Network::allSrcDVRP()
{
    for (auto node : nodes)
    {
        cout << "\nDVRP with source: " << node << endl;
        DVRP(node);
    }
}

void Network::allSrcLSRP()
{
    for (auto node : nodes)
    {
        cout << "\nLSRP with source: " << node << endl;
        LSRP(node);
    }
}

int Network::findMinDistNode(map<int, int> &dist, map<int, bool> &isMarked)
{
    int minDist = INF;
    int minDistNode;
    for (auto node : nodes)
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
        cout << "Iter " << to_string(i + 1) + ":" << endl;
        cout << "Dest ";
        for (auto node : nodes)
            cout << std::left << setw(3) << node << "|";
        cout << endl;
        cout << "cost ";
        for (int j = 0; j < itersInfo[i].size(); j++)
        {
            if (itersInfo[i][j] == INF)
                cout << setw(3) << -1 << "|";
            else
                cout << setw(3) << itersInfo[i][j] << "|";
        }
        cout << "\n---------------------------------------------------------" << endl;
    }
}

void Network::printLsrpPaths(const int src, map<int, int> &dist, map<int, int> &prevHop)
{
    string shortestPath;
    int prev;
    cout << "Path [s] -> [d] | Min-Cost | Shortest-Path\n";
    for (auto node : nodes)
    {
        if (node == src)
            continue;

        string shortestPath = "";
        int p = node;
        while (prevHop[p] != -1)
        {
            shortestPath = "->" + to_string(p) + shortestPath;
            prev = p;
            p = prevHop[p];
        }
        if (dist[node] == INF)
            cout << std::left << setw(13) << src << "->" << node << "|"
                 << "INF"
                 << " | "
                 << "-" << endl;
        else
            cout << src << "->" << setw(13) << node << "| " << setw(9) << dist[node] << "| " << to_string(src) << setw(14) << shortestPath << endl;
    }
}

void Network::LSRP(int src)
{
    auto start = std::chrono::steady_clock::now();

    if (!nodes.size())
        throw(NO_TOPOLOGY_CODE);

    map<int, int> dist;
    map<int, bool> isMarked;
    map<int, int> prevHop;

    for (auto node : nodes)
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
    vector<vector<int>> itersInfo;
    for (int itr = 1; itr < nodes.size(); itr++)
    {
        int minDistNode = findMinDistNode(dist, isMarked);
        isMarked[minDistNode] = true;

        vector<int> itrDistances;
        for (auto v : nodes)
            itrDistances.push_back(dist[v]);
        itersInfo.push_back(itrDistances);

        for (auto node : nodes)
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
    auto finish = std::chrono::steady_clock::now();
    double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
    cout << "Elapsed: " << elapsed_seconds * 1000000 << endl;
}

void Network::removeEdge(int v, int u)
{
    bool doesVexist = false;
    bool doesUexist = false;
    for (auto node : nodes)
    {
        if (node == v)
            doesVexist = true;
        if (node == u)
            doesUexist = true;
    }
    if (!doesUexist | !doesVexist)
        throw(NODE_NOT_EXIST_CODE);
    auto edge = weights.find(make_pair(v, u));
    if (edge == weights.end())
        throw(EDGE_NOT_EXIST_CODE);
    weights.erase(make_pair(u, v));
    weights.erase(make_pair(v, u));
}

void Network::modifyEdge(int v, int u, int newWeight)
{
    if (v == u)
        throw(SELF_LOOP_CODE);
    weights[make_pair(u, v)] = newWeight;
    weights[make_pair(v, u)] = newWeight;
}