// responsible for: handling interrupt signals, setting envvars, changing
// prompts between users the important signals: SIGINT, SIGSTP, SIGCHLD,
// SIGTERM, SIGKILL

#include "src/command.h"
#include "src/executor.h"
#include <csignal>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

pid_t childPID = -1;

void handleSignal(int signal) {
    if (childPID > 0) {
        switch (signal) {
        case SIGINT:
            cout << "\nCTRL+C pressed\n";
            kill(childPID, signal);
            break;
        case SIGTSTP:
            cout << "\nCTRL+Z pressed\n";
            kill(childPID, signal);
            break;
        default:
            cout << "\nSignal not implemented: " << signal << "\n";
            break;
        }
    }
}


int main() {

    Command commandParser;
    Executor executor;

    signal(SIGINT, handleSignal);
    signal(SIGTSTP, handleSignal);


    while (true) {
        cout << "nutshell> ";

        string cmd;
        getline(std::cin, cmd);

        if (cmd.compare("exit") == 0)
            break;

        ParsedCommand parsedCmd = commandParser.parse(cmd);
        // commandParser.debug(parsedCmd);
        if (!parsedCmd.isEmpty) {
            executor.execute(parsedCmd, childPID);
            // executor.debug();
        }
    }

    return 0;
}
