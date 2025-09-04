#include "header.h"
#include <algorithm>
#include <sys/wait.h>
#include <iostream>

using namespace std;

pid_t fgProcess = -1;
string fgName = "";
vector<Job> jobs;


void sigchld_handler(int sig)
{
    (void)sig;
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        auto it = find_if(jobs.begin(), jobs.end(),[pid](Job &job)
                            { return job.pid == pid; });
        if (it != jobs.end())
        {
            cout << "\nBackground process finished: " << it->command
                 << " (PID " << pid << ")" << endl;
            jobs.erase(it);
        }
    }
}

void sigintHandler(int sig)
{
    (void)sig; 
    if (fgProcess > 0)
    {
        kill(fgProcess, SIGINT);
        cerr<<endl<<fgName<<"(PID "<<fgProcess<<")"<<endl;
    }
}

void sigtstpHandler(int sig)
{
    (void)sig; 
    if (fgProcess > 0)
    {
        kill(fgProcess, SIGTSTP);
        cerr << "Stopped foreground process: " << fgName
             << " (PID " << fgProcess << ")" << endl;

        jobs.push_back({fgProcess, fgName, true});
        tcsetpgrp(STDIN_FILENO, getpgrp());
        fgProcess = -1; 
        fgName = "";
    }
}

void setup_signals()
{
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sa, nullptr) < 0)
        perror("sigaction");

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGINT, sigintHandler);
    signal(SIGTSTP,sigtstpHandler);
}
