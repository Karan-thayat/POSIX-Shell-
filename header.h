#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <signal.h>
#include <limits.h>
#include <sstream>
#include <iomanip>
#include <algorithm>

#define N 4096    // PATH_MAX
using namespace std;
inline string SHELL_HOME;           
inline pid_t foreground_pgid = -1;  
inline vector<pid_t> bg_pids;       

// shared structure
struct Cmd {
    vector<string> argv;
    string infile;
    string outfile;
    bool append = false;
};
// tokenize and display
vector<string> tokenize(const string &line);
string prompt();

// Builtins 
void cd(const vector<string> &args);
void pwd();
void echo(const string &line);
void search(const vector<string> &args);
void pinfo(vector<string> args);
void lsCommand(const vector<string> &args);

// System Commands 
void sigchld_handler(int sig);
void executesyscmd(vector<string> args);

// signals and job controls
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

//  History 
extern vector<string> histlist;   
extern int history_index;             
void addHistory(const string &cmd);
void loadHistory();
void saveHistory();
string get_input(const string &prompt);
void printHistory(int n);

//  I/O redirection
bool handle_redirection(vector<string> &args);

// Pipeline
void execute_pipeline(const string &line);

//autocomplete
extern vector<string> builtin_cmds;
void setup_autocomplete();
