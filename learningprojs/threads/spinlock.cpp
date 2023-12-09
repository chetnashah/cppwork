
// a lock implemented using atomic_flag by spinning/wasting CPU cycles

#include <atomic>
#include <thread>
#include <iostream>

class Spinlock {
    std::atomic_flag flag;
public:
    Spinlock(): flag(ATOMIC_FLAG_INIT) {}

    void lock() {
        while (flag.test_and_set());
    }

    void unlock() {
        flag.clear();
    }
};

Spinlock spinlock;

int main() {
    std::thread t1([]() {
        spinlock.lock();
        std::this_thread::sleep_for(std::chrono::milliseconds(15000));
        std::cout << "t1" << std::endl;
        spinlock.unlock();
    });

    std::thread t2([]() {
        spinlock.lock();
        std::cout << "t2" << std::endl;
        spinlock.unlock();
    });

    t1.join();
    t2.join();
    return 0;
}