#ifndef EXECUTOR_H
#define EXECUTOR_H

#include<iostream> // cin, cout
#include<unistd.h> // fork, read, write, exec, dup, access, dup2, close, 3 fds
#include<cstdlib> // getenv
#include<stdlib.h> // exit
#include<sys/types.h> // pid_t, ssize_t
#include<sstream> // istringstream
#include<string> // string
#include<vector> // vector
#include<sys/wait.h> // waitpid
#include<algorithm> // find_if
#include<cctype> // isspace

#include"command.h" 

class Executor {
public: 
    Executor(); 
    ~Executor(); 

    int statCmd1, statCmd2 = 69; // sometimes statCmd2 unused :) 

    void execute(const ParsedCommand& cmd); 
    void debug(); 
private: 
    void executePiped(const ParsedCommand& cmd, int& statCmd1, int& statCmd2); 
    void executeAndOr(const ParsedCommand& cmd, int& statCmd1, int& statCmd2); 
    void printError(const std::string& cmd, int status);
}; 

#endif //EXECUTOR_H