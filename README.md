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
### Scripting support 
Basic control flows for Bash-like scripting: 
| Syntax | Explanation |
|--------|-------------|
| `while`| While Loop: Repeatedly executes a block of code as long as a condition is true. |
| `if`   | If Statement: Executes a block of code if a specified condition is true. |
| `else` | Else Statement: Executes a block of code if the condition in the `if` statement is false. |
### Configure Environment 
- Environment variables for each session
- `.nutshellrc` configuration file for persistent environment configuration

## Developer Setup 
### Requirements 
- GCC 14.1.1

### Recommended 
- The recommended editor is Visual Studio Code or QtCreator 

### Build Instructions 
1. Clone the repository & `cd` into the repository 
```bash
git clone https://github.com/lanphgphm/nutshell.git
cd nutshell 
```
2. Compile source code (only demo file for now)
```bash
g++ demo.cpp -o demo  
```
3. Run the shell 
```bash
./demo
```
- Interact with the shell by running commands like `ls -la`, `echo hello`, `./hello` etc. 
- Find out the commands that your terminal has that this shell does not 

## User Installation
Not ready to be shipped to end-user.

