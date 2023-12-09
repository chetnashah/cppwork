
## std::mutex (non-recursive)

mutex offers exclusive, non-recursive ownership semantics:

**Mutex ownership by a thread** - A calling thread owns a mutex from the time that it successfully calls either lock or try_lock until it calls unlock.

1. When a thread owns a mutex, all other threads will block (for calls to lock) or receive a false return value (for try_lock) if they attempt to claim ownership of the mutex.
2. A calling thread must not own the mutex prior to calling lock or try_lock (i.e recursion not allowed)

The behavior of a program is undefined if a mutex is destroyed while still owned by any threads, or a thread terminates while owning a mutex. The mutex class satisfies all requirements of Mutex and StandardLayoutType.

`std::mutex` is neither copyable nor movable.

### Methods

#### mut.lock
 
locks the mutex, blocks the calling thread if the mutex is not available
(public member function)

#### mut.try_lock
 
tries to lock the mutex, returns if the mutex is not available
(public member function)

#### mut.unlock
 
unlocks the mutex
(public member function)

## std::timed_mutex

The `std::timed_mutex` class is a synchronization primitive that can be used to protect shared data from being simultaneously accessed by multiple threads.

In a manner similar to mutex, **timed_mutex offers exclusive, non-recursive ownership semantics**. In addition, `timed_mutex` provides the ability to attempt to claim ownership of a timed_mutex with a timeout via the member functions `try_lock_for()` and `try_lock_until()`.
Undefined behavior if thread already owns the mutex and tries to lock it again.

### timed_mutex Public Methods

### timed_mutex.lock
 
locks the mutex, blocks if the mutex is not available

#### timed_mutex.try_lock
 
tries to lock the mutex, returns if the mutex is not available

#### timed_mutex.try_lock_for(timeout_duration)

Blocks until the specified duration timeout_duration has elapsed (timeout) or the lock is acquired (owns the mutex), whichever comes first.
true if the lock was acquired successfully, otherwise false.



#### timed_mutex.try_lock_until(time_point)

time_point represents a point in time,
Blocks until specified timeout_time has been reached (timeout) or the lock is acquired (owns the mutex), whichever comes first. On successful lock acquisition returns true, otherwise returns false.


#### timed_mutex.unlock
 
unlocks the mutex

## std::recursive_mutex

The recursive_mutex class is a synchronization primitive that can be used to protect shared data from being simultaneously accessed by multiple threads.

recursive_mutex offers exclusive, recursive ownership semantics:

1. A calling thread owns a recursive_mutex for a period of time that starts when it successfully calls either lock or try_lock. During this period, the thread may make additional calls to lock or try_lock. **The period of ownership ends when the thread makes a matching number of calls to unlock.**
2. When a thread owns a recursive_mutex, all other threads will block (for calls to lock) or receive a false return value (for try_lock) if they attempt to claim ownership of the recursive_mutex.
3. The maximum number of times that a recursive_mutex may be locked is unspecified, but after that number is reached, calls to lock will throw std::system_error and calls to try_lock will return false.

### std::recursive_mutex Public methods

#### recMutex.lock
 
locks the mutex, blocks if the mutex is not available

#### recMutex.try_lock
 
tries to lock the mutex, returns if the mutex is not available

#### recMutex.unlock
 
unlocks the mutex

### Code

```cpp
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
```