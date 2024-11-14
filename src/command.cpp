#include"command.h"

using namespace std; 

Command::Command(){}

Command::~Command(){}

ParsedCommand Command::parse(const std::string& command){}

string Command::trim(const std::string& str){}

string resolvePath(const std::string& command){}

vector<char*> getArgsVector(const std::string& command){}