#include "header.h"
#include <termios.h>
#include <unistd.h>

vector<string> history_list;
int history_index = 0;

const int HISTORY_LIMIT = 20;   // max stored
const int PRINT_LIMIT   = 10;   // default max printed

// ---------------- Add command ----------------
void add_history(const string &cmd) {
    if (cmd.empty()) return;

    // avoid consecutive duplicates
    if (!history_list.empty() && history_list.back() == cmd) return;

    if ((int)history_list.size() >= HISTORY_LIMIT) {
        history_list.erase(history_list.begin()); // drop oldest
    }
    history_list.push_back(cmd);
}

// ---------------- Load from file ----------------
void load_history() {
    history_list.clear();
    ifstream fin("history.txt");
    string line;
    while (getline(fin, line)) {
        if (!line.empty()) {
            add_history(line);
        }
    }
    fin.close();
}

// ---------------- Save to file ----------------
void save_history() {
    ofstream fout("history.txt");
    int start = max(0, (int)history_list.size() - HISTORY_LIMIT);
    for (int i = start; i < (int)history_list.size(); i++) {
        fout << history_list[i] << "\n";
    }
    fout.close();
}

// ---------------- Print history ----------------
void print_history(int n) {
    if (n <= 0) n = PRINT_LIMIT;
    if (n > (int)history_list.size()) n = history_list.size();

    int start = history_list.size() - n;
    for (int i = start; i < (int)history_list.size(); i++) {
        cout << history_list[i] << endl;
    }
}

// ---------------- Terminal helpers ----------------
static void set_input_mode(struct termios &old) {
    struct termios newt = old;
    newt.c_lflag &= ~(ICANON | ECHO); // raw mode
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

static void reset_input_mode(struct termios &old) {
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
}

// ---------------- Arrow navigation ----------------
string get_input(const string &prompt) {
    cout << prompt;
    cout.flush();

    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    set_input_mode(oldt);

    string buffer;
    int index = history_list.size();  // cursor starts after last
    int c;

    while (true) {
        c = getchar();

        if (c == '\n') {  // Enter
            cout << endl;
            break;
        } else if (c == 127 || c == '\b') {  // Backspace
            if (!buffer.empty()) {
                buffer.pop_back();
                cout << "\b \b";
                cout.flush();
            }
        } else if (c == 27) { // Escape sequence (arrow keys)
            if (getchar() == '[') {
                char arrow = getchar();
                if (arrow == 'A') { // ↑ Up
                    if (index > 0) {
                        index--;
                        // clear line
                        cout << "\33[2K\r" << prompt;
                        buffer = history_list[index];
                        cout << buffer;
                        cout.flush();
                    }
                } else if (arrow == 'B') { // ↓ Down
                    if (index < (int)history_list.size() - 1) {
                        index++;
                        cout << "\33[2K\r" << prompt;
                        buffer = history_list[index];
                        cout << buffer;
                        cout.flush();
                    } else {
                        // go to empty input
                        index = history_list.size();
                        cout << "\33[2K\r" << prompt;
                        buffer.clear();
                        cout.flush();
                    }
                }
            }
        } else if (isprint(c)) { // printable chars
            buffer.push_back(c);
            cout << (char)c;
            cout.flush();
        }
    }

    reset_input_mode(oldt);
    return buffer;
}
