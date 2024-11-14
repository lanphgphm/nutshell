// responsible for: handling interrupt signals, setting envvars, changing prompts between users 
// the important signals: SIGINT, SIGSTP, SIGCHLD, SIGTERM, SIGKILL
// the important envvars: PATH, SHELL, USER, PS1
// changing prompts: whenever switch user, change username associated with prompt using format user@nutshell > 

#include <iostream>
#include <string>
#include "src/command.h"
#include "src/executor.h"

using namespace std; 

int main() {
    Command commandParser;

    while (true) {
        cout << "nutshell> ";

        string cmd;
        getline(std::cin, cmd);

        if (cmd.compare("exit") == 0)
            break;

        ParsedCommand parsedCmd = commandParser.parse(cmd);
        // commandParser.debug(parsedCmd);
    }

    return 0;
}
