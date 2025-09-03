#include "header.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<string> histlist;
const int HISTORY_LIMIT = 20;

void add_history(const string &cmd) {
    if (cmd.empty()) return;
    if (!histlist.empty() && histlist.back() == cmd) return;
    if ((int)histlist.size() >= HISTORY_LIMIT)
        histlist.erase(histlist.begin());
    histlist.push_back(cmd);
}

void load_history() {
    histlist.clear();
    ifstream fin("history.txt");
    string line;
    while (getline(fin, line)) {
        if (!line.empty())
            add_history(line);
    }
}

void save_history() {
    ofstream fout("history.txt");
    int start = max(0, (int)histlist.size() - HISTORY_LIMIT);
    for (int i = start; i < (int)histlist.size(); i++)
        fout << histlist[i] << "\n";
}

void print_history(int n) {
    if (n <= 0) n = 10;
    if (n > (int)histlist.size()) n = histlist.size();
    int start = histlist.size() - n;
    for (int i = start; i < (int)histlist.size(); i++)
        cout << histlist[i] << endl;
}
