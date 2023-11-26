
#include<iostream>
#include<utility>

struct S {
    int x;

    S(int new_x) : x(new_x) {
        std::cout << "S(int new_x) constructor called" << std::endl;
    }

    // copy constructors are declared by const reference
    S(const S& some_s) : x(some_s.x) {
        std::cout << "S(const int& new_x)  copy constructor called" << std::endl;
    }

    // move constructors are declared by rvalue reference
    S(S&& moved_s) : x(moved_s.x) {
        std::cout << "S(int&& new_x) move constructor called" << std::endl;
    }
};

int main(){
    std::vector<S> vec;
    // S s1(1);
    // vec.push_back(s1);// copy s1 into vec
    // vec.push_back(S(2));// move S(2) into vec since S(2) is a temporary object
    vec.emplace_back(10); // directly construct S(10) in vec
    return 0;
}