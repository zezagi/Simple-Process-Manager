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

MenuManager::MenuManager(string t) : Title(t), selectedOption(0) {}

        string MenuManager::boolMessage(bool val) {
            return val ? "[ + ] " : " [ - ]";
        }

        void MenuManager::AddOption(MenuOption option) {
            Options.push_back(option);
        }

        int MenuManager::Show() {
            ShowConsoleCursor(false);
            system("cls");
            const int CONSOLE_WIDTH = 200;
            while (true) {
                gotoxy(0,0);
                SetColor(7);

                cout << Title << endl;
                cout << string(CONSOLE_WIDTH, '-') << endl;

                int startVal = 0;
                int endVal = Options.size();

                if (Options.size() > MENU_HEIGHT) {
                    startVal = selectedOption - (MENU_HEIGHT / 2);
                    if (startVal < 0) startVal = 0;
                    endVal = startVal + MENU_HEIGHT;

                    if (endVal>Options.size()) {
                        endVal = Options.size();
                        startVal = endVal - MENU_HEIGHT;
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

                    if (lineToPrint.length() >= CONSOLE_WIDTH) {
                        lineToPrint = lineToPrint.substr(0, CONSOLE_WIDTH - 3) + "...";
                    }
                    cout << lineToPrint;
                    if (lineToPrint.length() < CONSOLE_WIDTH) {
                        cout << string(CONSOLE_WIDTH - lineToPrint.length(), ' ');
                    }
                    cout << endl;
                    SetColor(7);
                }

                if (Options.size() > MENU_HEIGHT) {
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
            }
        }