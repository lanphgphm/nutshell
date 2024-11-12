// input of parser: a big command string 
// output: command table 
// responsible for: ||, &&, >, < 

#include"parser.h"

using namespace std; 

Parser::Parser(){}

Parser::~Parser(){}

string Parser::trim(const string& command){
    auto validStart = find_if(command.begin(), command.end(), [](unsigned char c){
        return !isspace(c); 
    });

    auto validEnd = find_if(command.rbegin(), command.rend(), [](unsigned char c){
        return !isspace(c); 
    }).base();

    return (validStart < validEnd) 
            ? string(validStart, validEnd)
            : ""; 
}

string Parser::resolvePath(const string& command){
    if (command.find('/') != string::npos) {
        return command;
    }

    char* pathEnvVar = getenv("PATH"); 
    if (!pathEnvVar) return command; 

    istringstream paths(pathEnvVar); 
    string path; 

    while (getline(paths, path, ':')){
        string fullPath = path + "/" + command; 
        if (access(fullPath.c_str(), X_OK) == 0){
            return fullPath; 
        }
    }
    return command; 
}

vector<char*> Parser::parseSingleCommand(const string& command){
    istringstream iss(command); 
    vector<string> tokens; 
    string token; 

    while (iss >> token){ 
        tokens.push_back(token); 
    }

    vector<char*> args; 
    for (auto& t: tokens) {
        args.push_back(&t[0]); 
    }
    args.push_back(nullptr); 

    return args;
}

CommandTable Parser::parse(const string& bigCommand){

    return {(CommandBlock){}}; // placeholder
}