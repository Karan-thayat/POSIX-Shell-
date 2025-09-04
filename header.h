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
#include <readline/readline.h>
#include <readline/history.h>
#include <cstring>

#define N 4096    // PATH_MAX

using namespace std;

//  Globals 
inline string SHELL_HOME;           
inline pid_t foreground_pgid = -1;  
inline vector<pid_t> bg_pids;       

//  Job Control 
struct Job {
    pid_t pid;
    string command;
    bool stopped;
};

extern pid_t fgProcess;
extern string fgName;
extern vector<Job> jobs;

//  Command structure 
struct Cmd {
    vector<string> argv;
    string infile;
    string outfile;
    bool append = false;
};

//  Tokenizer & Prompt 
vector<string> tokenize(const string &line);
string prompt();

//  Builtins 
void cd(const vector<string> &args);
void pwd();
void echo(const string &line);
void search(const vector<string> &args);
void pinfo(vector<string> args);
void lsCommand(const vector<string> &args);

//  System Commands 
void executesyscmd(vector<string> args);

//  Signals 
void sigchld_handler(int sig);
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

//  I/O Redirection 
struct RedirFDs {
    int in_fd;
    int out_fd;
};
RedirFDs handle_redirection(vector<string> &args);

//  Pipelines 
void execute_pipeline(const string &line);

//  Autocomplete 
extern vector<string> builtin_cmds;
void setup_autocomplete();
