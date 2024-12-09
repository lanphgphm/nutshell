#ifndef ERROR_H
#define ERROR_H

#include <exception>
#include <string>
#include <sstream>

class CommandError : public std::exception{
    private:
        std::string errorMessage;
    
    public: 
        CommandError(const std::string& message, const std::string& file, int line) {
            std::ostringstream oss;
            oss << "Error: " << message << "\n";
            oss << "Occurred in file: " << file << ", line: " << line;
            errorMessage = oss.str();
        }

        const char* what() const noexcept override {
            return errorMessage.c_str();
        }
};

#endif // ERROR_H