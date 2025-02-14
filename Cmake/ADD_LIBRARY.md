## Syntax

```
add_library(<name> [<type>] [IMPORTED] [GLOBAL] [source1] [source2] [...])
```
`<name>`: The name of the library target. This is how you'll refer to it in other CMake commands (e.g., `target_link_libraries`, `target_include_directories`).


## Examples

Here are the main forms of `add_library`:

1. **Regular Library (Static/Shared)**
```cmake
# Basic form
add_library(mylib src1.cpp src2.cpp header1.h)

# Explicitly specify library type
add_library(mylib STATIC src1.cpp src2.cpp)  # Creates libmylib.a
add_library(mylib SHARED src1.cpp src2.cpp)  # Creates libmylib.so
```

2. **Interface Library (Header-only)**
```cmake
# No source files needed
add_library(mylib INTERFACE)
# Configure interface library
target_include_directories(mylib INTERFACE include)
```

3. **Object Library**
```cmake
# Creates object files that can be reused
add_library(mylib OBJECT src1.cpp src2.cpp)
# Use these objects in other targets
add_library(shared_lib SHARED $<TARGET_OBJECTS:mylib>)
add_executable(myapp $<TARGET_OBJECTS:mylib> main.cpp)
```

4. **Imported Library (External)**
```cmake
# Pre-built library from system
add_library(zlib SHARED IMPORTED)
set_target_properties(zlib PROPERTIES
    IMPORTED_LOCATION "/usr/lib/libz.so"
    INTERFACE_INCLUDE_DIRECTORIES "/usr/include")
```

5. **Alias Library (Alternative Name)**
```cmake
add_library(original STATIC src1.cpp)
# Create alias (useful for namespacing)
add_library(myproject::original ALIAS original)
```

Control library type globally:
```cmake
# Default to shared libraries
set(BUILD_SHARED_LIBS ON)
# Now this creates a shared library
add_library(mylib src1.cpp)
```

Would you like me to elaborate on any of these forms or show more examples?