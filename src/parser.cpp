// input of parser: a big command string 
// output: command table 
// responsible for: ||, &&, >, < 

#include"parser.h"

using namespace std; 

Parser::Parser(){}

Parser::~Parser(){}

string Parser::trim(const string& str){
    auto validStart = find_if(str.begin(), str.end(), [](unsigned char c){
        return !isspace(c); 
    });

    auto validEnd = find_if(str.rbegin(), str.rend(), [](unsigned char c){
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
        if (::access(fullPath.c_str(), X_OK) == 0){
            return fullPath; 
        }
    }
    return command; 
}

vector<char*> Parser::parseSingle(const string& single){
    istringstream iss(single); 
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

vector<Block> Parser::parseIntoBlocks(const string& big){
    vector<Block> blocks;
    size_t pos = 0;

    while (pos < big.length()) {
        size_t andPos = big.find("&&", pos);
        size_t orPos = big.find("||", pos);
        size_t nextOpPos = min(andPos, orPos);

        LogicOperator currentOperator = LogicOperator::NONE;
        if (nextOpPos != string::npos) {
            currentOperator = (andPos < orPos) ? LogicOperator::AND : LogicOperator::OR;
        } else {
            nextOpPos = big.length();
        }

        string blockString = big.substr(pos, nextOpPos - pos);
        Block block;
        block.singles = parseIntoSingle(trim(blockString));
        block.nextOperator = currentOperator;

        blocks.push_back(block);
        pos = nextOpPos + 2; // Skip past '&&' or '||'
    }

    return blocks;
}

vector<Single> parseIntoSingle(const string& block){
    vector<Single> singles;
    istringstream iss(block);
    string cmd;

    while (getline(iss, cmd, '|')) {
        Single single;
        cmd = trim(cmd);

        single.executable = resolvePath(cmd);
        single.args = parseSingle(cmd);

        // Check for input/output redirection
        size_t inputPos = cmd.find('<');
        if (inputPos != string::npos) {
            single.inputFileName = trim(cmd.substr(inputPos + 1));
            cmd.erase(inputPos); // Remove redirection from the command
        }

        size_t outputPos = cmd.find('>');
        if (outputPos != string::npos) {
            single.outputFileName = trim(cmd.substr(outputPos + 1));
            cmd.erase(outputPos); // Remove redirection from the command
        }

        single.pipingNext = !iss.eof();
        singles.push_back(single);
    }

    return singles;
}

CommandTable Parser::parse(const string& bigCommand){
    return parseIntoBlocks(bigCommand);
    return {(Block){}}; // placeholder return
}