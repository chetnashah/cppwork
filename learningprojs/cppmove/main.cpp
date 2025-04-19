#include <iostream>
#include<vector>
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
void validButUnspecifiedState() {
    std::string s("Hello");
    std::vector<std::string> coll;
    coll.push_back(std::move(s));
    std::cout << "original s:" << s << std::endl; // empty if moved from
    for (const std::string& el: coll) {
        std::cout << el << std::endl;
    }
}


int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the
    // <b>lang</b> variable name to see how CLion can help you rename it.
    validButUnspecifiedState();

    return 0;
}


// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.