#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
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
extern vector<string> histlist;   // shared history
extern int history_index;             // current index for UP/DOWN
void add_history(const string &cmd);
void load_history();
void save_history();
string get_input(const string &prompt);
void print_history(int n);


//  I/O redirection
bool handle_redirection(vector<string> &args);

// Pipeline
void execute_pipeline(const string &line);

//autocomplete
extern vector<string> builtin_cmds;
void setup_autocomplete();
