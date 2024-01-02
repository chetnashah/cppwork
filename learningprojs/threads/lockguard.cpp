
// lock_guard is a RAII idiom for functions that take a lock 
// and unlock it at the end of the function.
#include<mutex>
#include<thread>
#include<iostream>

std::mutex mutex;

int counter = 0;

void increment() {
    // std::lock_guard<std::mutex> lock(mutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    ++counter;
}

int main(){
    std::thread t1(increment);
    std::thread t2(increment);
    std::thread t3(increment);
    std::thread t4(increment);
    std::thread t5(increment);
    std::thread t6(increment);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    std::cout << counter << std::endl;
    return 0;
}