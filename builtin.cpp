// cd echo pwd builtin
#include "header.h"
//cd
void cd(const vector<string> &args) {
    if(args.size()>2) // more than 2 arguments
    {
        cout<<"Invalid arguments for error handling"<<endl;
        return ;
    }

    if (args.size() == 1) //moves to shell home
    {
       if(chdir(SHELL_HOME.c_str())!=0)
       {
        perror("cd");
       }
        return;
    }

    string target = args[1];
    static string preDir = SHELL_HOME;
    char cwd[N];
    if(!getcwd(cwd,N))perror("getcwd");
    string currDir = cwd;

    if (target == "~") {

        if(chdir(SHELL_HOME.c_str())==0){
            preDir = currDir;
        }
    }
     else if (target == "-") {
    
        if(chdir(preDir.c_str())==0){
        preDir = currDir;
        }
    } else {
        if (chdir(target.c_str()) == 0) {
            preDir = currDir;
        }else{
            perror("cd");
        }
    }
}
//pwd
void pwd() {
    char cwd[N];
    if (getcwd(cwd, N)) {
        cout << cwd << endl;
    } else {
        perror("pwd");
    }
}
//echo
void echo(const string &line) {

    string s = line.substr(4);
    string output;
    for (size_t i = 0; i < s.size();) {
        if (s.compare(i, 6, "&nbsp;") == 0) {
            output.push_back(' ');
            i += 6;
        } else {
            output.push_back(s[i]);
            i++;
        }
    }

    string sp;
    bool inQuotes = false;
    bool inSpace = false;

    for (size_t i = 0; i < output.size(); i++) {
        char c = output[i];

        if (c == '"' || c == '\'') {
            inQuotes = true;  
            sp.push_back(c); 
            continue;
        }
        if (!inQuotes && (c == ' ' || c == '\t')) {
            if (!inSpace) {
                sp.push_back(' ');
                inSpace = true;
            }
        } else {
            sp.push_back(c);
            inSpace = false;
        }
    }

    cout << sp << endl;
} 

  