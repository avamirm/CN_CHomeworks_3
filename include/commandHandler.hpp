#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include "network.hpp"

const char COMMAND_DELIM = ' ';
const string DETERMINE_TOPOLOGY = "topology";
const string SHOW_TOPOLOGY = "show";
const string SUCCESS = "OK";
const string DVRP = "dvrp";
const string LSRP = "lsrp";
const string REMOVE = "remove";
const string MODIFY = "modify";

class CommandHandler
{
public:
    CommandHandler();
    vector <string> tokenizeWithDelim(string cmdArgs);
    void run();
private:
    Network network;
};

#endif