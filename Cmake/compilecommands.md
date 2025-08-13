# What `set(CMAKE_EXPORT_COMPILE_COMMANDS ON)` Does

When you add  
```cmake
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```
to your CMakeLists.txt, you instruct CMake to produce a file named **compile_commands.json** in your build directory. This JSON file lists every translation unit (source file) in your project along with the exact compiler invocation (compiler path, flags, include paths, macros, etc.) that CMake will use to build it.

## Why It’s Useful

1. **Editor/IDE Integration**  
   Many code editors and IDEs (Visual Studio Code, Qt Creator, CLion, Sublime Text with plugins) can read compile_commands.json to provide:
   - Accurate code completion
   - Jump-to-definition
   - On-the-fly diagnostics
2. **Static Analysis & Linters**  
   Tools like clang-tidy or clang-d rely on compile commands to know exactly how you build each file:
   ```bash
   clang-tidy src/foo.cpp --export-fixes=fixes.yaml
   ```
3. **Cross-Platform Consistency**  
   generate the same compile commands on Linux, Windows (via Ninja or Make), or macOS.

## How to Enable It

### Minimal Example

CMakeLists.txt:
```cmake
cmake_minimum_required(VERSION 3.5)
project(MyApp CXX)

# Turn on export of compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(MyApp
    src/main.cpp
    src/foo.cpp
    src/foo.h
)
```

Usage:
```bash
mkdir build
cd build
cmake -G Ninja ..
# compile_commands.json appears in build/
ninja
```

### Integrating with clang-tidy

1. Enable compile commands:
   ```cmake
   set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
   ```
2. Invoke clang-tidy via CMake:
   ```cmake
   find_program(CLANG_TIDY clang-tidy)
   if(CLANG_TIDY)
     set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY};-checks=*")
   endif()
   ```
3. Now building runs clang-tidy checks on each file:
   ```bash
   cd build
   cmake ..
   ninja
   ```

### Using Bear for Legacy Projects

If you can’t modify CMake but still want compile_commands.json, you can use [Bear](https://github.com/rizsotto/Bear):
```bash
mkdir build && cd build
bear -- cmake .. && bear -- ninja
# or
bear -- make
```
Bear records compiler calls and emits compile_commands.json for any Make-based build.

## Practical Workflow in VS Code

1. Enable in CMakeLists.txt:
   ```cmake
   set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
   ```
   Creates `build/compile_commands.json`
2. In **.vscode/settings.json**:
   ```json
   {
     "C_Cpp.default.compileCommands": "${workspaceFolder}/build/compile_commands.json"
   }
   ```
3. Open the project in VS Code; IntelliSense now uses the exact flags to parse your code.

***

**In summary**, `set(CMAKE_EXPORT_COMPILE_COMMANDS ON)` tells CMake to output a detailed JSON manifest of how each file is compiled, unlocking rich editor integration and advanced tooling support without manual flag management.