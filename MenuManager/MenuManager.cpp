#include "MenuManager.h"

#include <cmath>
#include <conio.h>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>

#include "MenuManager.h"
#include "../processExplorer_helpers.h"
#include "../console_helpers.h"

#define MENU_HEIGHT 30

using namespace std;

int MenuManager::consoleWidth = 80;
int MenuManager::consoleHeight = 25;

MenuManager::MenuManager(string t) : Title(t), selectedOption(0) {}

        string MenuManager::boolMessage(bool val) {
            return val ? "[ + ] " : " [ - ]";
        }

        void MenuManager::AddOption(MenuOption option) {
            Options.push_back(option);
        }

        void MenuManager::NewConsoleWidth(int val) {
            if (val<0) val=150;
            consoleWidth = val;
        }

        void MenuManager::NewConsoleHeight(int val) {
    if (val<0 || val>45) val=40;
            consoleHeight = val;
        }

        int MenuManager::Show() {
            ShowConsoleCursor(false);
            system("cls");
            while (true) {
                gotoxy(0,0);
                SetColor(7);

                cout << Title << endl;
                cout << string(consoleWidth, '-') << endl;

                int startVal = 0;
                int endVal = Options.size();

                if (Options.size() > consoleHeight) {
                    startVal = selectedOption - (consoleHeight / 2);
                    if (startVal < 0) startVal = 0;
                    endVal = startVal + consoleHeight;

                    if (endVal>Options.size()) {
                        endVal = Options.size();
                        startVal = endVal - consoleHeight;
                    }
                }

                for (int i = startVal; i < endVal; i++) {
                    if (selectedOption == i) SetColor(240);
                    else SetColor(7);
                    string lineToPrint;
                    if (Options[i].ControlledValue==nullptr) lineToPrint = Options[i].Name;
                    else {
                        string namePart = Options[i].Name;
                        if (namePart.length() > 35) namePart = namePart.substr(0, 35); // Limit nazwy w ustawieniach
                        string padding(40 - namePart.length(), ' ');
                        lineToPrint = namePart + padding + boolMessage(*Options[i].ControlledValue);
                    }

                    if (lineToPrint.length() >= consoleWidth) {
                        lineToPrint = lineToPrint.substr(0, consoleWidth - 3) + "...";
                    }
                    cout << lineToPrint;
                    if (lineToPrint.length() < consoleWidth) {
                        cout << string(consoleWidth - lineToPrint.length(), ' ');
                    }
                    cout << endl;
                    SetColor(7);
                }

                if (Options.size() > consoleHeight) {
                    cout << "--- " << (selectedOption + 1) << " / " << Options.size() << " ---   " << endl;
                }

                int key = _getch();
                if (key == 224 || key == 0) {
                    key = _getch();
                    if (key == KEY_UP) {
                        if (selectedOption==0) selectedOption = Options.size()-1;
                        else selectedOption--;
                    }
                    else if (key == KEY_DOWN) {
                        if (selectedOption==Options.size()-1) selectedOption = 0;
                        else selectedOption++;
                    }
                }
                if (key==KEY_ENTER) {
                    if (Options[selectedOption].ControlledValue==nullptr) return selectedOption;
                    *Options[selectedOption].ControlledValue = !*Options[selectedOption].ControlledValue;
                }
                if (key==KEY_BACKSPACE) {
                    return -1;
                }
            }
        }

int MenuManager::HandleInput(string message) {
    int c;
    while (true) {
        cout << message << endl;
        cout << "> ";
        cin>>c;
        return c;
    }
}

void MenuManager::changeSelected(int val) {
    MenuManager::selectedOption = val;
}
