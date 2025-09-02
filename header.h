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

#define  N 4096    // PATH_MAX
using namespace std;

// ========= Shared Globals =========
// Inline avoids "multiple definition" errors when included in many .cpp files
inline string SHELL_HOME;           
inline vector<string> historyVec;   
inline pid_t foreground_pgid = -1;  
inline vector<pid_t> bg_pids;       

// ========= Shared Structure =========
struct Cmd {
    vector<string> argv;
    string infile;
    string outfile;
    bool append = false;
};

// ========= Display =========
void prompt();

// builtin commands
void cd(const vector<string> &args);
void pwd();
void echo(const string &line);

// Search
void search(const vector<string> &args);

// ========= Builtins =========
int builtin_cd(vector<string> &args);
int builtin_pwd(vector<string> &args);
int builtin_echo(vector<string> &args);
int builtin_ls(vector<string> &args);
int builtin_pinfo(vector<string> &args);
int builtin_history(vector<string> &args);
int builtin_search(vector<string> &args);

bool is_shell_builtin(const string &cmd);
int run_builtin_in_parent(vector<string> &tokens);

// ========= System Commands =========
int execute_syscmd(vector<Cmd> &pipeline, bool background);

// ========= Parsing =========
vector<Cmd> parse_pipeline(const string &cmdstr, bool &background);
string trim(const string &s);

// ========= Redirection & Pipelines =========
int handle_redirection(Cmd &cmd);
int execute_pipeline(vector<Cmd> &pipeline, bool background);

// ========= Signals =========
void setup_signals();

// ========= Autocomplete =========
string autocomplete_input();

// ========= History =========
void add_to_history(const string &cmd);
void load_history_file();
void save_history_file();

void lsCommand(const vector<string>& args);
