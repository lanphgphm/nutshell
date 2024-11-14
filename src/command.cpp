#include"command.h"

using namespace std; 

Command::Command(){}

Command::~Command(){}

ParsedCommand Command::parse(const string& command){
    ParsedCommand parsedCmd;

    string trimmedCmd = trim(command);

    if (trimmedCmd.empty()){
        cout << "Command must not be empty\n"; 
        displayUsage(); 
        return parsedCmd; 
    }

    if (trimmedCmd.back() == '&') {
        parsedCmd.inBackground = true;
        trimmedCmd.pop_back();  
        trimmedCmd = trim(trimmedCmd); 
    } else {
        parsedCmd.inBackground = false;
    }

    // PIPING
    size_t pipePos = trimmedCmd.find('|');
    if (pipePos != string::npos) {
        parsedCmd.isPiping = true;
        parsedCmd.command1 = trimmedCmd.substr(0, pipePos);
        parsedCmd.command2 = trimmedCmd.substr(pipePos + 1);
    } else {
        parsedCmd.isPiping = false;
    }

    // AND
    size_t andPos = trimmedCmd.find("&&");
    
    if (andPos != string::npos) {
        parsedCmd.isAnd = true;
        parsedCmd.command1 = trimmedCmd.substr(0, andPos);
        parsedCmd.command2 = trimmedCmd.substr(andPos + 2);
    } else {
        parsedCmd.isAnd = false;
    }
    
    // OR
    size_t orPos = trimmedCmd.find("||");
    if (orPos != string::npos) {
        parsedCmd.isOr = true;
        parsedCmd.command1 = trimmedCmd.substr(0, orPos);
        parsedCmd.command2 = trimmedCmd.substr(orPos + 2);
    } else {
        parsedCmd.isOr = false;
    }

    if (parsedCmd.isOr + parsedCmd.isAnd + parsedCmd.isPiping == 0){
        parsedCmd.command1 = trimmedCmd; 
    }

    parsedCmd.command1 = trim(parsedCmd.command1);
    parsedCmd.command2 = trim(parsedCmd.command2);

    parsedCmd.executable1 = resolvePath(parsedCmd.command1);
    parsedCmd.args1 = getArgsVector(parsedCmd.command1);

    if (!parsedCmd.command2.empty()) {
        parsedCmd.executable2 = resolvePath(parsedCmd.command2);
        parsedCmd.args2 = getArgsVector(parsedCmd.command2);
    }

    if (parsedCmd.isOr + parsedCmd.isAnd + parsedCmd.isPiping > 1){
        displayUsage(); 
    }

    return parsedCmd;
}

string Command::trim(const string& str){
    auto validStart = find_if(str.begin(), str.end(), [](unsigned char c) {
        return !isspace(c);
    });

    auto validEnd = find_if(str.rbegin(), str.rend(), [](unsigned char c) {
        return !isspace(c);
    }).base();

    return (validStart < validEnd) 
            ? string(validStart, validEnd)
            : ""; 
}

string Command::resolvePath(const string& command){
    string arg0; 
    istringstream iss(command); 
    iss >> arg0; 

    if (arg0.rfind("./", 0) == 0) return arg0; 

    char* pathEnvVar = getenv("PATH"); 
    if (!pathEnvVar) return arg0; 

    istringstream paths(pathEnvVar); 
    string path; 

    while (getline(paths, path, ':')){
        string fullPath = path + "/" + arg0; 
        if (access(fullPath.c_str(), X_OK) == 0){
            return fullPath; 
        }
    }
    
    return arg0; 
}

vector<char*> Command::getArgsVector(const string& command){
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

void Command::debug(const ParsedCommand& parsedCmd) {
    cout << "Parsed Command Debug Info:" << std::endl;
    cout << "In Background: " << parsedCmd.inBackground << endl;
    cout << "Is Piping: " << parsedCmd.isPiping << endl;
    cout << "Is AND (&&): " << parsedCmd.isAnd << endl;
    cout << "Is OR (||): " << parsedCmd.isOr << endl;
    cout << "Command 1: " << parsedCmd.command1 << endl;
    cout << "Executable 1: " << parsedCmd.executable1 << endl;
    cout << "Arguments 1: ";
    for (char* arg : parsedCmd.args1) {
        if (arg) {
            cout << arg << " ";
        }
    }
    cout << endl;

    if (parsedCmd.isPiping || parsedCmd.isAnd || parsedCmd.isOr) {
        cout << "Command 2: " << parsedCmd.command2 << endl;
        cout << "Executable 2: " << parsedCmd.executable2 << endl;
        cout << "Arguments 2: ";
        for (char* arg : parsedCmd.args2) {
            if (arg) {
                cout << arg << " ";
            }
        }
        cout << endl;
    }
}

void Command::displayUsage(){
    cout << "Usage:\n"; 
    cout << "<command> [OPTIONS]\n"; 
    cout << "<command> [OPTIONS] | <command> [OPTIONS]\n"; 
    cout << "<command> [OPTIONS] || <command> [OPTIONS]\n"; 
    cout << "<command> [OPTIONS] && <command> [OPTIONS]\n"; 
    cout << "To run in background, append \"&\" at the end\n";
}