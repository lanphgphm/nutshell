cmake_minimum_required(VERSION 3.16)
project(nutshell VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE HEADERS "src/*.h")
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(appnutshell 
    main.cpp
)

include(GNUInstallDirs)

# Install target (optional, for packaging later)
# install(TARGETS appnutshell DESTINATION /usr/bin)
