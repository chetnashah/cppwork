
#include <iostream>
#include <thread>

// We have 3 threads that increment a shared counter 10000 times each.
// Ideally the counter should be 30000 at the end of the program.
// But it is not. Why?
// Because the threads are not synchronized.
void problematic() {
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

    std::cout << "Unsynchronized thread answer: " << shared_counter << std::endl;
}

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

int main() {
    problematic();
    solved();
    return 0;
}
