#include"executor.h"

using namespace std; 

extern char** environ; 

Executor::Executor(){}

Executor::~Executor(){}

void Executor::execute(const ParsedCommand& cmd){
    if (cmd.isAnd + cmd.isOr + cmd.isPiping > 1) {
        ::_exit(EXIT_FAILURE); 
    } // todo: gotta tell them too many operators

    if (cmd.isPiping){
        executePiped(cmd, this->statCmd1, this->statCmd2); 
    } else if (cmd.isAnd || cmd.isOr) {
        executeAndOr(cmd, this->statCmd1, this->statCmd2); 
    } else { // single command 
        int rc = ::fork(); 
        if (rc < 0){
            perror("fork"); 
            ::_exit(EXIT_FAILURE); 
        } else if (rc == 0){
            ::execve(cmd.executable1.c_str(), cmd.args1.data(), environ); 
            printError(cmd.command1, errno); 
            ::_exit(EXIT_FAILURE); 
        }
        waitpid(rc, &this->statCmd1, 0);
        printError(cmd.command1, this->statCmd1);   
    }
}

void Executor::executePiped(const ParsedCommand& cmd, int& statCmd1, int& statCmd2){
    string errMsg; 

    // backup/save stdin, stdout, stderr 
    int stdin_bk = ::dup(STDIN_FILENO); 
    int stdout_bk = ::dup(STDOUT_FILENO);

    // create pipe 
    int fdPipe[2]; //[0] is read end, [1] is write end 
    pipe(fdPipe); 

    // ---------------- command1 ---------------------
    // setup output for first command
    // redirect output from STDOUT to the write end of pipe 
    // close write end of pipe after done (its dummy for setup)
    ::dup2(fdPipe[1], STDOUT_FILENO); 
    ::close(fdPipe[1]); 

    // fork process for first command 
    int rc1 = ::fork(); 
    if (rc1 < 0){
        errMsg = "fork: " + cmd.command1; 
        perror(errMsg.c_str());
        ::_exit(EXIT_FAILURE); 
    } else if (rc1 == 0){
        ::execve(cmd.executable1.c_str(), cmd.args1.data(), environ); 
        printError(cmd.command1, errno);
        ::_exit(EXIT_FAILURE); 
    }

    // restore STDOUT from backup
    ::dup2(stdout_bk, STDOUT_FILENO); 
    ::close(stdout_bk); 

    // ---------------- command2 --------------------
    // setup input for the second command 
    // redirect input from STDIN to read end of pipe 
    // close read end of pipe after done (its dummy for setup)
    ::dup2(fdPipe[0], STDIN_FILENO); 
    ::close(fdPipe[0]); 

    // fork process for second command 
    int rc2 = ::fork(); 
    if (rc2 < 0){
        errMsg = "fork: " + cmd.command2; 
        printError(cmd.command2, errno); 
        ::_exit(EXIT_FAILURE); 
    } else if (rc2 == 0){
        ::execve(cmd.executable2.c_str(), cmd.args2.data(), environ); 
        perror(cmd.command2.c_str()); 
        ::_exit(EXIT_FAILURE); 
    }

    // restore STDIN from backup 
    ::dup2(stdin_bk, STDIN_FILENO); 
    ::close(stdin_bk); 

    ::waitpid(rc1, &statCmd1, 0);
    printError(cmd.command1, statCmd1); 
    ::waitpid(rc2, &statCmd2, 0);
    printError(cmd.command2, statCmd2);
}

void Executor::executeAndOr(const ParsedCommand& cmd, int& statCmd1, int& statCmd2){
    string errMsg; 
    bool rickAstley = false; // true to continue 
    bool exitNormal; 
    int exitStatus; 

    int rc1 = ::fork(); 
    if (rc1 < 0){
        errMsg = "fork: " + cmd.command1; 
        perror(errMsg.c_str()); 
        ::_exit(EXIT_FAILURE); 
    } else if (rc1 == 0){
        ::execve(cmd.executable1.c_str(), cmd.args1.data(), environ); 
        printError(cmd.command1, errno); 
        ::_exit(EXIT_FAILURE); 
    }

    waitpid(rc1, &statCmd1, 0);
    printError(cmd.command1, statCmd1);
    exitNormal = WIFEXITED(this->statCmd1); 
    exitStatus = WEXITSTATUS(this->statCmd1); 

    if (cmd.isAnd){
        if (exitNormal && (exitStatus==0)) 
            rickAstley = true; 
    } else if (cmd.isOr) {
        if (exitNormal && (exitStatus!=0))
            rickAstley = true; 
    }

    if (rickAstley){
        int rc2 = ::fork(); 
        if (rc2 < 0){
            errMsg = "fork: " + cmd.command2; 
            perror(errMsg.c_str()); 
            ::_exit(EXIT_FAILURE); 
        } else if (rc2 == 0){
            ::execve(cmd.executable2.c_str(), cmd.args2.data(), environ); 
            printError(cmd.command2, errno);  
            ::_exit(EXIT_FAILURE); 
        }
        waitpid(rc2, &statCmd2, 0); 
        printError(cmd.command2, statCmd2);
    }
}

void Executor::debug(){
    cout << "------------------------\n";
    cout << "Executor debug info\n"; 
    cout << "Command 1 status: " << this->statCmd1 << endl; 
    cout << "Command 2 status: " << this->statCmd2 << endl; 
    cout << "------------------------\n";
}

void Executor::printError(const std::string& cmd, int status) {
    std::cerr << "Error executing command: " << cmd << std::endl;

    if (WIFSIGNALED(status)) {
        int signal = WTERMSIG(status);
        std::cerr << "Terminated by signal: " << signal << " (" << strsignal(signal) << ")" << std::endl;
        switch (signal) {
            case SIGSEGV:
                std::cerr << "Segmentation fault: Invalid memory access." << std::endl;
                break;
            case SIGABRT:
                std::cerr << "Aborted: The process was aborted, possibly due to an assertion failure." << std::endl;
                break;
            case SIGFPE:
                std::cerr << "Floating-point exception: Division by zero or similar error." << std::endl;
                break;
            default:
                std::cerr << "Signal received: " << strsignal(signal) << std::endl;
                break;
        }
    } else if (WIFEXITED(status)) {
        int exitStatus = WEXITSTATUS(status);
        std::cerr << "Exited with status: " << exitStatus << std::endl;
        if (exitStatus != 0) {
            std::cerr << "Suggestion: Check the command's syntax and ensure all dependencies are available." << std::endl;
        }
    } else {
        std::cerr << "Unknown error occurred." << std::endl;
    }
}

