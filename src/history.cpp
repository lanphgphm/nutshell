#include "history.h"

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>

History::History() : historyIndex(-1), historyFilePath(".nutshell_history") {}

History::~History() {}

std::string History::getCurrentDirectory() const {
    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        return std::string(buffer);
    }
    return "";
}

bool History::fileExists(const std::string &filePath) const {
    struct stat buffer;
    return (stat(filePath.c_str(), &buffer) == 0);
}

int History::size() {
    return history.size();
}

void History::loadHistory() {
    std::string currentDir = getCurrentDirectory();
    std::string fullHistoryFilePath = currentDir + "/" + historyFilePath;

    if (fileExists(fullHistoryFilePath)) {
        std::ifstream file(fullHistoryFilePath);
        std::string line;
        while (std::getline(file, line)) {
            history.push_back(line);
        }
        file.close();
    }

    historyIndex = history.size();
}

void History::saveHistory() const {
    std::string currentDir = getCurrentDirectory();
    std::string fullHistoryFilePath = currentDir + "/" + historyFilePath;

    std::ofstream file(fullHistoryFilePath, std::ios::out); // Open in write mode (overwrite)
    if (!file) {
        std::cerr << "Error: Could not open history file for writing.\n";
        return;
    }

    for (const auto &cmd : history) {
        file << cmd << "\n";
    }

    file.close();
}

void History::addToHistory(const std::string &command) {
    if (history.empty() || history.back() != command) { // Only add if different from the last command
        history.push_back(command);
        historyIndex = history.size();
    }
}

std::string History::getHistoryCommand(bool up) {
    if (up && historyIndex > 0) {
        historyIndex--;
    } else if (!up && historyIndex < (int)history.size() - 1) {
        historyIndex++;
    } else if (!up && historyIndex == (int)history.size() - 1) {
        historyIndex++;
        return ""; // Clear the line
    }

    return (historyIndex >= 0 && historyIndex < (int)history.size()) ? history[historyIndex] : "";
}

void History::updateCommandLine(const std::string &prompt, const std::string &command) {
    std::cout << "\33[2K\r" << prompt << std::flush; // Clear line, move to start, and print
}

void History::resetHistoryIterator() {
    historyIndex = history.size();
}

bool History::readArrowKey(std::string &key) {
    char c;
    if (read(STDIN_FILENO, &c, 1) != 1) return false;

    if (c == '\033') { // Escape sequence
        char seq[2];
        if (read(STDIN_FILENO, &seq[0], 1) == 1 && read(STDIN_FILENO, &seq[1], 1) == 1) {
            if (seq[0] == '[') {
                if (seq[1] == 'A') { // UP arrow
                    key = "UP";
                    return true;
                } else if (seq[1] == 'B') { // DOWN arrow
                    key = "DOWN";
                    return true;
                }
            }
        }
    }
    key = ""; // Clear the key for non-arrow inputs
    return false;
}
