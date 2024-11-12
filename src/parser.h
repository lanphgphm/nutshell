#ifndef PARSER_H
#define PARSER_H

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

/*
  This is a struct and not a class because it is intended to only hold 
  data and perform no operation. It is meant to be a dumb data plane 
  whose members are all public. 
*/
struct SingleCommand{
    std::string executable;         // first arg to execve()
    std::vector<std::string> args;  // second arg to execve() 
    std::string inputFile; 
    std::string outputFile; 
    bool pipingNext; 
};

using CommandTable = std::vector<SingleCommand>; // a workaround to avoid building AST :)

/*
  At the point of writing this, this shell don't need a separate Parser 
  class because it offers basic scripting feature (no subshell, no 
  command substitution). 

  But I made the choice to write a separate Parser class, to decouple
  parsing from the CommandTable data structure. Future addition of 
  complex parsing would be easier if there is a separate class responsible
  for parsing :) 
*/
class Parser {
public: 
    Parser();  // constructor 
    ~Parser(); // destructor 

    CommandTable parse(const std::string& bigcmd);  

private: 
    std::string trimSingleCmd(const std::string& cmd); 
    std::string resolvePath(const std::string& cmd); 
    std::vector<char*> parseSingleCmd(const std::string& cmd); 
};

#endif //PARSER_H