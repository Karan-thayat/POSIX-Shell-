// cd echo pwd builtin
#include "header.h"

void cd(const vector<string> &args) {
    if(args.size()>2){
        cout<<"Invalid arguments for error handling"<<endl;
        return ;
    }

    if (args.size() == 1) 
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
        cout<<preDir<<endl;
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
void pwd() {
    char cwd[N];
    if (getcwd(cwd, N)) {
        cout << cwd << endl;
    } else {
        perror("pwd");
    }
}

void echo(const string &line) {

     string raw = line.substr(4);

    string output;
    for (size_t i = 0; i < raw.size(); ) {
        if (raw.compare(i, 6, "&nbsp;") == 0) {
            output.push_back(' ');
            i += 6;
        } else {
            output.push_back(raw[i]);
            i++;
        }
    }

    string collapsed;
    bool inQuotes = false;
    bool inSpace = false;

    for (size_t i = 0; i < output.size(); i++) {
        char c = output[i];

        if (c == '"' || c == '\'') {
            inQuotes = !inQuotes;  
            collapsed.push_back(c); 
            continue;
        }

        if (!inQuotes && (c == ' ' || c == '\t')) {
            if (!inSpace) {
                collapsed.push_back(' ');
                inSpace = true;
            }
        } else {
            collapsed.push_back(c);
            inSpace = false;
        }
    }

    cout << collapsed << endl;
} 

  