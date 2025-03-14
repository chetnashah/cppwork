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