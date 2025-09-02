// implements ls builtin with -a and -l
#include "header.h"

// Helper: expand ~ to HOME directory
string expandPath(const string &path) {
    if (path == "~") {
        const char *home = getenv("HOME");
        return home ? string(home) : string(".");
    }
    return path;
}

// Helper: print permissions like rwxr-xr-x
string getPermissions(mode_t mode) {
    string perm;
    perm += (S_ISDIR(mode)) ? 'd' : '-';
    perm += (mode & S_IRUSR) ? 'r' : '-';
    perm += (mode & S_IWUSR) ? 'w' : '-';
    perm += (mode & S_IXUSR) ? 'x' : '-';
    perm += (mode & S_IRGRP) ? 'r' : '-';
    perm += (mode & S_IWGRP) ? 'w' : '-';
    perm += (mode & S_IXGRP) ? 'x' : '-';
    perm += (mode & S_IROTH) ? 'r' : '-';
    perm += (mode & S_IWOTH) ? 'w' : '-';
    perm += (mode & S_IXOTH) ? 'x' : '-';
    return perm;
}

// Print -l format
void printLongFormat(const string &dir, const string &name) {
    struct stat fileStat;
    string fullPath = dir + "/" + name;

    if (stat(fullPath.c_str(), &fileStat) == -1) {
        perror("stat");
        return;
    }

    // permissions
    cout << getPermissions(fileStat.st_mode) << " ";

    // links
    cout << setw(2) << fileStat.st_nlink << " ";

    // owner
    struct passwd *pw = getpwuid(fileStat.st_uid);
    struct group  *gr = getgrgid(fileStat.st_gid);
    cout << setw(8) << (pw ? pw->pw_name : to_string(fileStat.st_uid)) << " ";
    cout << setw(8) << (gr ? gr->gr_name : to_string(fileStat.st_gid)) << " ";

    // size
    cout << setw(8) << fileStat.st_size << " ";

    // time
    char timeBuf[80];
    strftime(timeBuf, sizeof(timeBuf), "%b %d %H:%M", localtime(&fileStat.st_mtime));
    cout << timeBuf << " ";

    // filename
    cout << name << endl;
}

// Core function
void processDirectory(const string &path, bool showAll, bool longFormat) {
    DIR *dir = opendir(path.c_str());
    if (!dir) {
        perror(("ls: cannot access " + path).c_str());
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        string name = entry->d_name;

        if (!showAll && name[0] == '.')
            continue;

        if (longFormat)
            printLongFormat(path, name);
        else
            cout << name << endl;
    }
    closedir(dir);
}

// Main ls command
void lsCommand(const vector<string>& args) {
    bool showAll = false;
    bool longFormat = false;
    vector<string> paths;

    // Parse arguments
    for (string arg : args) {
        if (arg[0] == '-') {
            for (size_t i = 1; i < arg.size(); i++) {
                if (arg[i] == 'a') showAll = true;
                else if (arg[i] == 'l') longFormat = true;
            }
        } else {
            paths.push_back(expandPath(arg));
        }
    }

    if (paths.empty()) {
        paths.push_back(".");
    }

    for (size_t i = 0; i < paths.size(); i++) {
        if (paths.size() > 1)
            cout << paths[i] << ":" << endl;

        processDirectory(paths[i], showAll, longFormat);

        if (i != paths.size() - 1)
            cout << endl;
    }
}
