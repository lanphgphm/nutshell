#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "command.h"
#include <algorithm> // find_if
#include <cctype>    // isspace
#include <cstdlib>   // getenv
#include <iostream>  // cin, cout
#include <map>
#include <signal.h>
#include <sstream>  // istringstream
#include <stdlib.h> // exit
#include <string>   // string
#include <string>
#include <sys/types.h> // pid_t, ssize_t
#include <sys/wait.h>  // waitpid
#include <unistd.h>    // fork, read, write, exec, dup, access, dup2, close, 3 fds
#include <unordered_map>
#include <vector> // vector

class Executor {
  public:
    Executor();
    ~Executor();

    int statCmd1, statCmd2 = 0; // sometimes statCmd2 unused :)

    void execute(const ParsedCommand &cmd, pid_t &child_pid,int shell_terminal, pid_t &nutshell_pgid);
    void debug();

    // for handling stopped jobs (SIGTSTP)
    void addStoppedJob(pid_t pid);
    pid_t getLastStoppedJob();
    void removeStoppedJob(pid_t pid);
    int getStoppedJobsSize();

  private:
    void executePiped(const ParsedCommand &cmd, int &statCmd1, int &statCmd2, pid_t &child_pid,int shell_terminal, pid_t &nutshell_pgid);
    void executeAndOr(const ParsedCommand &cmd, int &statCmd1, int &statCmd2, pid_t &child_pid,int shell_terminal, pid_t &nutshell_pgid);
    void printError(int status, const std::string &command);
    static const std::unordered_map<int, std::string> signalMessages;

    std::vector<pid_t> stoppedJobs;
};

#endif // EXECUTOR_H