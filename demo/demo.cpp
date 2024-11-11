#include<iostream> // cin, cout
#include<unistd.h> // fork, read, write, exec, dup2, close
#include<stdlib.h> // exit
#include<sys/types.h> // pid_t, ssize_t
#include<sstream> // istringstream
#include<string> // string
#include<vector> // vector
#include<sys/wait.h> // waitpid
#include<algorithm> // find_if
#include<cctype> // isspace

using namespace std; 

// Function to parse a single command into arguments
vector<char*> parseSingleCmd(const string& cmd) {
    istringstream iss(cmd); // Create a string stream from the command
    vector<string> tokens; // Vector to store tokens
    string token; 

    // Tokenize the command by spaces
    while (iss >> token){ 
        tokens.push_back(token); 
    }

    vector<char*> args; // Vector to store arguments as char*
    for (auto& t: tokens) {
        args.push_back(&t[0]); // Convert string to char* and add to args
    }
    args.push_back(nullptr); // Null-terminate the arguments list

    return args;
}

// Function to trim leading and trailing whitespace from a command
string trimSingleCmd(const string& cmd){
    // Find the first non-whitespace character
    auto validStart = find_if(cmd.begin(), cmd.end(), [](unsigned char ch) {
        return !isspace(ch);
    });

    // Find the last non-whitespace character
    auto validEnd = find_if(cmd.rbegin(), cmd.rend(), [](unsigned char ch) {
        return !isspace(ch);
    }).base();

    // Return the trimmed command
    return (validStart < validEnd) 
            ? string(validStart, validEnd)
            : ""; 
}

// Function to execute a single command
void execSingleCmd(const string& cmd){ 
    vector<char*> args = parseSingleCmd(cmd); // Parse the command into arguments

    pid_t rc = fork(); // Fork a new process
    if (rc < 0){
        perror("fork"); // Handle fork error
    }
    else if (rc == 0) { // Child process
        if (execvp(args[0], args.data()) < 0) { // Execute the command
            perror("execvp"); // Handle execvp error
        }
        exit(EXIT_FAILURE); // Exit if execvp fails
    }
    else { // Parent process
        int status; 
        waitpid(rc, &status, 0); // Wait for the child process to finish
    }
}

int main(){
    while(1){ 
        cout << "nutshell> "; // Display prompt
        string cmd; 
        getline(std::cin, cmd); // Read command from input

        cmd = trimSingleCmd(cmd); // Trim the command

        if (cmd.compare("exit") == 0) { // Check for exit command
            break;
        }
        execSingleCmd(cmd); // Execute the command
    }

    return 0; 
}