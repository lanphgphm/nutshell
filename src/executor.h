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

class Executor {
public: 
    Executor(); 
    ~Executor(); 

    void execute(const CommandTable& commandTable); 

private: 
    int fuckaroundandfindout; 
}; 

#endif //EXECUTOR_H