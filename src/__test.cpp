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

#include"parser.h"
#include"executor.h"

using namespace std; 

int main(){
    while (true) {
        Parser parser; 
        Executor executor; 

        string input;
        cout << "nutshell@tester> ";
        getline(std::cin, input);
        if (input == "exit") break; 
        
        CommandTable commandTable = parser.parse(input);
        executor.execute(commandTable);
    }
    return 0; 
}