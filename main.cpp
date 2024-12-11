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

pid_t nutshell_pgid;
struct termios shell_tmodes;
int shell_terminal;
int shell_is_interactive;

pid_t childPID = -1;

void handleSignal(int signal) {
    if (childPID > 0) {
        switch (signal) {
        case SIGINT:
            cout << "CTRL+C pressed\n";
            kill(childPID, signal);
            break;
        case SIGTSTP:
            cout << "CTRL+Z pressed\n";
            kill(childPID, signal);
            break;
        default:
            cout << "Signal not implemented: " << signal << "\n";
            break;
        }
    }
}

void init_shell() {

    /* See if we are running interactively.  */
    shell_terminal = STDIN_FILENO;
    shell_is_interactive = isatty(shell_terminal);

    if (shell_is_interactive) {
        /* Loop until we are in the foreground.  */
        while (tcgetpgrp(shell_terminal) != (nutshell_pgid = getpgrp()))
            kill(-nutshell_pgid, SIGTTIN);

        /* Ignore interactive and job-control signals.  */
        signal(SIGINT, handleSignal);
        signal(SIGTSTP, handleSignal);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        signal(SIGCHLD, SIG_IGN);

        /* Put ourselves in our own process group.  */
        nutshell_pgid = getpid();
        if (setpgid(nutshell_pgid, nutshell_pgid) < 0) {
            perror("Couldn't put the shell in its own process group");
            exit(1);
        }

        /* Grab control of the terminal.  */
        tcsetpgrp(shell_terminal, nutshell_pgid);

        /* Save default terminal attributes for shell.  */
        tcgetattr(shell_terminal, &shell_tmodes);
    }
}

int main() {

    Command commandParser;
    Executor executor;

    init_shell();

    while (true) {
        cout << "nutshell> ";

        string cmd;
        getline(std::cin, cmd);

        if (cmd.compare("exit") == 0)
            break;

        ParsedCommand parsedCmd = commandParser.parse(cmd);
        // commandParser.debug(parsedCmd);
        if (!parsedCmd.isEmpty) {
            executor.execute(parsedCmd, childPID, shell_terminal, nutshell_pgid);
            // executor.debug();
        }
    }

    return 0;
}
