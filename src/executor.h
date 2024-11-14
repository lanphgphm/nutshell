#ifndef EXECUTOR_H
#define EXECUTOR_H

#include<iostream> // cin, cout
#include<unistd.h> // fork, read, write, exec, dup, access, dup2, close
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

    void execute(const ParsedCommand& cmd); 

private: 
    void executePiped(const ParsedCommand& cmd); 
    void executeAndOr(const ParsedCommand& cmd); 
    
    void printError(); // change me 
}; 

#endif //EXECUTOR_H