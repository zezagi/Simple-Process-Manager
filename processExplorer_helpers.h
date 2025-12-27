#pragma once

#include <string>
#include "MenuManager/MenuManager.h"
#include <windows.h>
#include <tlhelp32.h>


#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13
#define KEY_BACKSPACE 8

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
    std::string name;
    std::string path;
    int memUsage;
    int threads;
    int parentPID;
    bool is64Bit;
};

bool ShowAllProcesses(ProcessSettings* settings);
bool SettingsSaved();
bool HandleProcessExplorer(DWORD pid);
std::string generateProcessString(DWORD pid);
std::string generateProcessString(PROCESSENTRY32* entry, ProcessSettings* settings);
std::string boolMessage(bool val);
