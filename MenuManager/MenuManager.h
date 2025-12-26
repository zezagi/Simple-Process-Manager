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
    vector<MenuOption> Options;
    int selectedOption;

    string boolMessage(bool val);

public:
    MenuManager(string t);

    void AddOption(MenuOption option);
    int Show();
};