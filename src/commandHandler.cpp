#include "commandHandler.hpp"
#include "errors.hpp"

#include <iostream>
#include <sstream>

CommandHandler::CommandHandler()
{
    // network = Network();
}

vector<string> CommandHandler::tokenizeWithDelim(string cmdArgs)
{
    vector<string> tokens;
    stringstream argStream(cmdArgs);
    string tok;
    while (getline(argStream, tok, NODE_DELIM))
        tokens.push_back(tok);
    return tokens;
}

void CommandHandler::run()
{
    network = Network();

    string command, cmdArgs;
    stringstream commandStream;

    while (getline(cin, command))
    {
        cmdArgs = "";
        commandStream = stringstream(command);
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
                    network.allSrcDVRP();
                else
                {
                    int src = stoi(cmdArgs);
                    network.DVRP(src);
                }
            }
            else if (command == LSRP)
            {
                if (!cmdArgs.size())
                    network.allSrcLSRP();
                else
                {
                    int src = stoi(cmdArgs);
                    network.LSRP(src);
                }
            }
            else if (command == REMOVE)
            {
                vector<string> tokens = tokenizeWithDelim(cmdArgs);
                network.removeEdge(stoi(tokens[0]), stoi(tokens[1]));
                cout << SUCCESS << endl;
            }
            else if (command == MODIFY)
            {
                vector<string> tokens = tokenizeWithDelim(cmdArgs);
                network.modifyEdge(stoi(tokens[0]), stoi(tokens[1]), stoi(tokens[2]));
                cout << SUCCESS << endl;
            }
            else
                std::cout << COMMAND_NOT_EXIST << std::endl;
        }
        catch (int errCode)
        {
            switch (errCode)
            {
            case NO_TOPOLOGY_CODE:
                std::cout << NO_TOPOLOGY << std::endl;
                break;
            case SELF_LOOP_CODE:
                std::cout << SELF_LOOP << std::endl;
                break;
            case NEGATIVE_LOOP_CODE:
                std::cout << NEGATIVE_LOOP << std::endl;
                break;
            case NODE_NOT_EXIST_CODE:
                std::cout << NODE_NOT_EXIST << std::endl;
                break;
            case EDGE_NOT_EXIST_CODE:
                std::cout << EDGE_NOT_EXIST << std::endl;
                break;
            default:
                break;
            }
        }
    }
}