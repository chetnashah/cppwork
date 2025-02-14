here's how to use a header-only library in another target:

```cmake
# 1. Create the header-only library
add_library(header_lib INTERFACE)
target_include_directories(header_lib INTERFACE include)

# If header_lib depends on other libraries
target_link_libraries(header_lib INTERFACE dependency_lib)

# 2. Use the header-only library in another target
add_executable(myapp src/main.cpp)
target_link_libraries(myapp PRIVATE header_lib)
# myapp automatically gets:
# - header_lib's include directories
# - dependency_lib and its include directories
```

Directory structure would look like:
```
project/
  ├── include/
  │   └── header_lib/
  │       └── myheader.h
  ├── src/
  │   └── main.cpp
  └── CMakeLists.txt
```

Would you like me to explain any specific part in more detail?