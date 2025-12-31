#pragma once
#include <string>
#include <vector>

struct MenuOption {
    std::string Name;
    bool* ControlledValue;
};

class MenuManager {
private:
    std::string Title;
    int selectedOption;
    std::string boolMessage(bool val);

public:
    std::vector<MenuOption> Options;
    MenuManager(std::string t);
    static int consoleWidth;
    static int consoleHeight;
    static void NewConsoleWidth(int val);
    static void NewConsoleHeight(int val);

    int HandleInput(std::string message);
    void changeSelected(int val);
    void AddOption(MenuOption option);
    int Show();
};