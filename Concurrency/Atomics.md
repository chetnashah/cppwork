
## std::atomic_flag

1. It is a **lock free atomic.** 
2. `clear` method sets it to false
3. `test_and_set` method sets it to true and returns previous value.
4. To use it, it must be initialized to false with constant initializer `ATOMIC_FLAG_INIT`

## Spinlock using std::atomic_flag

With a spinlock, you can protect a critical section as you would with a mutex, with active/busy CPU waiting consuming CPU cycles. 
The spinlock will not passively wait, in contrast to a mutex, until it gets it to lock. It will eagerly ask for the lock to get access to the critical section.
**it fully utilizes the CPU and does waste CPU cycles.**

```cpp
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
```

