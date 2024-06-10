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

By using these keywords, you can better control the visibility and usage requirements of your CMake targets, making your build system more modular, maintainable, and easier to understand.