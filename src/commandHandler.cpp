#include "commandHandler.hpp"
#include "errors.hpp"

#include <iostream>
#include <sstream>

CommandHandler::CommandHandler()
{
    // network = Network();
}

void CommandHandler::run()
{
    network = Network();

    string command, cmdArgs;
    stringstream commandStream;

    while(getline(cin,command))
    {
        cmdArgs = "";
        commandStream=stringstream(command);
        getline(commandStream, command, COMMAND_DELIM);
        getline(commandStream, cmdArgs);
        try
        {
            if (command == DETERMINE_TOPOLOGY)
            {
                network.addEdge(cmdArgs);
                cout << SUCCESS << endl;
            }
            else if (command == SHOW_TOPOLOGY)
            {
                network.show();
            }
            else if (command == DVRP)
            {
                if (!cmdArgs.size())
                    network.allDVRP();
                else
                {
                    int src = stoi(cmdArgs);
                    network.DVRP(src);
                }
            }
        }
        catch (int errCode)
        {
            switch (errCode) {
            case NO_TOPOLOGY_CODE:
                std::cout << NO_TOPOLOGY << std::endl;
                break;
            case SELF_LOOP_CODE:
                std::cout << SELF_LOOP << std::endl;
                break;
            default:
                break;
            }
        }
    }
}