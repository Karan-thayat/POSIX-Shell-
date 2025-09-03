#include "header.h"

bool handle_redirection(vector<string> &args) {
    int in_fd = -1, out_fd = -1;

    for (size_t i = 0; i < args.size();) {
        if (args[i] == "<") {
            if (i + 1 >= args.size()) {
                cerr << "Redirection error: no input file specified\n";
                return true;
            }
            in_fd = open(args[i + 1].c_str(), O_RDONLY);
            if (in_fd < 0) {
                perror("Input redirection");
                return true;
            }
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
            args.erase(args.begin() + i, args.begin() + i + 2);
        }
        else if (args[i] == ">") {
            if (i + 1 >= args.size()) {
                cerr << "Redirection error: no output file specified\n";
                return true;
            }
            out_fd = open(args[i + 1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out_fd < 0) {
                perror("Output redirection");
                return true;
            }
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
            args.erase(args.begin() + i, args.begin() + i + 2);
        }
        else if (args[i] == ">>") {
            if (i + 1 >= args.size()) {
                cerr << "Redirection error: no output file specified\n";
                return true;
            }
            out_fd = open(args[i + 1].c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (out_fd < 0) {
                perror("Append redirection");
                return true;
            }
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
            args.erase(args.begin() + i, args.begin() + i + 2);
        }
        else {
            i++;
        }
    }
    return false; // success
}
