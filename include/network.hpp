#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>
#include <utility>
#include <string>
#include <set>
#include <map>

const char EDGE_DELIM = ' ';
const char NODE_DELIM = '-';
const int INF = 2147483647;

using namespace std;

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

#endif