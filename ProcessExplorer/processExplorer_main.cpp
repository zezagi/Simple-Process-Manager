#include <iostream>
#include <windows.h>
#include <iomanip>
#include <string>
#include <psapi.h>
#include <vector>
#include <cmath>
#include <cctype>
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

string GuessType(const unsigned char* buffer) {
    bool isString = true;
    for (int i = 0; i < 4; i++) {
        if (!isprint(buffer[i])) { isString = false; break; }
    }
    if (isString) return "String (tekst)";

    float fVal;
    memcpy(&fVal, buffer, sizeof(float));

    if (isnormal(fVal) && abs(fVal) > 0.001f && abs(fVal) < 10000000.0f) {
         return "Float";
    }

    if (buffer[3] == 0 && buffer[2] == 0) {
        return "Int (liczba calkowita)";
    }

    return "Raw / Unknown";
}

void InspectMemory(DWORD pid) {
    ShowConsoleCursor(true);
    system("cls");

    DWORD_PTR address = GetProcessBaseAddress(pid);
    cout << "Process Base: 0x" << hex << address << endl;
    cout << "Enter address (HEX) or press ENTER to use base: ";

    if (cin.peek() == '\n') {
        cin.ignore();
    } else {
        cin >> hex >> address;
        cin.ignore(1000, '\n');
    }

    HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, pid);
    if (!hProcess) {
        cout << "Failed to open process!" << endl;
        system("pause");
        return;
    }

    const int size = 128;
    unsigned char buffer[size];
    SIZE_T bytesRead;

    cout << "\n--- DATA INSPECTOR: 0x" << hex << address << " ---\n";

    if (ReadProcessMemory(hProcess, (LPCVOID)address, buffer, size, &bytesRead)) {
        int intVal;
        float floatVal;
        double doubleVal;
        bool boolVal;

        memcpy(&intVal, buffer, sizeof(int));
        memcpy(&floatVal, buffer, sizeof(float));
        memcpy(&doubleVal, buffer, sizeof(double));
        boolVal = (buffer[0] != 0);

        cout << left << setw(20) << "Type" << "Value (first 4-8 bytes)" << endl;
        cout << string(60, '-') << endl;

        cout << left << setw(20) << "Int" << intVal << endl;
        cout << left << setw(20) << "Float" << floatVal << endl;
        cout << left << setw(20) << "Double" << doubleVal << endl;
        cout << left << setw(20) << "Bool" << (boolVal ? "true" : "false") << endl;
        cout << string(60, '-') << endl;
        cout << "AI Guess: " << GuessType(buffer) << endl;
        cout << string(60, '-') << endl;

        cout << "\n--- RAW HEX DUMP (" << dec << size << " bytes) ---\n";

        for (int i = 0; i < bytesRead; i++) {
            if (i % 16 == 0) {
                if (i > 0) cout << endl;
                cout << hex << setfill('0') << setw(2) << i << "| ";
            }
            cout << hex << setfill('0') << setw(2) << (int)buffer[i] << " ";
        }

        cout << "\n\n--- TEXT PREVIEW ---\n";
        for (int i = 0; i < bytesRead; i++) {
            if (isprint(buffer[i])) cout << buffer[i];
            else cout << ".";
        }
        cout << endl;

    } else {
        cout << "Memory read error." << endl;
    }

    CloseHandle(hProcess);
    cout << "\n";
    system("pause");
    ShowConsoleCursor(false);
}

void PrintMemoryHex(DWORD pid) {
    ShowConsoleCursor(true);
    system("cls");
    DWORD_PTR address = GetProcessBaseAddress(pid);
    cout << "Podaj adres (HEX) [ENTER dla Bazy]: ";
    if (cin.peek() != '\n') cin >> hex >> address;
    cin.ignore(1000, '\n');

    HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, pid);
    if(hProcess){
        const int chunkSize = 128; unsigned char buffer[chunkSize]; SIZE_T br;
        if(ReadProcessMemory(hProcess, (LPCVOID)address, buffer, chunkSize, &br)){
            cout << "\n--- HEX DUMP: 0x" << hex << address << " ---\n";
            for(size_t i=0; i<br; ++i) {
                if(i%16==0) cout << endl << setw(8) << hex << (address+i) << ": ";
                cout << setw(2) << hex << (int)buffer[i] << " ";
            }
        }
        CloseHandle(hProcess);
    }
    cout << endl; system("pause"); ShowConsoleCursor(false);
}

bool HandleProcessExplorer(DWORD pid) {
    while (true) {
        string info = generateProcessString(pid);
        if (info == "Process not found or terminated") return false;

        MenuManager ProcessExplorerMenu("[ PROCESS ]");
        ProcessExplorerMenu.AddOption({info, nullptr});
        ProcessExplorerMenu.AddOption({"Inspect Value (Smart Types)", nullptr});
        ProcessExplorerMenu.AddOption({"Read memory (Hex Dump)", nullptr});
        ProcessExplorerMenu.AddOption({"Kill Process", nullptr});
        ProcessExplorerMenu.AddOption({"Back", nullptr});

        int choice = ProcessExplorerMenu.Show();
        if (choice == 1) InspectMemory(pid);
        else if (choice == 2) PrintMemoryHex(pid);
        else if (choice == 3) {
            KillProcess(pid);
            return false;
        }
        else if (choice == 4 || choice == -1) return false;
    }
}