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

int main() {
    signal(SIGINT, handleSignal);
    signal(SIGTSTP, handleSignal);


    
    Command commandParser;
    Executor executor;
    History history;
    history.loadHistory();
    const string prompt = "nutshell> ";

    string cmd; 
    string key;

while (true) {
    std::cout << prompt << std::flush;
    cmd.clear();
    key.clear();

    while (true) {
        char c;
        if (read(STDIN_FILENO, &c, 1) == 1) {
            if (c == '\n') { // Enter key
                std::cout << "\n";
                break; // Exit inner loop and process the command
            } else if (c == 127 || c == '\b') { // Backspace key
                if (!cmd.empty()) {
                    cmd.pop_back(); // Remove the last character from cmd
                    history.updateCommandLine(prompt, cmd); // Update the command line
                }
            } else if (c == '\033') { // Detect Escape sequences for arrow keys
                char seq[2];
                if (read(STDIN_FILENO, &seq[0], 1) == 1 && read(STDIN_FILENO, &seq[1], 1) == 1) {
                    if (seq[0] == '[') {
                        if (seq[1] == 'A') { // UP arrow key
                            cmd = history.getHistoryCommand(true); // Get the previous command
                            history.updateCommandLine(prompt, cmd); // Update the command line
                        } else if (seq[1] == 'B') { // DOWN arrow key
                            cmd = history.getHistoryCommand(false); // Get the next command
                            history.updateCommandLine(prompt, cmd); // Update the command line
                        }
                    }
                }
            } else {
                key = std::string(1, c); // Convert the character to string
                cmd += key; // Append key to cmd
                // std::cout << c << std::flush; // Echo the character to the terminal

                // // Debug log for key
                // std::cerr << "Debug: Key = \"" << key << "\"\n";
            }
        }
    }

    if (cmd.empty()) continue; // Ignore empty commands
    if (cmd == "exit") break; // Exit the shell

    // std::cerr << "Debug: Final Command = \"" << cmd << "\"\n";

    history.addToHistory(cmd); // Add the command to history
    ParsedCommand parsedCmd = commandParser.parse(cmd);
    if (!parsedCmd.isEmpty) {
        executor.execute(parsedCmd, childPID); // Execute the parsed command
        history.saveHistory(); // Save history after command execution
    }

    // std::cout << std::flush;
}


return 0;
}