
C++ 11 is the first standard aware of multiple threads.

## Creating & Running threads

We could use standard library `std::thread` to create threads.

Use `std::thread` constructor to create a thread and pass a function or lambda to it to execute.

```cpp
#include<iostream>
#include<thread>

// function or lambda to be given to a thread to run/execute
void print_thread_id(int id) {
    std::cout << "printing from thread id: " << id << std::endl;
}

int main() {
    std::thread t1(print_thread_id, 1);// start parellel execution of given function
    std::cout <<"hellow world" <<std::endl;
    t1.join(); // make main thread wait for t1 to finish
    return 0;
}
```

## A thread must be marked for detatch or joining before destruction!!!

One must mark a thread object for detatch or joining before it is destructed, otherwise
the program is terminated with `std::terminate`.

```cpp
#include <thread>
#include <iostream>
#include <chrono>

void runSomeThread()
{
    std::thread t([]()
                  { std::cout << "Running thread t" << std::endl; });

    // ERROR! as t goes out of scope and destructed,
    // Bcoz we have not decided to detatch or join t,
    // Alternative: move t via returning so that it is not destructed!
}

int main()
{
    runSomeThread();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}
```

Note: that you only have to make this decision before the std::thread object is destroyed—the thread itself may well have finished long before you join with it or detach it, and if you detach it, then if the thread is still running, it will continue to do so, and may continue running long after the std::thread object is destroyed.

## jThread

Introduced in C++20, automatically rejoins on destruction.

## Thread joining

a joinable thread is a thread that can be "joined" by another thread. When a thread is joinable, it means that the program can explicitly wait for that thread to finish its execution before continuing.

You can wait on a thread to finish by calling `join()` on it. e.g. if a thread t is created,
then you can wait for it to finish by calling `t.join()`.

**Note** - You can only join a thread once. If you try to join a thread twice, it will result in a runtime error.

e.g.
```cpp
int main() {
    std::thread t1(print_thread_id, 1);// start parellel execution of given function
    std::cout <<"hellow world" <<std::endl;
    t1.join(); // make main thread wait for t1 to finish
    t1.join(); // runtime error
    return 0;
}
```


## Thread Detaching

You can detach a thread by calling `detach()` on it. e.g. if a thread t is created,
then you can detach it by calling `t.detach()`.

When you detach a thread, you essentially relinquish control and responsibility for managing that thread's resources.
The thread will continue executing in the background until it completes. **You can no longer wait on a detached thread to finish.**

## Manage local function memory used by thread

Either copy your data for the thread, so it does not have to worry about scope based destruction,
or join the thread before function exit, so thread has guaranteed acess to function scoped memory.