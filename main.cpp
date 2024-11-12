// responsible for: handling interrupt signals, setting envvars, changing prompts between users 
// the important signals: SIGINT, SIGSTP, SIGCHLD, SIGTERM, SIGKILL
// the important envvars: PATH, SHELL, HOME, USER, PWD, PS1, HISTFILE, HISTSIZE 
// changing prompts: whenever switch user, change username associated with prompt using format user@nutshell > 