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
#include "processExplorer_helpers.h"
#include "console_helpers.h"
#include "MenuManager/MenuManager.h"

using namespace std;

int ShowMenu() {
    MenuManager MainMenu("=======[ MENU ] =======");
    MenuOption option {"Show all proecesses", nullptr};
    MenuOption option2 {"Exit", nullptr};
    MainMenu.AddOption(option);
    MainMenu.AddOption(option2);
    return MainMenu.Show();
}

ProcessSettings currentSettings={true, true, true};
bool SettingsSaved() {
    MenuManager SettingsMenu("=======[ SETTINGS ] =======");
    MenuOption options[] = {
        "[ SAVE AND SHOW PROCESSES ]", nullptr,
        "Show PID", &currentSettings.pid,
        "Show Name", &currentSettings.name,
        "Show Path", &currentSettings.path,
        "Show Memory Usage", &currentSettings.memUsage,
        "Show Threads", &currentSettings.threads,
        "Show Parent ID", &currentSettings.parentPID,
        "Show 64 bit?", &currentSettings.is64Bit,
    };
    for (auto option : options) SettingsMenu.AddOption(option);
    return SettingsMenu.Show() == 0;
}

int main() {
    while (true) {
        switch (ShowMenu()) {
            case 0:
                if (SettingsSaved())
                    if (ShowAllProcesses(&currentSettings)) {
                        HandleProcessExplorer();
                    }
                    break;
            case 1: return 0;
            default: return 1;
        }
    }
}

