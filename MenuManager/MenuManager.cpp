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

#define MAX_NAME_LENGTH 50

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
            while (true) {
                gotoxy(0,0);
                SetColor(7);
                cout << Title << endl;
                for (int i=0; i<Options.size(); i++) {
                    if (selectedOption == i) SetColor(240);
                    if (Options[i].ControlledValue==nullptr)
                    { cout << left << setw(MAX_NAME_LENGTH) << Options[i].Name << endl; SetColor(7); continue;}

                    cout << left << setw(MAX_NAME_LENGTH)
                    << Options[i].Name
                    << boolMessage(*Options[i].ControlledValue)
                    << endl;
                    SetColor(7);
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