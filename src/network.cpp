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



