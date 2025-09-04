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
    signal(SIGINT, sigintHandler);
    signal(SIGTSTP, sigtstpHandler);
    setup_signals();

    char cwd0[PATH_MAX];
    if(!getcwd(cwd0, sizeof(cwd0))) perror("getcwd");
    SHELL_HOME = string(cwd0);

    loadHistory();
    setup_autocomplete();
    while (true)
    {
       

        string line = get_input(prompt());
        if (line.empty())
            continue;

        addHistory(line);
        if (line.find('|') != string::npos)
        {
            execute_pipeline(line);
            continue; 
        }

        vector<string> args = tokenize(line);
        if (args.empty()) continue;

        string cmd = args[0];

        int saved_stdin = dup(STDIN_FILENO);
        int saved_stdout = dup(STDOUT_FILENO);

        if (handle_redirection(args))
        {
            dup2(saved_stdin, STDIN_FILENO);
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdin);
            close(saved_stdout);
            continue;
        }

        if (cmd == "exit")
        {
            saveHistory();
            break;
        }
        else if (cmd == "cd")cd(args);
        else if (cmd == "pwd")pwd();
        else if (cmd == "echo")echo(line);  
        else if (cmd == "search")search(args);
        else if (cmd == "pinfo")pinfo(args);
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
