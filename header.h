#pragma once
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <fstream>
#include <iostream>
#include <vector>

#define N 4096    // PATH_MAX
using namespace std;

// ========= Shared Globals =========
// Inline avoids "multiple definition" errors when included in many .cpp files
inline string SHELL_HOME;           
inline pid_t foreground_pgid = -1;  
inline vector<pid_t> bg_pids;       

// ========= Shared Structure =========
struct Cmd {
    vector<string> argv;
    string infile;
    string outfile;
    bool append = false;
};
// helpers
vector<string> tokenize(const string &line);
// ========= Display =========
string prompt();

// ========= Builtins =========
void cd(const vector<string> &args);
void pwd();
void echo(const string &line);
void search(const vector<string> &args);
void pinfo(vector<string> args);
void lsCommand(const vector<string> &args);

// ========= System Commands =========
void sigchld_handler(int sig);
void executesyscmd(vector<string> args);

// ========= Signals =========
extern pid_t fgProcess;
struct Job {
    pid_t pid;
    string command;
    bool stopped;
};
extern vector<Job> jobs;
extern string fgName;
void sigintHandler(int sig);
void sigtstpHandler(int sig);
void setup_signals();

 // ========= History =========
extern vector<string> history_list;   // shared history
extern int history_index;             // current index for UP/DOWN
void add_history(const string &cmd);
void load_history();
void save_history();
void print_history(int n);

// ========= Input (with UP/DOWN arrows) =========
string get_input(const std::string &prompt);

//  I/O redirection
bool handle_redirection(vector<string> &args);

// Pipeline
void execute_pipeline(string line);

//autocomplete
string get_input(const string &prompt);
