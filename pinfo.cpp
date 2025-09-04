// implements pinfo[pid]

#include "header.h"
#include <fstream>
#include <sstream>
#include <limits.h>

void pinfo(vector<string> args) {
    pid_t pid;

    if (args.size() == 1) {
        pid = getpid();
    } 
    else {
        pid = stoi(args[1]);
    }

    string statusPath = "/proc/" + to_string(pid) + "/status";
    string exePath = "/proc/" + to_string(pid) + "/exe";

    ifstream statusFile(statusPath);
    if (!statusFile.is_open()) {
        cerr << "pinfo: No process with PID " << pid << endl;
        return;
    }

    string line, state, memory;
    while (getline(statusFile, line)) {
        if (line.rfind("State:", 0) == 0) {
            string key;
            stringstream ss(line);
            ss >> key >> state;  
        } else if (line.rfind("VmSize:", 0) == 0) {
            string key;
            stringstream ss(line);
            ss >> key >> memory; 
        }
    }
    statusFile.close();

    if (tcgetpgrp(STDIN_FILENO) == getpgid(pid)) {
        state += "+";
    }

  
    char exeBuf[PATH_MAX];
    ssize_t len = readlink(exePath.c_str(), exeBuf, sizeof(exeBuf) - 1);
    string exeRealPath;
    if (len != -1) {
        exeBuf[len] = '\0';
        exeRealPath = string(exeBuf);
    } else {
        exeRealPath = "N/A";
    }

    cout << "pid -- " << pid << endl;
    cout << "Process Status -- " << state << endl;
    cout << "memory -- " << memory << endl;
    cout << "Executable Path -- " << exeRealPath << endl;
}
