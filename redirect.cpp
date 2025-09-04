#include "header.h"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <vector>
using namespace std;

RedirFDs handle_redirection(vector<string> &args) {
    RedirFDs fds;
    fds.in_fd = -1;
    fds.out_fd = -1;

    for (size_t i = 0; i < args.size(); ) {
        if (args[i] == "<") {
            if (i + 1 >= args.size()) {
                cerr << "Input redirection: No file specified\n";
                fds.in_fd = -2; // signal error
                return fds;
            }
            int fd = open(args[i+1].c_str(), O_RDONLY);
            if (fd < 0) {
                perror("Input redirection");
                fds.in_fd = -2; // signal error
                return fds;
            }
            fds.in_fd = fd;
            args.erase(args.begin() + i, args.begin() + i + 2);
        }
        else if (args[i] == ">") {
            if (i + 1 >= args.size()) {
                cerr << "Output redirection: No file specified\n";
                fds.out_fd = -2; // signal error
                return fds;
            }
            int fd = open(args[i+1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("Output redirection");
                fds.out_fd = -2;
                return fds;
            }
            fds.out_fd = fd;
            args.erase(args.begin() + i, args.begin() + i + 2);
        }
        else if (args[i] == ">>") {
            if (i + 1 >= args.size()) {
                cerr << "Output redirection: No file specified\n";
                fds.out_fd = -2;
                return fds;
            }
            int fd = open(args[i+1].c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0) {
                perror("Output redirection");
                fds.out_fd = -2;
                return fds;
            }
            fds.out_fd = fd;
            args.erase(args.begin() + i, args.begin() + i + 2);
        }
        else {
            i++;
        }
    }

    return fds;
}
