
## std::lock_guard

`std::lock_guard` is a class template, which implements the RAII for mutexes. It wraps the mutex inside it’s object and locks the attached mutex in its constructor. When it’s destructor is called it releases the mutex.

A lock_guard is a RAII (Resource Acquisition Is Initialization) object that automatically unlocks the mutex when it goes out of scope.

It needs a mutex object to be passed while its creation. 

It guarantees the unlocking of mutex in case of exceptions.


### Without lock_guard

Following program returns 4,5 or 6 due to data races

```cpp
// lock_guard is a RAII idiom for functions that take a lock and unlock it
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
```


## With lock_guard

`lock_guard` is a RAII idiom for functions that take a lock and unlock it

```cpp
// lock_guard is a RAII idiom for functions that take a lock and unlock it
#include<mutex>
#include<thread>
#include<iostream>

std::mutex mutex;

int counter = 0;

void increment() {
    std::lock_guard<std::mutex> lock(mutex); // RAII idiom - lock on mutex stays till end of function
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
```


## std::unique_lock

**The class unique_lock is a general-purpose mutex ownership wrapper allowing deferred locking, time-constrained attempts at locking, recursive locking, transfer of lock ownership, and use with condition variables.**

1. Wraps a mutex, but has more features than a std::lock_guard.
Unlike a std::lock_guard, a std::unique_lock can be explicitly locked and unlocked after creation.
2. At creation, by default, it automatically locks the mutex (same as a std::lock_guard), but this behavior can be modified by passing special values to the constructor.
3. Upon destruction as it exits scope, it automatically unlocks the mutex (same as a std::lock_guard).
4. Is required by the receiving side (the side which is notified) for use by a std::condition_variable in order to receive the notification.
5. The reason a std::unique_lock is required by a std::condition_variable is so that it can lock the underlying mutex each time the condition variable wakes up from a wait after a valid notification and runs a critical section of code, and unlock the underlying mutex each time `A)` the condition variable wait() call spuriously wakes up and it needs to wait again, and `B)` upon automatic destruction when the critical section runs and is over and the scope of the std::unique_lock is exited.
6. You can always use a std::unique_lock in place of a std::lock_guard, but not the other way around.


### code


## differences
