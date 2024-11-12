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
struct Command{
    std::string executable;         // first arg to execve()
    std::vector<char*> args;  // second arg to execve() 
    std::string inputFileName; 
    std::string outputFileName; 
    bool pipingNext; 
};

struct CommandBlock {
  std::vector<Command> commands; 
};

using CommandTable = std::vector<CommandBlock>; // a workaround to avoid building AST :)

/*
  At the point of writing this, this shell don't need a separate Parser 
  class because it offers basic scripting feature (no subshell, no 
  command substitution). 

  But I made the choice to write a separate Parser class, to decouple
  parsing from the CommandTable data structure. Future addition of 
  complex parsing would be easier if there is a separate class responsible
  for parsing :) 

  The Parser class acts like the smart control-plane that handles the 
  data stored by the dumb data-plane struct SingleCommand. 
*/
class Parser {
public: 
    Parser();  // constructor 
    ~Parser(); // destructor 

    CommandTable parse(const std::string& bigCommand);  

private: 
    std::string trim(const std::string& command); 
    std::string resolvePath(const std::string& command); 
    std::vector<char*> parseSingleCommand(const std::string& command); 
};

#endif //PARSER_H