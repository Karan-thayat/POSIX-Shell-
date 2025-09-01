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
    getcwd(cwd,N);
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
    if (getcwd(cwd, N) != NULL) {
        cout << cwd << endl;
    } else {
        perror("pwd");
    }
}

void echo(const string &line) {
    if(line.size()<=5){
        cout<<endl;
        return;
    }
    string output = line.substr(5);  
    cout << output << endl;
}