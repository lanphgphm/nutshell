# nutshell 
A compact, minimal Linux shell written in C++

## Features
### Multi-command execution logic
Currently supports up to 2 chaining commands. 
| Symbol | Explanation |
|--------|-------------|
| `\|`    | Pipe: Used to pass the output of one command as input to another command. |
| `\|\|`   | Logical OR: Executes the second command only if the first command fails. |
| `&&`   | Logical AND: Executes the second command only if the first command succeeds. |
| `&`   | Running process in background |  

## Developer Setup 
### Requirements 
- GCC 14.1.1 
- CMake >= 3.16

If you are running on MacOS then install GCC-14 via brew.

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
If you are a MacOS user, then run:
```bash
cmake -DCMAKE_C_COMPILER=/usr/local/bin/gcc-14 -DCMAKE_CXX_COMPILER=/usr/local/bin/g++-14 .. && cmake --build . && ./appnutshell
```
## User Installation
Not ready to be shipped to end-user.


