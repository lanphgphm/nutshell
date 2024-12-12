#include "src/command.h"
#include "src/executor.h"
#include "src/history.h"

#include <csignal>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <termios.h>

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
                    history.updateCommandLine(prompt, cmd);                }
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
                continue; 
            } else {
                cmd += c;
        
            }
        }
    }
    return cmd;
}


int main() {

    Command commandParser;
    Executor executor;
    History history;

    signal(SIGINT, handleSignal);
    signal(SIGTSTP, handleSignal);

    history.loadHistory();

    const std::string prompt = "nutshell> ";
    std::string cmd;

    while (true) {
        std::cout << prompt << std::flush;
        cmd = readCommandLine(prompt, history);
        if (cmd.empty()) continue; 
        if (cmd == "exit") break;  

        history.addToHistory(cmd);

        if (cmd.compare("steve") == 0) {
            if (executor.getStoppedJobsSize() > 0) {

                std::vector<pid_t> stoppedJobs = executor.getStoppedJob();
                for (int i = 0; i < stoppedJobs.size(); i++) {
                    if (i == stoppedJobs.size() - 1) {
                        cout << "[" << i + 1 << "]+ Stopped process " << stoppedJobs[i] << "\n";
                        break;
                    } else {
                        cout << "[" << i + 1 << "]- Stopped process " << stoppedJobs[i] << "\n";
                    }
                }
            } else {
                cout << "No stopped jobs\n";
            }

            continue;
        }

        ParsedCommand parsedCmd = commandParser.parse(cmd);
        if (!parsedCmd.isEmpty) {
            cout << "\n"; 
            executor.execute(parsedCmd, childPID);
            history.saveHistory();
            history.resetHistoryIterator();
        }
    }
    return 0;
}