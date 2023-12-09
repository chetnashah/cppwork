
#include<iostream>
#include<thread>
#include<mutex>

std::mutex mutex;

// here we observe if we sleep while holding a lock, we do not busy wait.
int main() {
    std::thread t1([]() {
        mutex.lock();
        std::this_thread::sleep_for(std::chrono::milliseconds(25000));
        std::cout << "t1" << std::endl;
        mutex.unlock();
    });

    std::thread t2([]() {
        mutex.lock();
        std::cout << "t2" << std::endl;
        mutex.unlock();
    });

    t1.join();
    t2.join();
    return 0;
}