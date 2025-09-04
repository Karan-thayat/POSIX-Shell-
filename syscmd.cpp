#include "header.h"
#include <iostream>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

using namespace std;

void executesyscmd(vector<string> args) {
    if (args.empty()) return;

    bool background = false;
    if (args.back() == "&") {
        background = true;
        args.pop_back();
    }

    vector<char*> argv;
    for (auto &arg : args) argv.push_back(const_cast<char*>(arg.c_str()));
    argv.push_back(nullptr);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {
       
        setpgid(0, 0);  

        if (!background) {
            
            tcsetpgrp(STDIN_FILENO, getpid());
        }

        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

        if (execvp(argv[0], argv.data()) < 0) {
            perror("execvp failed");
            exit(1);
        }
    } 
    else {
        setpgid(pid, pid);  

        if (background) {
            cout << "Started background process: " << args[0] 
                 << " (PID " << pid << ")" << endl;
            jobs.push_back({pid, args[0], false}); 
        } 
        else {
            fgProcess = pid;
            fgName = args[0];

            tcsetpgrp(STDIN_FILENO, pid);

            int status;
            waitpid(pid, &status, WUNTRACED);

            tcsetpgrp(STDIN_FILENO, getpgrp());

            if (WIFSTOPPED(status)) {
                jobs.push_back({pid, args[0], true}); 
                cout << endl << "Stopped foreground process: " << args[0] 
                     << " (PID " << pid << ")" << endl;
            }

            fgProcess = -1;
            fgName = "";
        }
    }
}
