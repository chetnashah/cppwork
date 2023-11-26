#include<iostream>
#include<thread>

void print_thread_id(int id) {
    std::cout << "printing from thread id: " << id << std::endl;
}

int main() {
    std::thread t1(print_thread_id, 1);// start parellel execution of given function
    std::cout <<"hellow world" <<std::endl;
    t1.join();
    return 0;
}