#include "header.h"

// trim helper
static string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}

void execute_pipeline(const string &line) {
    // split by '|'
    vector<string> commands;
    stringstream ss(line);
    string part;
    while (getline(ss, part, '|')) {
        commands.push_back(trim(part));
    }

    int n = commands.size();
    int prev_fd = -1;

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
            // ===== Child =====
            if (prev_fd != -1) {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (i < n - 1) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            // tokenize this command
            vector<string> args = tokenize(commands[i]);
            if (args.empty()) exit(0);

            // apply redirections if any
            handle_redirection(args);

            string cmd = args[0];

            // ===== Builtins in pipeline =====
            if (cmd == "cd") cd(args);
            else if (cmd == "pwd") pwd();
            else if (cmd == "echo") echo(commands[i]);
            else if (cmd == "history") {
                int n = 10;
                if (args.size() > 1) {
                    try { n = stoi(args[1]); } 
                    catch (...) { cerr << "history: invalid number" << endl; }
                }
                print_history(n);
            } 
            else {
                // external command
                vector<char*> argv;
                for (auto &a : args) argv.push_back((char*)a.c_str());
                argv.push_back(NULL);
                execvp(argv[0], argv.data());
                perror("execvp");
            }
            exit(0);
        } 
        else if (pid > 0) {
            // ===== Parent =====
            if (prev_fd != -1) close(prev_fd);
            if (i < n - 1) {
                close(pipefd[1]);
                prev_fd = pipefd[0];
            }
            waitpid(pid, NULL, 0);
        } 
        else {
            perror("fork");
            return;
        }
    }
}
