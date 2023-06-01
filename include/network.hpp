#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>
#include <utility>
#include <string>
#include <set>
#include <map>

const char EDGE_DELIM = ' ';
const char NODE_DELIM = '-';

using namespace std;

class Network
{
public:
    Network();
    void addEdge(string networkTopology);
    void show();
    int countDigit(int num);
    // void show();
    // void delete_edge(int v, int u);
    // void modify_edge(int v, int u, int cost);
    // Network(string topology);
    // void link_state(int source);
    // void distance_vector(int source);
private:
    set<int> nodes;
    map<pair<int, int>, int> weights;
};

#endif