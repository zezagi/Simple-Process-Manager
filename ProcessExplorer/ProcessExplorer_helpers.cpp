#include <iostream>
#include <windows.h>
#include <vector>
#include <list>
#include <tlhelp32.h>
#include <iomanip>
#include <conio.h>
#include <string>
#include <psapi.h>
#include <sstream>
#include "../processExplorer_helpers.h"
#include "../console_helpers.h"
#include "../MenuManager/MenuManager.h"

#pragma comment(lib, "psapi.lib")

using namespace std;

DWORD SelectedPID = 0;

string GetMemoryUsage(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (NULL == hProcess) return "0.0 MB";

    PROCESS_MEMORY_COUNTERS pmc;
    string result = "0.0 MB";

    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        double memMB = pmc.WorkingSetSize / 1024.0 / 1024.0;
        stringstream ss;
        ss << fixed << setprecision(1) << memMB << " MB";
        result = ss.str();
    }

    CloseHandle(hProcess);
    return result;
}
string GetArch(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!hProcess) return "-";

    BOOL isWow64 = FALSE;
    if (IsWow64Process(hProcess, &isWow64)) {
        CloseHandle(hProcess);
        return isWow64 ? "x32" : "x64";
    }

    CloseHandle(hProcess);
    return "unk";
}
string GetProcessPath(DWORD pid) {
    if (pid == 0 || pid == 4) return "SYSTEM";

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!hProcess) return "-";

    char buffer[MAX_PATH];
    string path = "-";
    if (GetModuleFileNameExA(hProcess, NULL, buffer, MAX_PATH)) {
        path = buffer;
    }
    CloseHandle(hProcess);
    return path;
}
string GetHeaderString(ProcessSettings* settings) {
    stringstream ss;
    if (settings->pid) ss << left << setw(8) << "PID";
    if (settings->name) ss << left << setw(30) << "NAME";
    if (settings->path) ss << left << setw(45) << "PATH";
    if (settings->memUsage) ss << left << setw(12) << "MEM";
    if (settings->threads) ss << left << setw(8) << "THRD";
    if (settings->parentPID) ss << left << setw(10) << "PARENT";
    if (settings->is64Bit) ss << left << setw(6) << "ARCH";
    return ss.str();
}

string generateProcessString(PROCESSENTRY32* entry, ProcessSettings* settings) {
    stringstream ss;

    if (settings->pid) {
        ss << left << setw(8) << entry->th32ProcessID;
    }

    if (settings->name) {
        string name = entry->szExeFile;
        if (name.length() > 28) {
            name = name.substr(0, 25) + "...";
        }
        ss << left << setw(30) << name;
    }

    if (settings->path) {
        string path = GetProcessPath(entry->th32ProcessID);
        if (path.length() > 43) path = "..." + path.substr(path.length() - 40);
        ss << left << setw(45) << path;
    }

    if (settings->memUsage) {
        ss << left << setw(12) << GetMemoryUsage(entry->th32ProcessID);
    }

    if (settings->threads) {
        ss << left << setw(8) << entry->cntThreads;
    }

    if (settings->parentPID) {
        ss << left << setw(10) << entry->th32ParentProcessID;
    }

    if (settings->is64Bit) {
        ss << left << setw(6) << GetArch(entry->th32ProcessID);
    }

    return ss.str();
}
string generateProcessString(DWORD pid) {
    ProcessSettings allOn = {true, true, true, true, true, true, true};

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return "Error: Cannot access process list";

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    string result = "Process not found or terminated";

    if (Process32First(snapshot, &entry)) {
        do {
            if (entry.th32ProcessID == pid) {
                result = generateProcessString(&entry, &allOn);
                break;
            }
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return result;
}

bool ShowAllProcesses(ProcessSettings* settings) {
        EnableDebugPrivilege();
        PROCESSENTRY32 entry;
        static int lastSelected;
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot == INVALID_HANDLE_VALUE) return false;
        entry.dwSize = sizeof(PROCESSENTRY32);

        string headerTitle = GetHeaderString(settings);
        MenuManager ProcessListMenu(headerTitle);
        vector<DWORD> pidMap;

        if (Process32First(snapshot, &entry)) {
            do {
                string procLine = generateProcessString(&entry, settings);
                ProcessListMenu.AddOption({procLine, nullptr});
                pidMap.push_back(entry.th32ProcessID);
            } while (Process32Next(snapshot, &entry));
        }
        else {
            CloseHandle(snapshot);
            return false;
        }
        CloseHandle(snapshot);

        ProcessListMenu.AddOption({"[ BACK TO MAIN MENU ]", nullptr});
        ProcessListMenu.changeSelected(lastSelected);
        int choice = ProcessListMenu.Show();

        if (choice==-1)
            return false;

        if (choice == ProcessListMenu.Options.size()-1) {
            return false;
        }

        SelectedPID = pidMap[choice];
        if (HandleProcessExplorer(SelectedPID)) // == true, when bug?, ==  false when user wants to go back and explore processes
            return false;

         lastSelected = choice;

        // true means explore processes + refresh, false means go back to main menu
        return true;
}