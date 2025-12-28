#include <array>
#include <iostream>
#include <windows.h>
#include <vector>
#include <list>
#include <tlhelp32.h>
#include <iomanip>
#include <conio.h>
#include <string>
#include <psapi.h>
#include "../processExplorer_helpers.h"
#include "../console_helpers.h"

using namespace std;
void PrintMemoryHex(DWORD pid) {
    system("cls");
    cout << "Podaj adres startowy (HEX), np. 00400000: ";
    DWORD_PTR address;
    cin >> hex >> address;

    HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, pid);
    if (!hProcess) {
        cout << "Blad: Nie mozna otworzyc procesu (Error: " << GetLastError() << ")\n";
        system("pause");
        return;
    }

    const int chunkSize = 128;
    unsigned char buffer[chunkSize];
    SIZE_T bytesRead;

    cout << "\n--- ZRZUT PAMIECI: 0x" << hex << address << " ---\n";
    if (ReadProcessMemory(hProcess, (LPCVOID)address, buffer, chunkSize, &bytesRead)) {
        for (size_t i = 0; i < bytesRead; ++i) {
            if (i % 16 == 0) cout << "\n" << setfill('0') << setw(8) << hex << (address + i) << ": ";
            cout << setfill('0') << setw(2) << hex << (int)buffer[i] << " ";
        }
        cout << "\n";
    } else {
        cout << "Blad odczytu pamieci.\n";
    }
    CloseHandle(hProcess);
    system("pause");
}
bool HandleProcessExplorer(DWORD pid) {
    MenuManager ProcessExplorerMenu = MenuManager("[ PROCESS ]");
    MenuOption ProcessInfo = {generateProcessString(pid), nullptr};
    MenuOption options[] = {
        {"Process info", nullptr},
        {"Read memory", nullptr},
        {"Kill Process", nullptr},
        {"Back", nullptr}
    };
    ProcessExplorerMenu.AddOption(ProcessInfo);
    for (auto option : options) ProcessExplorerMenu.AddOption(option);
    ProcessExplorerMenu.Show();
}