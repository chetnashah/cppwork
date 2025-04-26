#include <iostream>
#include<vector>
#include <folly/FBString.h>

#include "customer.h"
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


void workWithCustomer() {
    Customer c {std::string{"jzy"}};
    c.addValue(1);
    c.addValue(2);
    std::cout << c << std::endl;
}

void fooByVal(std::string str) {
    std::cout << str << std::endl;
}

void checkMoveByVal() {
    std::string s{"Hello"};
    std::cout << "checkMoveByVal 1: s = "<< s << std::endl;
    fooByVal(std::move(s));
    // s is empty after the move
    std::cout << "checkMoveByVal 2:: s = " << s << std::endl; // empty ""
}

void readManyLines() {
    std::vector<std::string> lines;
    std::string line;
    // use Ctrl-D in a terminal to send EOF and stop
    while ((std::getline(std::cin, line))) {
        lines.push_back(std::move(line)); // line is moved from line to lines, ready for re-use
    }
    std::cout << "lines.size():" << lines.size() << std::endl;
}

void makeACollectionOfCustomers() {
    std::vector<Customer> coll;
    // coll.reserve(16); // reserve preserves unnecessary moving!
    for (int i=0; i<12; ++i) {
        coll.push_back(Customer{"TestCustomer " + std::to_string(i+0)});
    }
}

int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the
    // <b>lang</b> variable name to see how CLion can help you rename it.
    validButUnspecifiedState();
    // readManyLines();
    checkMoveByVal();
    std::cout << "=================" <<std::endl;
    workWithCustomer();
    std::cout << "=================" <<std::endl;
    makeACollectionOfCustomers();


    folly::fbstring str = "Folly is working!";
    std::cout << str << std::endl;
    return 0;
}


// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.