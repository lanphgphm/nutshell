#include<iostream> // cin, cout
#include<string> // string


#include"parser.h"
#include"executor.h"

using namespace std; 

string litmus_test = "cat < input.txt | grep \"error\" > output.txt && echo \"Errors found\" || echo \"No errors found\" & ";

int main(){
    Parser parser; // share one instance across all big commands 

    while (true) {
        Executor executor; // one instance per big command

        string input;
        cout << "nutshell@tester> ";
        getline(std::cin, input);
        if (input == "exit") break; 
        
        CommandTable commandTable = parser.parse(input);
        executor.execute(commandTable);
    }
    return 0; 
}

// cat < input.txt | grep "error" > output.txt && echo "Errors found" || echo "No errors found" & 
