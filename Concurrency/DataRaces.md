
## What is a data race?

A data race is a situation where two threads access the same memory location concurrently and at least one of the accesses is a write.

Writes are destructive & expensive, because if writes are done - you have to tell the whole world - that you have done a write. E.g. all the memory hierarchy and caches need to be flushed.

**If a data race occurs, the behavior of progra is undefined**

### Example data race (Shared counter increments by multiple threds without synchronization)

```cpp

#include <iostream>
#include <thread>

// We have 3 threads that increment a shared counter 10000 times each.
// Ideally the counter should be 30000 at the end of the program.
// But it is not. Why?
// Because the threads are not synchronized.
int main() {
    int shared_counter = 0;
    auto work = [&shared_counter]() {
        for (int i = 0; i < 10000; i++) {
            shared_counter++;
        }
    };

    std::thread t1(work);
    std::thread t2(work);
    std::thread t3(work);

    t1.join();
    t2.join();
    t3.join();

    std::cout << shared_counter << std::endl; // 30000? No. It is undefined behavior.
}
```

### Why are increments susceptible to data races?

Since they contain:
1. read
2. modify
3. write

And more than two threads doing these operations, lead to a data race.

### Solving data races using std::atomic

**Note** - you must use `atomic_ctr++` or `atomic_ctr += 1` operator to increment an atomic variable for synchronization. 
`atomic_ctr = atomic_ctr + 1` will not work.


```cpp
// we synchronize by making data structure atomic
void solved() {
    std::atomic<int> shared_counter {0 };
    auto work = [&shared_counter]() {
        for (int i = 0; i < 10000; i++) {
            shared_counter++;
        }
    };

    std::thread t1(work);
    std::thread t2(work);
    std::thread t3(work);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "Synchronized atomic counter ans = " << shared_counter << std::endl;
}
```