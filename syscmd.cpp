#include "header.h"

void executesyscmd(vector<string> args) {
    if (args.empty()) return;

    bool background = false;

    // If last argument is "&" → run in background
    if (args.back() == "&") {
        background = true;
        args.pop_back(); 
    }

    // Convert args to char* array for execvp
    vector<char*> argv;
    for (auto &arg : args) argv.push_back(const_cast<char*>(arg.c_str()));
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {
        // In child → execute system command
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

        if (execvp(argv[0], argv.data()) < 0) {
            perror("execvp failed");
            exit(1);
        }
    } else {
        if (background) {
            // Parent does NOT wait → background process
            cout << "Started background process PID: " << pid << endl;
            jobs.push_back({pid,args[0],true});
        } else {
            // Foreground → wait for child to finish
            fgProcess = pid;
            fgName = args[0];

            int status;
            waitpid(pid, &status, WUNTRACED);
            fgProcess = -1;
            fgName ="";
        }
    }
}
