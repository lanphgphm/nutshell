cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    set(CMAKE_CXX_FLAGS "${CMKE_CXX_FLAGS} -stdlib=libstdc++")
endif()
project(nutshell VERSION 0.1 LANGUAGES CXX)


file(GLOB_RECURSE HEADERS "src/*.h")
file(GLOB_RECURSE SOURCES "src/*.cpp")

include_directories(src)

add_executable(appnutshell 
    "main.cpp"
    "src/command.h" "src/command.cpp" 
    "src/executor.h" "src/executor.cpp"
    "src/history.h" "src/history.cpp"
)

include(GNUInstallDirs)

# Install target (optional, for packaging later)
# install(TARGETS appnutshell DESTINATION /usr/bin)
