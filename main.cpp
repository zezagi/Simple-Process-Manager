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

using namespace std;

int startingIndex = 2;
vector <pair<bool, string>> mainMenuMessages = {
    {false, "=======[ MENU ] ======="},
    {true, "Show all processes"},
    {true, "Exit"},
};

vector<string> settingsMenuMessages = {
    "[ SAVE AND SHOW PROCESSES ]",
    "PID (Process ID)",
    "Name",
    "Path",
    "Memory Usage",
    "Threads",
    "Parent ID",
    "64 bit?"
};

int ShowMenu() {
    int selected = 2;
    while (true) {
        system("cls");
        int index = 1;
        for (const auto& message : mainMenuMessages) {
            if (selected == index) {
                if (!message.first) {
                    selected = startingIndex;
                    continue;
                }
                SetColor(240);
                cout << message.second << endl;
                SetColor(7);
            }
            else {
                cout << message.second << endl;
            }
            index++;
        }
        int key = _getch();
        if (key == 224 || key == 0) {
            key = _getch();
            if (key == KEY_UP) {
                if (selected==startingIndex) selected = mainMenuMessages.size();
                else selected--;
            }
            else if (key == KEY_DOWN) {
                if (selected==mainMenuMessages.size()) selected = startingIndex;
                else selected++;
            }

        }
        if (key==KEY_ENTER) break;
    }
    return selected;
}
string boolMessage(bool val) {
    return val ? "[ + ] " : " [ - ]";
}
ProcessSettings currentSettings={true, true, true};
bool SettingsSaved() {
    vector<bool> settingsState = {
        false, // Placeholder dla save
        currentSettings.pid,
        currentSettings.name,
        currentSettings.path,
        currentSettings.memUsage,
        currentSettings.threads,
        currentSettings.parentPID,
        currentSettings.is64Bit
    };
    int selected = 0;
    ShowConsoleCursor(false);
    system("cls");
    while (true) {
        gotoxy(0, 0);
        for (int i=0; i<settingsMenuMessages.size(); i++) {
            if (selected == i) SetColor(240);
            if (i == 0) {
                cout << " " << left << setw(30) << settingsMenuMessages[i] << endl;
            }
            else {
                cout << " " << left << setw(20) << settingsMenuMessages[i]
                     << boolMessage(settingsState[i]) << " " << endl;
            }
            SetColor(7);
        }
        int key = _getch();
        if (key == 224 || key == 0) {
            key = _getch();
            if (key == KEY_UP) {
                if (selected==0) selected = settingsMenuMessages.size();
                else selected--;
            }
            else if (key == KEY_DOWN) {
                if (selected==settingsMenuMessages.size()) selected = 0;
                else selected++;
            }
        }
        if (key==KEY_ENTER) {
            if (selected==0) {
                currentSettings.pid = settingsState[1];
                currentSettings.name = settingsState[2];
                currentSettings.path = settingsState[3];
                currentSettings.memUsage = settingsState[4];
                currentSettings.threads = settingsState[5];
                currentSettings.parentPID = settingsState[6];
                currentSettings.is64Bit = settingsState[7];
                return true;
            }
            settingsState[selected] = !settingsState[selected];

        }
    }
}

int main() {
    while (true) {
        switch (ShowMenu()) {
            case 2:
                if (SettingsSaved())
                    ShowAllProcesses(&currentSettings);
                    break;
            case 3: return 0;
            default: return 1;
        }
    }
}

