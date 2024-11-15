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
            perror(cmd.command1.c_str()); 
            ::_exit(EXIT_FAILURE); 
        }
        waitpid(rc, &this->statCmd1, 0);   
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
        perror(cmd.command1.c_str());
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
        perror(errMsg.c_str()); 
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
    ::waitpid(rc2, &statCmd2, 0);
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
        perror(cmd.command1.c_str()); 
        ::_exit(EXIT_FAILURE); 
    }

    waitpid(rc1, &statCmd1, 0);
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
            perror(cmd.command2.c_str()); 
            ::_exit(EXIT_FAILURE); 
        }
        waitpid(rc2, &statCmd2, 0); 
    }
}

void Executor::debug(){
    cout << "------------------------\n";
    cout << "Executor debug info\n"; 
    cout << "Command 1 status: " << this->statCmd1 << endl; 
    cout << "Command 2 status: " << this->statCmd2 << endl; 
    cout << "------------------------\n";
}

void Executor::printError(){}
