#ifndef COMMAND_H
#define COMMAND_H

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
#include<assert.h> // assert
#include<history.h>

struct ParsedCommand{
    bool isEmpty = false; 

    bool isPiping = false; 
    bool isAnd = false; 
    bool isOr = false; 

    std::string command1; 
    std::string executable1; 
    std::vector<char*> args1; 

    std::string command2; 
    std::string executable2; 
    std::vector<char*> args2; 
};

class Command {
public: 
    Command(); 
    ~Command(); 

    ParsedCommand parse(const std::string& command); 
    void debug(const ParsedCommand& parsedCmd);
private:  
    std::string trim(const std::string& command); 
    std::string resolvePath(const std::string& arg0); 
    std::vector<char*> getArgsVector(const std::string& command); 
    void displayUsage(); 
};

#endif //COMMAND_H