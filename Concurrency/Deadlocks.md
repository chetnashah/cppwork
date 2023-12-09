
## Deadlocks

Deadlocks occur when more than two threads try to acquire the same resources in different order.

```cpp
#include<mutex>
#include<thread>
#include<iostream>

std::mutex mutex1, mutex2;

int main() {

    std::thread t1([]() {
        mutex1.lock();
        std::cout << "t1" << std::endl;
        mutex2.lock();
        mutex2.unlock();
        mutex1.unlock();
    });

    std::thread t2([]() {
        mutex2.lock();
        std::cout << "t2" << std::endl;
        mutex1.lock();
        mutex1.unlock();
        mutex2.unlock();
    });

    return 0;
}
```