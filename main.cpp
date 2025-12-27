#include <string>
#include "processExplorer_helpers.h"
#include "console_helpers.h"
#include "MenuManager/MenuManager.h"

using namespace std;

int ShowMenu() {
    MenuManager MainMenu("=======[ MENU ] =======");
    MenuOption option0 {"Width: "+to_string(MenuManager::consoleWidth)+" Height: "+to_string(MenuManager::consoleHeight)+ " [ CHANGE ]", nullptr};
    MenuOption option1 {"Show all proecesses", nullptr};
    MenuOption option2 {"Exit", nullptr};
    MainMenu.AddOption(option0);
    MainMenu.AddOption(option1);
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

void ChangeConsoleSettings() {
    MenuManager SettingsMenu("=======[ CONSOLE SETTINGS ] =======");
    int newWidth = SettingsMenu.HandleInput("Set new console width");
    int newHeight = SettingsMenu.HandleInput("Set new console height");
    MenuManager::NewConsoleWidth(newWidth);
    MenuManager::NewConsoleHeight(newHeight);
}

int main() {
    while (true) {
        switch (ShowMenu()) {
            case 0:
                ChangeConsoleSettings();
                break;
            case 1:
                if (SettingsSaved())
                    if (ShowAllProcesses(&currentSettings)) {
                        HandleProcessExplorer();
                    }
                    break;
            case 2: return 0;
            default: return 1;
        }
    }
}

