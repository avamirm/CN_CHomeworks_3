#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <string>

const std::string SELF_LOOP = "Self loop detected";
const std::string NO_TOPOLOGY = "No topology";
const std::string NEGATIVE_LOOP = "Graph contains negative loop cycle";
const std::string NODE_NOT_EXIST = "Node does not exist";
const std::string EDGE_NOT_EXIST = "Edge does not exist";
const std::string COMMAND_NOT_EXIST = "Command does not exist";

const int SELF_LOOP_CODE = 101;
const int NO_TOPOLOGY_CODE = 102;
const int NEGATIVE_LOOP_CODE = 103;
const int NODE_NOT_EXIST_CODE = 104;
const int EDGE_NOT_EXIST_CODE = 105;

#endif