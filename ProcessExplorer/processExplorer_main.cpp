#include <iostream>
#include <windows.h>
#include <iomanip>
#include <string>
#include <psapi.h>
#include <vector>
#include "../processExplorer_helpers.h"
#include "../console_helpers.h"
#include "../MenuManager/MenuManager.h"

#pragma comment(lib, "psapi.lib")

using namespace std;

DWORD_PTR GetProcessBaseAddress(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!hProcess) return 0;

    HMODULE hMod;
    DWORD cbNeeded;
    DWORD_PTR baseAddr = 0;

    if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
        baseAddr = (DWORD_PTR)hMod;
    }

    CloseHandle(hProcess);
    return baseAddr;
}

void KillProcess(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess == NULL) return;
    TerminateProcess(hProcess, 1);
    CloseHandle(hProcess);
}

void PrintMemoryHex(DWORD pid) {
    ShowConsoleCursor(true);
    system("cls");

    DWORD_PTR address = GetProcessBaseAddress(pid);
    if (address == 0) {
        cout << "Error: Could not find base address." << endl;
        system("pause");
        return;
    }

    cout << "Auto-detected Base Address: 0x" << hex << address << endl;
    cout << "Press ENTER to read from here or type new offset (hex): ";

    string input;
    char c = cin.get();
    if(c == '\n') {

    } else {
        cin.putback(c);
        DWORD_PTR offset;
        if (cin >> hex >> offset) {
            address = offset;
        }
        cin.ignore(1000, '\n');
    }

    HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, pid);
    if (!hProcess) {
        system("pause");
        return;
    }

    const int chunkSize = 128;
    unsigned char buffer[chunkSize];
    SIZE_T bytesRead;
    cout << endl;

    if (ReadProcessMemory(hProcess, (LPCVOID)address, buffer, chunkSize, &bytesRead)) {
        for (size_t i = 0; i < bytesRead; ++i) {
            if (i % 16 == 0) cout << endl << setfill('0') << setw(8) << hex << (address + i) << ": ";
            cout << setfill('0') << setw(2) << hex << (int)buffer[i] << " ";
        }
        cout << endl;
    } else {
        cout << "Error reading memory at " << hex << address << endl;
    }

    CloseHandle(hProcess);
    system("pause");
    ShowConsoleCursor(false);
}

bool HandleProcessExplorer(DWORD pid) {
    while (true) {
        string info = generateProcessString(pid);
        if (info == "Process not found or terminated") return false;
        MenuManager ProcessExplorerMenu("[ PROCESS ]");
        ProcessExplorerMenu.AddOption({info, nullptr});
        ProcessExplorerMenu.AddOption({"Read memory (Base Address)", nullptr});
        ProcessExplorerMenu.AddOption({"Kill Process", nullptr});
        ProcessExplorerMenu.AddOption({"Back", nullptr});
        int choice = ProcessExplorerMenu.Show();
        if (choice == 1) PrintMemoryHex(pid);
        else if (choice == 2) {
            KillProcess(pid);
            return false;
        }
        else if (choice == 3 || choice == -1) return false;
    }
}