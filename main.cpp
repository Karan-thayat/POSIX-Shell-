#include "header.h"

// helper: split string by spaces (for cd, pwd etc.)
vector<string> tokenize(const string &line)
{
    stringstream ss(line);
    string word;
    vector<string> tokens;
    while (ss >> word)
        tokens.push_back(word);
    return tokens;
}

int main()
{
    signal(SIGINT, sigintHandler);
    signal(SIGTSTP, sigtstpHandler);
    setup_signals();

    // save starting directory into SHELL_HOME
    char cwd0[PATH_MAX];
    getcwd(cwd0, sizeof(cwd0));
    SHELL_HOME = string(cwd0);

    load_history();

    while (true)
    {
       

        string line = get_input(prompt());
        if (line.empty())
            continue;

        add_history(line);

        // ========= PIPELINE HANDLING =========
        if (line.find('|') != string::npos)
        {
            execute_pipeline(line);
            continue; // already executed
        }

        // ========= NORMAL COMMANDS =========
        vector<string> args = tokenize(line);
        if (args.empty()) continue;

        string cmd = args[0];

        // save original stdin/stdout
        int saved_stdin = dup(STDIN_FILENO);
        int saved_stdout = dup(STDOUT_FILENO);

        // apply I/O redirection if any
        if (handle_redirection(args))
        {
            dup2(saved_stdin, STDIN_FILENO);
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdin);
            close(saved_stdout);
            continue;
        }

        // -------- Builtins --------
        if (cmd == "exit")
        {
            save_history();
            break;
        }
        else if (cmd == "cd")
            cd(args);
        else if (cmd == "pwd")
            pwd();
        else if (cmd == "echo")
            echo(line);  // echo preserves spaces
        else if (cmd == "search")
            search(args);
        else if (cmd == "pinfo")
            pinfo(args);
        else if (cmd == "ls")
        {
            vector<string> lsArgs(args.begin() + 1, args.end());
            lsCommand(lsArgs);
        }
        else if (cmd == "history")
        {
            int n = 10; // default
            if (args.size() > 1)
            {
                try { n = stoi(args[1]); }
                catch (...) {
                    cerr << "history: invalid number" << endl;
                    dup2(saved_stdin, STDIN_FILENO);
                    dup2(saved_stdout, STDOUT_FILENO);
                    close(saved_stdin);
                    close(saved_stdout);
                    continue;
                }
            }
            print_history(n);
        }
        else
        {
            // -------- External Commands --------
            executesyscmd(args);
        }

        // restore stdin/stdout after every command
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
    }

    return 0;
}
