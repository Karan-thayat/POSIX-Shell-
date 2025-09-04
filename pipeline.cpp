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
    stringstream ss(line);
    string segment;
    vector<string> sequences;

    // Split by ;
    while (getline(ss, segment, ';')) {
        string trimmed = trim(segment);
        if (!trimmed.empty()) sequences.push_back(trimmed);
    }

    for (const string &seq : sequences) {
        // Split by |
        vector<string> commands;
        stringstream ss2(seq);
        string part;
        while (getline(ss2, part, '|')) {
            commands.push_back(trim(part));
        }

        int n = commands.size();
        if (n == 0) continue;

        int prev_fd = -1;
        vector<pid_t> pids;

        for (int i = 0; i < n; i++) {
            int pipefd[2];
            if (i < n - 1) {
                if (pipe(pipefd) < 0) {
                    perror("pipe");
                    return;
                }
            }

            pid_t pid = fork();
            if (pid == 0) { 
                // Input from previous pipe
                if (prev_fd != -1) {
                    dup2(prev_fd, STDIN_FILENO);
                    close(prev_fd);
                }

                // Output to next pipe
                if (i < n - 1) {
                    dup2(pipefd[1], STDOUT_FILENO);
                }

                // Close unused pipe ends
                if (i < n - 1) {
                    close(pipefd[0]);
                    close(pipefd[1]);
                }

                vector<string> args = tokenize(commands[i]);
                if (args.empty()) _exit(0);

                // Apply redirection inside child
                RedirFDs fds = handle_redirection(args);

                // Exit immediately if any open() failed
                if (fds.in_fd == -2 || fds.out_fd == -2) _exit(1);

                if (fds.in_fd != -1) {
                    if (dup2(fds.in_fd, STDIN_FILENO) < 0) {
                        perror("dup2 input");
                        _exit(1);
                    }
                    close(fds.in_fd);
                }
                if (fds.out_fd != -1) {
                    if (dup2(fds.out_fd, STDOUT_FILENO) < 0) {
                        perror("dup2 output");
                        _exit(1);
                    }
                    close(fds.out_fd);
                }
                if (args.empty()) _exit(0);

                // Builtins 
                string cmd = args[0];
                if (cmd == "pwd") pwd();
                else if (cmd == "echo") echo(commands[i]);
                else if (cmd == "cd") cd(args);
                else if (cmd == "search") search(args);
                else if (cmd == "pinfo") pinfo(args);
                else if (cmd == "ls") {
                    vector<string> lsArgs(args.begin() + 1, args.end());
                    lsCommand(lsArgs);
                }
                else if (cmd == "history") {
                    int num = 10;
                    if (args.size() > 1) {
                        try { num = stoi(args[1]); }
                        catch (...) { cerr << "history: invalid number\n"; }
                    }
                    printHistory(num);
                }
                else {
                    // External command 
                    vector<char*> argv;
                    for (auto &a : args) argv.push_back(const_cast<char*>(a.c_str()));
                    argv.push_back(nullptr);
                    execvp(argv[0], argv.data());
                    perror("execvp");
                }

                // Flush streams before exit
                cout.flush();
                cerr.flush();
                _exit(0);
            }
            else if (pid > 0) {
                
                pids.push_back(pid);
                if (prev_fd != -1) close(prev_fd);
                if (i < n - 1) {
                    close(pipefd[1]);
                    prev_fd = pipefd[0];
                }
            }
            else {
                perror("fork");
                return;
            }
        }

        for (pid_t pid : pids) {
            waitpid(pid, NULL, 0);
        }
    }
}
