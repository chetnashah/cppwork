
## Usage with cmake

The Threads module in the latest versions (>= 3.1) of CMake generates the `Threads::Threads` imported target. Linking your target against Threads::Threads adds all the necessary compilation and linking flags. It can be done like this:

```cmake
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

add_executable(test test.cpp)
target_link_libraries(test Threads::Threads)
```

## pthread_create

It takes 4 arguments:
1. address of thread variable 
2. attributes for thread - e.g. joinable, cancelable etc.
3. function pointer to code to run
4. params to pass to the code to run

### Example

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

// Function that each thread will execute
void *printHello(void *threadId) {
    long tid = (long)threadId;
    printf("Hello from thread %ld!\n", tid);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    for (t = 0; t < NUM_THREADS; t++) {
        printf("Creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, printHello, (void *)t);

        if (rc) {
            printf("Error creating thread; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // Wait for all threads to finish
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    printf("Main thread exiting.\n");
    pthread_exit(NULL);
}
```


```cpp
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
```
Let's break down each parameter:

1. `pthread_t *thread`: This is a pointer to a pthread_t variable that will be used to store the thread ID of the newly created thread. The thread ID is an identifier that can be used to refer to the thread later on. After a successful call to pthread_create, this variable will be filled with the thread ID.

2. `const pthread_attr_t *attr`: This parameter is used to specify attributes for the new thread. It can be set to NULL if you want to use the default attributes. Attributes can include information like the stack size, scheduling policy, etc.

3. `void *(*start_routine)(void *)`: This is a pointer to the function that the new thread will execute. The function should take a single argument of type void * and return a void *. This function will be the entry point for the new thread.

4. `void *arg`: This is the argument that will be passed to the start_routine function when the new thread is created. It allows you to pass data to the thread function.

The `pthread_create` function returns an integer value. If the function succeeds, it returns 0. If an error occurs, it returns an error code indicating the nature of the error.



