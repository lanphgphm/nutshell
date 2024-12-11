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
#include <string>
#include "command.h"
#include "history.h"

class Executor {
  public:
    Executor();
    ~Executor();

    int statCmd1, statCmd2 = 0; // sometimes statCmd2 unused :)

    void execute(const ParsedCommand &cmd, pid_t &child_pid);
    void executeSingle(std::string cmd, std::string executable1, std::vector<char *> args, pid_t &childPID, int &status);
    void debug();

    // for handling stopped jobs (SIGTSTP)
    void addStoppedJob(pid_t pid);
    std::vector<pid_t> getStoppedJob();
    void removeStoppedJob(pid_t pid);
    int getStoppedJobsSize();

  private:
    static const std::unordered_map<int, std::string> signalMessages;
    std::vector<pid_t> stoppedJobs;
    
    void executePiped(const ParsedCommand &cmd, int &statCmd1, int &statCmd2);
    void executeAndOr(const ParsedCommand &cmd, int &statCmd1, int &statCmd2, pid_t &child_pid);
    void printError(int status, const std::string &command);
    void ignoreTTOU(bool ignore); 
};

#endif // EXECUTOR_H