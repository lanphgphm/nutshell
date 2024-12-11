#ifndef HISTORY_H
#define HISTORY_H

#include <string>
#include <vector>
#include <fstream>

class History {
public:
    History();  // Constructor to initialize history
    ~History(); // Destructor to clean up resources

    // Methods to manage command history
    void loadHistory();                     
    void saveHistory() const;                
    void addToHistory(const std::string &command); 
    int size();                              
    std::string getHistoryCommand(bool up);  

    std::string getCurrentDirectory() const;
    bool fileExists(const std::string &filePath) const; 

    void updateCommandLine(const std::string &prompt, const std::string &command); 
    bool readArrowKey(std::string &key);   
    void resetHistoryIterator();  

private:
    std::vector<std::string> history;        
    size_t historyIndex;                     
    std::string historyFilePath;             
    void loadHistoryFromFile();              
    void saveHistoryToFile() const;          
};

#endif 
