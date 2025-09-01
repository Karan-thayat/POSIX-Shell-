// main file
#include "header.h"

// helper: split string by spaces (for cd, pwd etc.)
vector<string> tokenize(const string &line) {
    stringstream ss(line);
    string word;
    vector<string> tokens;
    while (ss >> word) tokens.push_back(word);
    return tokens;
}

int main() {
    // save starting directory into SHELL_HOME
    char cwd0[PATH_MAX];
    getcwd(cwd0, sizeof(cwd0));
    SHELL_HOME = string(cwd0);

    while (true) {
        prompt();  // show prompt

        string line;
        if (!getline(cin, line)) break;   // Ctrl+D quits
        if (line.empty()) continue;

        vector<string> args = tokenize(line);
        string cmd = args[0];

        if (cmd == "exit") break;
        else if (cmd == "cd") cd(args);
        else if (cmd == "pwd") pwd();
        else if (cmd == "echo") echo(line);  
        else if (cmd == "search") search(args);
        else {
            cout << "Unknown command: " << cmd << endl;
        }
    }

    return 0;
}
