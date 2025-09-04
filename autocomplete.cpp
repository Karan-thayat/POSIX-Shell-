#include "header.h"

using namespace std;

vector<string> builtin_cmds = {"cd","pwd","echo","history","ls","pinfo","exit","search"};

vector<string> get_files_in_dir(const string &prefix) {
    vector<string> matches;
    DIR *dir = opendir(".");
    if (!dir) return matches;
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        string name = entry->d_name;
        if (name.find(prefix) == 0) matches.push_back(name);
    }
    closedir(dir);
    return matches;
}

char *command_generator(const char *text, int state) {
    static size_t index;
    static vector<string> matches;
    if (state == 0) {
        matches.clear();
        index = 0;
        vector<string> cmds = builtin_cmds;
        vector<string> files = get_files_in_dir(text);
        cmds.insert(cmds.end(), files.begin(), files.end());
        for (auto &c : cmds) {
            if (c.find(text) == 0) matches.push_back(c);
        }
    }
    if (index >= matches.size()) return nullptr;
    return strdup(matches[index++].c_str());
}

char **autocomplete_func(const char *text, int start, int end) {
    (void)start;
    (void)end;
    return rl_completion_matches(text, command_generator);
}

void setup_autocomplete() {
    rl_attempted_completion_function = autocomplete_func;
}

string get_input(const string &prompt_str) {
    char *line = readline(prompt_str.c_str());
    if(!line){
        return "EOF" ;
    }
    if (*line) add_history(line); 
    string s(line);
    free(line);
    return s;
}
