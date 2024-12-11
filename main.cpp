// responsible for: handling interrupt signals, setting envvars, changing
// prompts between users the important signals: SIGINT, SIGSTP, SIGCHLD,
// SIGTERM, SIGKILL

#include "src/command.h"
#include "src/executor.h"
#include "src/history.h"

#include <csignal>
#include <iostream>
#include <string>
#include <unistd.h>
#include <termios.h>

using namespace std;

pid_t childPID = -1;

void handleSignal(int signal) {
    if (childPID > 0) {
        switch (signal) {
        case SIGINT:
            cout << "\n";
            kill(childPID, signal);
            break;
        case SIGTSTP:
            cout << "\n";
            kill(childPID, signal);
            break;
        default:
            break;
        }
    }
}

std::string readCommandLine(const std::string &prompt, History &history) {
    std::string cmd;
    char c;
    const int BACKSPACE = 127;

    while (true) {
        if (read(STDIN_FILENO, &c, 1) == 1) {
            if (c == '\n') {
                break;
            } else if (c == BACKSPACE || c == '\b') {
                if (!cmd.empty()) {
                    cmd.pop_back();
                    history.updateCommandLine(prompt, cmd);
                }
            } else if (c == '\033') {
                char seq[2];
                if (read(STDIN_FILENO, &seq[0], 1) == 1 && read(STDIN_FILENO, &seq[1], 1) == 1) {
                    if (seq[0] == '[') {
                        if (seq[1] == 'A') {
                            cmd = history.getHistoryCommand(true);
                            history.updateCommandLine(prompt, cmd);
                        } else if (seq[1] == 'B') {
                            cmd = history.getHistoryCommand(false);
                            history.updateCommandLine(prompt, cmd);
                        }
                    }
                }
            } else {
                cmd += c;
            }
        }
    }
    return cmd;
}




int main() {
    signal(SIGINT, handleSignal);
    signal(SIGTSTP, handleSignal);

    Command commandParser;
    Executor executor;
    History history;

    history.loadHistory();

    const std::string prompt = "nutshell> ";
    std::string cmd;

    while (true) {
        std::cout << prompt << std::flush;
        cmd = readCommandLine(prompt, history); // Call static method directly
        if (cmd.empty()) continue; // Ignore empty commands
        if (cmd == "exit") break;  // Exit the shell

        history.addToHistory(cmd);

        ParsedCommand parsedCmd = commandParser.parse(cmd);
        if (!parsedCmd.isEmpty) {
            executor.execute(parsedCmd, childPID);
            history.saveHistory();
            history.resetHistoryIterator();
        }
    }
    return 0;
}