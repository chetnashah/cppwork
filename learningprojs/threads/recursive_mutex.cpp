
#include<mutex>
#include<thread>
#include<iostream>

std::recursive_mutex rec_mutex;

void f1() {
    rec_mutex.lock();
    std::cout << "f1" << std::endl;
    rec_mutex.unlock();
}

void f2() {
    rec_mutex.lock();
    std::cout << "f2" << std::endl;
    f1();
    rec_mutex.unlock();
}

int main() {
    std::thread t1(f2);
    std::thread t2(f2);
    std::thread t3(f2);
    t1.join();
    t2.join();
    t3.join();
    return 0;
}