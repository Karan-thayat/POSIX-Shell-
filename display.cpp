#include "header.h"

string prompt()
{
    char curdir[N];        
    if(!getcwd(curdir, N))perror("getcwd"); //current working directory
    string cwd = curdir;

    char sysname[N];            
    gethostname(sysname, N);   // system's hostname
    string host = sysname;

    struct passwd *pw = getpwuid(getuid()); //get current user's uid and looks passwd struct for that uid
    string username = pw ? pw->pw_name : "user";


    string res;
    if (cwd.compare(0, SHELL_HOME.size(), SHELL_HOME) == 0) // shortening the path
    {
        res = "~" + cwd.substr(SHELL_HOME.size());
        if (res == "~")
            res = "~";
    }
    else
    {
        res = cwd;
    }
    return  "<" + username + "@" + host + ":" + res + "> ";
    
}