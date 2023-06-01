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
    void allDVRP();
private:
    set<int> nodes;
    map<pair<int, int>, int> weights;
    void printDVRPres(int src, map<int, int> dist, map<int, int> nextHopFromSrc);
};

#endif