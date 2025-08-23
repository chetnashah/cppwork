# Conditional Platform Checks in CMake: WIN32 and APPLE

CMake provides built-in variables to detect the target platform at configure time. Two of the most commonly used are:

- **WIN32**  
  True when generating build files for any Windows environment (32- or 64-bit).  
- **APPLE**  
  True when targeting any Apple platform (macOS, iOS, tvOS, watchOS).

These can be used in `CMakeLists.txt` to tailor compiler flags, link libraries, or add source files conditionally.

***

## Basic Usage

```cmake
cmake_minimum_required(VERSION 3.0)
project(MyApp)

# Common sources
set(SOURCES
    src/main.cpp
    src/common_util.cpp
)

# Platform-specific sources or settings
if(WIN32)
    message(STATUS "Configuring for Windows")
    list(APPEND SOURCES src/windows_util.cpp)
    add_definitions(-DPLATFORM_WINDOWS)
elseif(APPLE)
    message(STATUS "Configuring for Apple platform")
    list(APPEND SOURCES src/apple_util.mm)    # Objective-C++ for macOS
    add_definitions(-DPLATFORM_APPLE)
else()
    message(STATUS "Configuring for Unix/Linux")
    list(APPEND SOURCES src/unix_util.cpp)
    add_definitions(-DPLATFORM_UNIX)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
```

- `if(WIN32)` blocks execute for both 32- and 64-bit Windows generators.
- `if(APPLE)` triggers for any Apple target (macOS, iOS, etc.), so you can further discriminate with `CMAKE_SYSTEM_NAME` or `CMAKE_OSX_SYSROOT` if needed.

***

## Example: Setting Compiler Flags

You may need to adjust warning levels or language standards per platform:

```cmake
if(WIN32)
    # Use MSVC-specific flags
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
elseif(APPLE)
    # Use Clang on macOS with specific standard library
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -stdlib=libc++")
else()
    # Use GCC/Clang on Linux
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()
```

***

## Example: Conditional Linking

On Windows, you might link against `ws2_32`; on macOS, link against `Cocoa` frameworks:

```cmake
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
elseif(APPLE)
    find_library(COCOA_FRAMEWORK Cocoa)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COCOA_FRAMEWORK})
endif()
```

***

## Combining with Source-Level Guards

To control code inclusion inside C/C++ source files, define preprocessor symbols in CMake:

```cmake
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_WINDOWS)
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_APPLE)
endif()
```

Then in your C++ code:

```cpp
#include <iostream>

int main() {
#if defined(PLATFORM_WINDOWS)
    std::cout << "Running on Windows\n";
#elif defined(PLATFORM_APPLE)
    std::cout << "Running on Apple platform\n";
#else
    std::cout << "Running on Unix/Linux\n";
#endif
    return 0;
}
```

***

## Further Discrimination on Apple

If you need to tell macOS apart from iOS:

```cmake
if(APPLE)
    if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        message(STATUS "macOS target")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
        message(STATUS "iOS target")
    endif()
endif()
```

***

### Summary

- **WIN32** and **APPLE** are built-in Boolean variables in CMake for platform detection.
- Use `if(WIN32)` or `if(APPLE)` to configure sources, compiler flags, and linked libraries.
- Define your own preprocessor symbols via `target_compile_definitions` for in-code guards.
- For deeper granularity (e.g., macOS vs. iOS), inspect `CMAKE_SYSTEM_NAME` or other Apple-specific variables.