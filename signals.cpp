#include "header.h"
#include <algorithm>
#include <sys/wait.h>
#include <iostream>

using namespace std;

// ===== Globals =====
pid_t fgProcess = -1;
string fgName = "";
vector<Job> jobs;

// ===== SIGCHLD handler =====
// Reap background processes when they terminate
void sigchld_handler(int sig)
{
    (void)sig;
    int status;
    pid_t pid;

    // Reap all finished background processes
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        auto it = find_if(jobs.begin(), jobs.end(),
                          [pid](Job &job){ return job.pid == pid; });
        if (it != jobs.end())
        {
            cout << "\nBackground process finished: " << it->command
                 << " (PID " << pid << ")" << endl;
            jobs.erase(it);
        }
    }
}

// ===== Ctrl+C handler =====
void sigintHandler(int sig)
{
    (void)sig; // suppress unused parameter warning
    if (fgProcess > 0)
    {
        kill(fgProcess, SIGINT);
        cout << endl
             << "Killed foreground process: " << fgName
             << " (PID " << fgProcess << ")" << endl;
        fgProcess = -1; // reset
        fgName = "";
    }
}

// ===== Ctrl+Z handler =====
void sigtstpHandler(int sig)
{
    (void)sig; // suppress unused parameter warning
    if (fgProcess > 0)
    {
        kill(fgProcess, SIGTSTP);
        cout << endl
             << "Stopped foreground process: " << fgName
             << " (PID " << fgProcess << ")" << endl;

        jobs.push_back({fgProcess, fgName, false});
        fgProcess = -1; // reset
        fgName = "";
    }
}

// ===== Setup signal handling =====
void setup_signals()
{
    // SIGCHLD → reap background processes
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sa, nullptr) < 0)
        perror("sigaction");

    // Ignore SIGTTOU and SIGTTIN so shell can control terminal
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
}
