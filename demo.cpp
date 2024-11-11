#include<iostream> 
#include<unistd.h> // fork, read, write, exec, dup2, close
#include<stdlib.h> // exit
#include<sys/types.h> // pid_t, ssize_t
#include<sstream> 
#include<string> 
#include<vector>
#include<sys/wait.h>
#include<algorithm> // find_if
#include<cctype> // isspace

using namespace std; 

vector<char*> parseSingleCmd(const string& cmd) {
    istringstream iss(cmd); 
    vector<string> tokens; 
    string token; 

    while (iss >> token){ // tokenize cmd
        tokens.push_back(token); 
    }

    vector<char*> args; 
    for (auto& t: tokens) {
        args.push_back(&t[0]); 
    }
    args.push_back(nullptr); // char* because this bro :) 

    return args;
}

string trimSingleCmd(const string& cmd){
    auto validStart = find_if(cmd.begin(), cmd.end(), [](unsigned char ch) {
        return !isspace(ch);
    });

    auto validEnd = find_if(cmd.rbegin(), cmd.rend(), [](unsigned char ch) {
        return !isspace(ch);
    }).base();

    return (validStart < validEnd) 
            ? string(validStart, validEnd)
            : ""; 
}

void execSingleCmd(const string& cmd){ 
    string trimmedCmd = trimSingleCmd(cmd); 

    if (cmd.compare("exit") == 0) {
        printf("attempt exit");
        exit(0); 
    } // not working 

    vector<char*> args = parseSingleCmd(cmd); 

    pid_t rc = fork(); 
    if (rc < 0){
        perror("fork"); 
    }
    else if (rc == 0) { // child
        if (execvp(args[0], args.data()) < 0) { 
            perror("execvp"); 
        }
        exit(EXIT_FAILURE); 
    }
    else { // parent
        int status; 
        waitpid(rc, &status, 0); 
    }
}

// vector<vector<char*>> parsePipedCmd(const string& pipedCmd){ 
//     vector<vector<char*>> allCmdArgs; 
//     istringstream iss(pipedCmd); 
//     string oneCmd; 
//     vector<char*> oneCmdArgs; 
    
//     while(getline(iss, oneCmd, '|')){
//         oneCmdArgs = parseSingleCmd(oneCmd); 

//         allCmdArgs.push_back(oneCmdArgs); 
//     }
    
//     return allCmdArgs; 
// }

int main(){

    while(1){ 
        cout << "nutshell> "; 
        string cmd; 
        getline(std::cin, cmd); 

        execSingleCmd(cmd); 
    }

    return 0; 
}
