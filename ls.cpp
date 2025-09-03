#include "header.h"

// Expand ~ to HOME directory
string expandPath(const string &path) {
    if (path == "~") {
        const char *home = getenv("HOME");
        return home ? string(home) : ".";
    }
    return path;
}

// Print details for -l
void printLongFormat(const string &fullPath, const string &name) {
    struct stat st;
    if (stat(fullPath.c_str(), &st) == -1) {
        perror(("ls: cannot access " + fullPath).c_str());
        return;
    }

    // Permissions
    cout << ((S_ISDIR(st.st_mode)) ? "d" : "-");
    cout << ((st.st_mode & S_IRUSR) ? "r" : "-");
    cout << ((st.st_mode & S_IWUSR) ? "w" : "-");
    cout << ((st.st_mode & S_IXUSR) ? "x" : "-");
    cout << ((st.st_mode & S_IRGRP) ? "r" : "-");
    cout << ((st.st_mode & S_IWGRP) ? "w" : "-");
    cout << ((st.st_mode & S_IXGRP) ? "x" : "-");
    cout << ((st.st_mode & S_IROTH) ? "r" : "-");
    cout << ((st.st_mode & S_IWOTH) ? "w" : "-");
    cout << ((st.st_mode & S_IXOTH) ? "x" : "-");

    // Hard links
    cout << " " << setw(2) << st.st_nlink;

    // Owner and group
    struct passwd *pw = getpwuid(st.st_uid);
    struct group *gr = getgrgid(st.st_gid);
    cout << " " << (pw ? pw->pw_name : to_string(st.st_uid));
    cout << " " << (gr ? gr->gr_name : to_string(st.st_gid));

    // File size
    cout << " " << setw(8) << st.st_size;

    // Modification time
    char buf[80];
    strftime(buf, sizeof(buf), "%b %d %H:%M", localtime(&st.st_mtime));
    cout << " " << buf;

    // File name
    cout << " " << name << endl;
}

// Implementation of ls
void lsCommand(const vector<string>& args) {
    bool showAll = false;
    bool longFormat = false;
    vector<string> paths;

    // Parse arguments
    for (string arg : args) {
        if (arg[0] == '-') {
            for (size_t i = 1; i < arg.size(); i++) {
                if (arg[i] == 'a') showAll = true;
                if (arg[i] == 'l') longFormat = true;
            }
        } else {
            paths.push_back(expandPath(arg));
        }
    }

    if (paths.empty()) paths.push_back(".");

    for (size_t p = 0; p < paths.size(); p++) {
        struct stat st;
        if (stat(paths[p].c_str(), &st) == -1) {
            perror(("ls: cannot access " + paths[p]).c_str());
            continue;
        }

        // Case 1: Path is a directory
        if (S_ISDIR(st.st_mode)) {
            DIR *dir = opendir(paths[p].c_str());
            if (!dir) {
                perror(("ls: cannot open directory " + paths[p]).c_str());
                continue;
            }

            if (paths.size() > 1) cout << paths[p] << ":" << endl;

            vector<string> entries;
            struct dirent *entry;
            while ((entry = readdir(dir)) != nullptr) {
                string name = entry->d_name;
                if (!showAll && name[0] == '.') continue;
                entries.push_back(name);
            }
            closedir(dir);

            // Sort alphabetically like real ls
            sort(entries.begin(), entries.end());

            // Print entries
            for (auto &name : entries) {
                string fullPath = paths[p] + "/" + name;
                if (longFormat) {
                    printLongFormat(fullPath, name);
                } else {
                    cout << name << endl;
                }
            }

            if (p != paths.size() - 1) cout << endl;
        }
        // Case 2: Path is a file
        else {
            if (longFormat) {
                printLongFormat(paths[p], paths[p]);
            } else {
                cout << paths[p] << endl;
            }
        }
    }
}
