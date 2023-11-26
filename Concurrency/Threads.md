
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
