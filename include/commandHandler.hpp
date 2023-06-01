#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include "network.hpp"

const char COMMAND_DELIM = ' ';
const string DETERMINE_TOPOLOGY = "topology";
const string SHOW_TOPOLOGY = "show";
const string SUCCESS = "OK";

class CommandHandler
{
public:
    CommandHandler();
    // void handleCommand(std::string command);
    void run();
private:
    Network network;
};

#endif