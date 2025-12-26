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

    void AddOption(MenuOption option);
    int Show();
};