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
    void loadHistory();                       // Load command history from a file
    void saveHistory() const;                 // Save command history to a file
    void addToHistory(const std::string &command); // Add a command to the history
    int size();                              // Get the size of the history
    std::string getHistoryCommand(bool up);   // Get a command from history for UP/DOWN navigation

    // Utility methods
    std::string getCurrentDirectory() const;  // Get the current working directory
    bool fileExists(const std::string &filePath) const; // Check if a file exists

    // Command line handling method
    void updateCommandLine(const std::string &prompt, const std::string &command); // Update the command line
    bool readArrowKey(std::string &key);     // Detect and read arrow key inputs (UP/DOWN)

private:
    std::vector<std::string> history;        // Store command history
    size_t historyIndex;                     // Index for navigating history
    std::string historyFilePath;             // Path to history file

    // Private utility methods
    void loadHistoryFromFile();              // Load history from a file
    void saveHistoryToFile() const;          // Save history to a file
};

#endif // HISTORY_H
