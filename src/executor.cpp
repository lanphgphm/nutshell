#include"executor.h"

using namespace std; 

extern char** environ; 

Executor::Executor(){}

Executor::~Executor(){}

void Executor::execute(const ParsedCommand& cmd){}

void Executor::executePiped(const ParsedCommand& cmd){}

void Executor::executeAndOr(const ParsedCommand& cmd){}

void Executor::printError(){}

void Executor::debug(){}