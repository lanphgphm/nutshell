// responsible for: handling interrupt signals, setting envvars, changing prompts between users 
// the important signals: SIGINT, SIGSTP, SIGCHLD, SIGTERM, SIGKILL

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
        if (!parsedCmd.isEmpty) {
            Executor executor; 
            executor.execute(parsedCmd); 
            // executor.debug(); 
        }
    }

    return 0;
}
