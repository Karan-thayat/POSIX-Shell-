#include "header.h"

pid_t fgProcess = -1;
string fgName ="";
vector<Job> jobs;

// Ctrl+C → interrupt foreground job
void sigintHandler(int sig) {
    if (fgProcess > 0) {
        kill(fgProcess, SIGINT);
        cout<<endl<<"Killed foreground process: "<<fgName<<" (Pid "<<fgProcess <<")"<<endl;
    }
}

// Ctrl+Z → stop foreground job and push to background
void sigtstpHandler(int sig) {
    if (fgProcess > 0) {
        kill(fgProcess, SIGTSTP);
        cout<<endl<<"Stopped foreground process: "<<fgName<<"(PID "<<fgProcess<<endl;
        jobs.push_back({fgProcess,fgName,false});
    }
}
