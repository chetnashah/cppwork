In CMake, the keywords `PRIVATE`, `PUBLIC`, and `INTERFACE` are used to control the visibility and usage requirements of target properties, such as include directories, compile definitions, and linked libraries.

1. **PRIVATE**:
   - The `PRIVATE` keyword is used to specify that a property should only be used by the target itself, and not propagated to other targets that link against it.
   - When you use `PRIVATE`, the property is not visible to the consumers of the target.
   - Example:
     ```cmake
     target_include_directories(my_target PRIVATE include_dir)
     ```
     In this case, the `include_dir` directory is only used for compiling `my_target`, and not exposed to other targets that link against `my_target`.

2. **PUBLIC**:
   - The `PUBLIC` keyword is used to specify that a property should be used by the target itself and also propagated to other targets that link against it.
   - When you use `PUBLIC`, the property is visible to the consumers of the target.
   - Example:
     ```cmake
     target_include_directories(my_target PUBLIC include_dir)
     ```
     In this case, the `include_dir` directory is used for compiling `my_target` and is also exposed to other targets that link against `my_target`.

3. **INTERFACE**:
   - The `INTERFACE` keyword is used to specify that a property should not be used by the target itself, but should be propagated to other targets that link against it.
   - When you use `INTERFACE`, the property is only visible to the consumers of the target, not the target itself.
   - Example:
     ```cmake
     target_include_directories(my_target INTERFACE include_dir)
     ```
     In this case, the `include_dir` directory is not used for compiling `my_target`, but is exposed to other targets that link against `my_target`.

The choice between `PRIVATE`, `PUBLIC`, and `INTERFACE` depends on the intended usage of the target and its dependencies. Here are some general guidelines:

- Use `PRIVATE` when the property is only relevant to the target itself and should not be exposed to its consumers.
- Use `PUBLIC` when the property is important for both the target and its consumers.
- Use `INTERFACE` when the property is only relevant to the consumers of the target and not the target itself.

## Examples

By using these keywords, you can better control the visibility and usage requirements of your CMake targets, making your build system more modular, maintainable, and easier to understand.

`PRIVATE` and `PUBLIC` in `target_include_directories` control how include directories are propagated to dependent targets. Here's a practical explanation:

1. **PRIVATE**:
- Include directories are only used by the target itself
- NOT propagated to targets that link against this target
```cmake
add_library(mylib src/lib.cpp)
target_include_directories(mylib PRIVATE include)  # Only mylib can use these headers
```

2. **PUBLIC**:
- Include directories are used by both the target AND any target that links against it
- Propagates to dependent targets
```cmake
add_library(mylib src/lib.cpp)
target_include_directories(mylib PUBLIC include)  # mylib and anyone using mylib can use these headers
```

3. **INTERFACE** (not in your question but related):
- Include directories are only used by targets that link against this target
- NOT used by the target itself

Real example showing the difference:
```cmake
# Library with public headers
add_library(math_lib src/math.cpp include/math.h)
target_include_directories(math_lib PUBLIC include)  # math.h is public API

# Library with internal headers
add_library(utils src/utils.cpp internal/utils.h)
target_include_directories(utils PRIVATE internal)  # utils.h is internal

# Main executable
add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE math_lib utils)

# Result:
# - app can include math.h (because it's PUBLIC in math_lib)
# - app cannot include utils.h (because it's PRIVATE in utils)
# - math_lib and utils can include their own headers
```