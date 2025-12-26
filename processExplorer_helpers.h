#pragma once

#include <string>
using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13

struct ProcessSettings {
    bool pid;
    bool name;
    bool path;
    bool memUsage;
    bool threads;
    bool parentPID;
    bool is64Bit;
};

struct ProcessInfo {
    int pid;
    string name;
    string path;
    int memUsage;
    int threads;
    int parentPID;
    bool is64Bit;
};

bool ShowAllProcesses(ProcessSettings* settings);
bool SettingsSaved();
string boolMessage(bool val);
