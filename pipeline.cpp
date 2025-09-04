#include "header.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

static string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}

void execute_pipeline(const string &line) {
    
    vector<string> commands;
    stringstream ss(line);
    string part;
    while (getline(ss, part, '|')) {
        commands.push_back(trim(part));
    }

    int n = commands.size();
    if (n == 0) return;

    int prev_fd = -1;
    vector<pid_t> pids;

    for (int i = 0; i < n; i++) {
        int pipefd[2];
        if (i < n - 1) {
            if (pipe(pipefd) < 0) { perror("pipe"); return; }
        }

        pid_t pid = fork();
        if (pid == 0) { 
            if (prev_fd != -1) {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (i < n - 1) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            vector<string> args = tokenize(commands[i]);
            if (args.empty()) exit(0);

            handle_redirection(args);

            string cmd = args[0];

            if (cmd == "pwd") pwd();
            else if (cmd == "echo") echo(commands[i]);
            else if (cmd == "history") {
                int n = 10;
                if (args.size() > 1) {
                    try { n = stoi(args[1]); } catch (...) { cerr << "history: invalid number\n"; }
                }
                printHistory(n);
            }
            else {
                vector<char*> argv;
                for (auto &a : args) argv.push_back(const_cast<char*>(a.c_str()));
                argv.push_back(nullptr);
                execvp(argv[0], argv.data());
                perror("execvp");
            }
            exit(0);
        } else if (pid > 0) { 
            pids.push_back(pid);
            if (prev_fd != -1) close(prev_fd);
            if (i < n - 1) {
                close(pipefd[1]);
                prev_fd = pipefd[0];
            }
        } else {
            perror("fork");
            return;
        }
    }
    for (pid_t pid : pids) waitpid(pid, NULL, 0);
}
