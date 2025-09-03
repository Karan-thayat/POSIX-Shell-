// recursive search for filename
#include "header.h"

bool searchHelper(const string &dirPath, const string &target) {
    DIR *dir = opendir(dirPath.c_str());
    if (!dir) return false;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        string name = entry->d_name;

        if (name == "." || name == "..") continue;

        if (name == target) {
            closedir(dir);
            return true;   
        }

        string fullPath = dirPath + "/" + name;
        
        struct stat st;
        if (stat(fullPath.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
            if (searchHelper(fullPath, target)) {
                closedir(dir);
                return true;
            }
        }
    }

    closedir(dir);
    return false;
}
// search command 
void search(const vector<string> &args) {
    if (args.size() != 2) {
        cout << "Usage: search <filename>" << endl;
        return;
    }

    string target = args[1];

    char cwd[N];
    getcwd(cwd, N);
    string startDir = cwd;

    bool found = searchHelper(startDir, target);

    cout << (found ? "True" : "False") << endl;
}


  