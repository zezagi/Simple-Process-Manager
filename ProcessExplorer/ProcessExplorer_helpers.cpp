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

#pragma comment(lib, "psapi.lib")

using namespace std;

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

void PrintHeader(ProcessSettings* settings) {
    SetColor(240);
    if (settings->pid) cout << left << setw(8) << "PID";
    if (settings->name) cout << left << setw(30) << "NAME";
    if (settings->path) cout << left << setw(45) << "PATH";
    if (settings->memUsage) cout << left << setw(12) << "MEM";
    if (settings->threads) cout << left << setw(8) << "THRD";
    if (settings->parentPID) cout << left << setw(10) << "PARENT";
    if (settings->is64Bit) cout << left << setw(6) << "ARCH";
    SetColor(7);
    cout << endl;
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

void EnableDebugPrivilege() {
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tkp;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {
            tkp.PrivilegeCount = 1;
            tkp.Privileges[0].Luid = luid;
            tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL);
        }
        CloseHandle(hToken);
    }
}

bool ShowAllProcesses(ProcessSettings* settings) {
    EnableDebugPrivilege();
    PROCESSENTRY32 entry;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return false;
    entry.dwSize = sizeof(PROCESSENTRY32);

    system("cls");
    PrintHeader(settings);

    if (Process32First(snapshot, &entry)) {
        do {
            cout << generateProcessString(&entry, settings) << endl;
        } while (Process32Next(snapshot, &entry));
    }
    else {
        CloseHandle(snapshot);
        return false;
    }

    CloseHandle(snapshot);
    cout << "BACKSPACE to back to main menu \n ENTER to explore processes" << endl;
    int key = _getch();
    if (key == KEY_ENTER) {
        return true;
    }
    else if (key == KEY_BACKSPACE) {
        return false;
    }
}