
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

```cpp