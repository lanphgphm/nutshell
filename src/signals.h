#ifndef SIGNAL_MESSAGES_H
#define SIGNAL_MESSAGES_H

#include <unordered_map>
#include <string>

static const std::unordered_map<int, std::string> signalMessages = {
    {SIGSEGV, "Segmentation Fault. Your process is going to kernel's jail."},
    {SIGFPE, "Floating-point exception (e.g., divide by zero)"},
    {SIGABRT, "Aborted"},
    {SIGILL, "Illegal instruction."},
    {SIGKILL, "Killed"},
    {SIGTERM, "Terminated: Your process has been asked to leave the house."},
    {SIGQUIT, "Quit. The user requested program termination."},
};

#endif // SIGNAL_MESSAGES_H