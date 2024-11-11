# nutshell 
A compact, minimal Linux shell written in C++

## Intended Features
### Multi-command execution logic
| Symbol | Explanation |
|--------|-------------|
| `\|`    | Pipe: Used to pass the output of one command as input to another command. |
| `\|\|`   | Logical OR: Executes the second command only if the first command fails. |
| `&&`   | Logical AND: Executes the second command only if the first command succeeds. |
| `>`    | Output Redirection: Redirects the output of a command to a file. |
| `<`    | Input Redirection: Redirects the input of a command from a file. |
| `&`   | Running process in background |  
### Scripting support 
Basic control flows for Bash-like scripting: 
| Syntax | Explanation |
|--------|-------------|
| `while`| While Loop: Repeatedly executes a block of code as long as a condition is true. |
| `if`   | If Statement: Executes a block of code if a specified condition is true. |
| `else` | Else Statement: Executes a block of code if the condition in the `if` statement is false. |
### Environment Variables
- Environment variables for each session
- `.nutshellrc` configuration file for persistent environment configuration

## Developer Setup 
### Requirements 
- GCC 14.1.1 
- CMake >= 3.16

### Recommended 
- The recommended editor is Visual Studio Code or QtCreator 

### Build Instructions 
1. Clone the repository && `cd` into it
```bash 
git clone https://github.com/lanphgphm/nutshell.git
cd nutshell 
```
2. Add your code to `src/` directory 
- It is recommended that you put all imports and method signatures into a `class.h` file. The `class.cpp` file is only for implementation. 
- See class template in `src/templates/` (don't add your code under this directory).

3. Compile source code & run app
```bash 
cd build
cmake .. && cmake --build . && ./appnutshell 
```

## User Installation
Not ready to be shipped to end-user.


