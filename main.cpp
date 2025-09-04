#include "header.h"
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
    // register signal handlers
    signal(SIGINT, sigintHandler);   // ctrl+c
    signal(SIGTSTP, sigtstpHandler); // ctrl+z
    setup_signals();
    // save shell home directory
    char cwd0[PATH_MAX];
    if (!getcwd(cwd0, sizeof(cwd0)))
        perror("getcwd");
    SHELL_HOME = string(cwd0);

    loadHistory();
    setup_autocomplete();
    while (true)
    {

        string line = get_input(prompt());
        if (line == "EOF")
        {
            saveHistory();
            break;
        }

        if (line.empty())
            continue;

        addHistory(line);
        if (line.find('|') != string::npos)
        {
            execute_pipeline(line);
            continue;
        }

        vector<string> args = tokenize(line);
        if (args.empty())
            continue;

        string cmd = args[0];

        int saved_stdin = dup(STDIN_FILENO);
        int saved_stdout = dup(STDOUT_FILENO);

        RedirFDs fds = handle_redirection(args);

        if (fds.in_fd != -1)
        {
            dup2(fds.in_fd, STDIN_FILENO);
            close(fds.in_fd);
        }
        if (fds.out_fd != -1)
        {
            dup2(fds.out_fd, STDOUT_FILENO);
            close(fds.out_fd);
        }

        if (cmd == "exit")
        {
            saveHistory();
            break;
        }
        else if (cmd == "cd")
            cd(args);
        else if (cmd == "pwd")
            pwd();
        else if (cmd == "echo")
            echo(line);
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
            int n = 10;
            if (args.size() > 1)
            {
                try
                {
                    n = stoi(args[1]);
                }
                catch (...)
                {
                    cerr << "history: invalid number" << endl;
                    dup2(saved_stdin, STDIN_FILENO);
                    dup2(saved_stdout, STDOUT_FILENO);
                    close(saved_stdin);
                    close(saved_stdout);
                    continue;
                }
            }
            printHistory(n);
        }
        else
        {
            executesyscmd(args);
        }
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
    }

    return 0;
}
