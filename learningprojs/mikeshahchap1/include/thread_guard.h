//
// Created by jay shah on 09/03/25.
//

#ifndef THREAD_GUARD_H
#define THREAD_GUARD_H
#include<thread>

class thread_guard
{
    std::thread t;
public:
    explicit thread_guard(std::thread&& t_):
        t(std::move(t_))
    {}

    ~thread_guard()
    {
        if(t.joinable())
        {
            t.join();
        }
    }
    thread_guard(thread_guard const&)=delete;
    thread_guard& operator=(thread_guard const&)=delete;
};

struct func;
void f()
{
    int some_local_state=0;
    func my_func(some_local_state);
    std::thread t(my_func);
    thread_guard g(std::move(t));
    do_something_in_current_thread();
}

#endif //THREAD_GUARD_H
