#include "header.h"

string prompt()
{
    char curdir[N];
    if(!getcwd(curdir, N))perror("getcwd");
    string cwd = curdir;

    char sysname[N];
    gethostname(sysname, N);
    string host = sysname;

    struct passwd *pw = getpwuid(getuid());
    string username = pw ? pw->pw_name : "user";

    string res;

    
    if (cwd.compare(0, SHELL_HOME.size(), SHELL_HOME) == 0)
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