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
    // vector<string> arg_list;

    while(getline(cin,command))
    {
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
            if (command == SHOW_TOPOLOGY)
            {
                network.show();
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
        // catch(const std::exception& e)
        // {
        //     std::cerr << e.what() << '\n';
        // }
        // network.show();
    }
}