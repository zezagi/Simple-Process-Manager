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

bool HandleProcessExplorer(DWORD pid) {
    MenuManager ProcessExplorerMenu = MenuManager("[ PROCESS ]");
    MenuOption ProcessInfo = {generateProcessString(pid), nullptr};
    MenuOption options[] = {
        {"Process info", nullptr},
        {"Show and read memory", nullptr},
        {"Kill Process", nullptr},
        {"Back", nullptr}
    };
    ProcessExplorerMenu.AddOption(ProcessInfo);
    for (auto option : options) ProcessExplorerMenu.AddOption(option);
    ProcessExplorerMenu.Show();
}