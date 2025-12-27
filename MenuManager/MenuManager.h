#pragma once
#include <string>
#include <vector>

using namespace std;

struct MenuOption {
    string Name;
    bool* ControlledValue;
};

class MenuManager {
private:
    string Title;
    int selectedOption;


    string boolMessage(bool val);

public:
    vector<MenuOption> Options;
    MenuManager(string t);
    static int consoleWidth;
    static int consoleHeight;
    static void NewConsoleWidth(int val);
    static void NewConsoleHeight(int val);

    int HandleInput(string message);

    void AddOption(MenuOption option);
    int Show();
};