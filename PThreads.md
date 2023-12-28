
## Usage with cmake

The Threads module in the latest versions (>= 3.1) of CMake generates the `Threads::Threads` imported target. Linking your target against Threads::Threads adds all the necessary compilation and linking flags. It can be done like this:

```cmake
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

add_executable(test test.cpp)
target_link_libraries(test Threads::Threads)
```