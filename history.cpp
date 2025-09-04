#include "header.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<string> histlist;
const int HISTORY_LIMIT = 20;

void addHistory(const string &cmd)
{
    if (cmd.empty())
        return;

    if (!histlist.empty() && histlist.back() == cmd)
        return;

    if ((int)histlist.size() >= HISTORY_LIMIT)
    {
        histlist.erase(histlist.begin());
    }
    histlist.push_back(cmd);
}

void loadHistory()
{
    histlist.clear();
    int fd = open("history.txt", O_RDONLY);
    if (fd == -1)
        return;
    char buf[1024];
    string line;
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buf, sizeof(buf))) > 0)
    {
        for (ssize_t i = 0; i < bytesRead; i++)
        {
            if (buf[i] == '\n')
            {
                addHistory(line);
                line.clear();
            }
            else
            {
                line.push_back(buf[i]);
            }
        }
    }
    if (!line.empty())
        addHistory(line);
    close(fd);
}

void saveHistory()
{
    int fd = open("history.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        return;

    int start = max(0, (int)histlist.size() - HISTORY_LIMIT);
    for (int i = start; i < (int)histlist.size(); i++)
    {
        string entry = histlist[i] + "\n";
        ssize_t written = write(fd, entry.c_str(), entry.size());
        if (written == -1)
        {
            perror("write");
            break;
        }
        while (written < (ssize_t)entry.size())
        {
            ssize_t more = write(fd, entry.c_str() + written, entry.size() - written);
            if (more == -1)
            {
                perror("write");
                break;
            }
            written += more;
        }
    }
    close(fd);
}
void printHistory(int n)
{
    if (n <= 0)
        n = 10;
    if (n > (int)histlist.size())
        n = histlist.size();
    int start = histlist.size() - n;
    for (int i = start; i < (int)histlist.size(); i++)
        cout << histlist[i] << endl;
}
