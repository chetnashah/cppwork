# Context variables

# Frequently-Used “Context” Variables in CMake

The variables below are **generated automatically** by CMake every time it parses a script.  
They tell you *where you are* in the directory tree or in the file that is being processed so you can build reliable, path-independent code.

| Variable | What it contains (at configure time) | Most common use-cases | Mini example |
|-----------|--------------------------------------|-----------------------|--------------|
| `CMAKE_SOURCE_DIR` | Absolute path to the **top-level** source directory (the one that contained the first `cmake` call). | • Set global output paths<br>• Find helper files relative to the project root | `set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")` |
| `CMAKE_BINARY_DIR` | Absolute path to the **top-level** build (binary) directory. | • Locate generated artefacts from other sub-projects | `install(FILES ${CMAKE_BINARY_DIR}/changelog.txt DESTINATION share/doc)` |
| `PROJECT_SOURCE_DIR` | Source dir of the project declared by the *nearest* `project()` call. | • Projects inside `add_subdirectory()` that want to be self-contained | In `src/foo/CMakeLists.txt`:<br>`configure_file(${PROJECT_SOURCE_DIR}/config.h.in config.h)` |
| `PROJECT_BINARY_DIR` | Build dir that corresponds to `PROJECT_SOURCE_DIR`. | • Same as above, but for generated files | `target_include_directories(foo PRIVATE ${PROJECT_BINARY_DIR})` |
| `CMAKE_CURRENT_SOURCE_DIR` | Source dir that contains the **CMakeLists.txt currently executing**. | • Add sources relative to the file you’re editing | `file(GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")` |
| `CMAKE_CURRENT_BINARY_DIR` | Build dir that corresponds to `CMAKE_CURRENT_SOURCE_DIR`. | • Write generated files next to the object files | `add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/version.cpp …)` |
| `CMAKE_CURRENT_LIST_DIR` | Directory of the script that is *currently included* (`CMakeLists.txt` **or** a `.cmake` file). | • Inside reusable modules to locate sibling resources without assuming an install location. | `include(${CMAKE_CURRENT_LIST_DIR}/GenerateVersion.cmake)` |
| `CMAKE_CURRENT_LIST_FILE` | Full path of the file that is being parsed. | • Debugging or producing helpful error messages. | `message(STATUS "Reading ${CMAKE_CURRENT_LIST_FILE}")` |
| `CMAKE_CURRENT_LIST_LINE` | 1-based line number that is being executed inside `CMAKE_CURRENT_LIST_FILE`. | • Pin-pointing problems in meta-programming helpers. | `message(FATAL_ERROR "Wrong arguments (at ${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE})")` |
| `CMAKE_PARENT_LIST_FILE` | Path of the list file **that included** the current one (`include()`, `find_package()`, …). | • Trace include chains or compute paths relative to the parent. | `message(VERBOSE "Included from ${CMAKE_PARENT_LIST_FILE}")` |

---

## Key Relationships Illustrated

```
project/
├── CMakeLists.txt          ← CMAKE_SOURCE_DIR  /  PROJECT_SOURCE_DIR  /  CMAKE_CURRENT_SOURCE_DIR *
├── build/                  ← CMAKE_BINARY_DIR  /  PROJECT_BINARY_DIR  /  CMAKE_CURRENT_BINARY_DIR *
│
└── lib/
    ├── CMakeLists.txt      ← when parsing this: CMAKE_CURRENT_SOURCE_DIR ≠ CMAKE_SOURCE_DIR
    └── cmake/
        └── MyMacros.cmake  ← inside here: CMAKE_CURRENT_LIST_DIR points **here**
```
\* while *top-level* `CMakeLists.txt` is executing.

---

## Practical Patterns

### 1. Shipping helper modules with your project

```cmake
# project_root/cmake/BoostWarnings.cmake
if(TARGET Boost::boost)
  target_compile_definitions(Boost::boost PRIVATE BOOST_ALL_NO_LIB)
endif()

# project_root/CMakeLists.txt
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(BoostWarnings)   # will resolve because of CMAKE_CURRENT_LIST_DIR
```

`BoostWarnings.cmake` can now include other files relative to itself with  
`include(${CMAKE_CURRENT_LIST_DIR}/SomeOtherHelper.cmake)`.

---

### 2. Generating files next to object files (out-of-source safe)

```cmake
# lib/CMakeLists.txt
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/git_hash.h
  COMMAND git rev-parse --short HEAD > git_hash.h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_custom_target(gen_git DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/git_hash.h)
```

Whether the user builds in `build/`, `out/build/` or any IDE-specific folder, the header lands in the correct place.

---

### 3. Installing resources found relative to the project root

```cmake
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data DESTINATION share/myapp)
```

---

### 4. Defensive error reporting inside functions/macros

```cmake
function(require_var _name)
  if(NOT DEFINED ${_name})
    message(FATAL_ERROR
      "Variable '${_name}' must be set "
      "(called at ${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE})")
  endif()
endfunction()
```

---

## Cheat-Sheet: Which one to reach for?

* Global path (independent of current subdir) → `CMAKE_SOURCE_DIR` / `CMAKE_BINARY_DIR`
* Path of *this* project (`project()` scope) → `PROJECT_SOURCE_DIR` / `PROJECT_BINARY_DIR`
* Path of the *file* you’re editing → `CMAKE_CURRENT_SOURCE_DIR` / `CMAKE_CURRENT_BINARY_DIR`
* Path of the *module* you just `include()`d → `CMAKE_CURRENT_LIST_DIR`

Memorise these four groups and you’ll very rarely have to hard-code a path again.

# Frequently-used `cmake` Cache Variables

These variables are most often passed from the shell with `-D<var>=<value>` when you invoke the configure step (`cmake -S … -B …`).  
All of them live in the CMake *cache*, so they are remembered between runs.

| Variable | What it controls | Typical values | Practical CLI example |
|----------|-----------------|----------------|-----------------------|
| `CMAKE_BUILD_TYPE` | Single-configuration build mode (Makefiles, Ninja) | `Debug`, `Release`, `RelWithDebInfo`, `MinSizeRel` | `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release` |
| `CMAKE_INSTALL_PREFIX` | Where `cmake --install` copies the artefacts | `/usr/local`, `$HOME/sw`, `C:/SDK` | `cmake -S . -B build -DCMAKE_INSTALL_PREFIX=$HOME/sw` |
| `CMAKE_C_COMPILER`, `CMAKE_CXX_COMPILER` | Override the compiler executable | `/usr/bin/clang`, `/opt/gcc-13/bin/g++` | `cmake -S . -B build -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++` |
| `CMAKE_TOOLCHAIN_FILE` | Path to a *toolchain* file (cross, special flags, etc.) | `cmake/toolchains/arm-none-eabi.cmake` | `cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/arm-none-eabi.cmake` |
| `BUILD_SHARED_LIBS` | Ask project targets without an explicit `STATIC/SHARED` to default to shared libs | `ON`/`OFF` | `cmake -S . -B build -DBUILD_SHARED_LIBS=ON` |
| `CMAKE_INSTALL_BINDIR`, `LIBDIR`, `DATAROOTDIR`, … | GNU-style sub-directory names | `bin`, `lib64`, `share` | `cmake -S. -Bbuild -DCMAKE_INSTALL_LIBDIR=lib64` |
| `CMAKE_PREFIX_PATH` | Search path seed for `find_package()` | `"/opt/qt6;/opt/vtk"` (semicolon-separated) | `cmake -S. -Bbuild -DCMAKE_PREFIX_PATH="/opt/qt6;/opt/vtk"` |
| `CMAKE_<LANG>_FLAGS`, `CMAKE_<LANG>_FLAGS_<CONFIG>` | Global extra compiler flags | `-Wall -Wextra`, `-O3` | `cmake -S. -Bbuild -DCMAKE_CXX_FLAGS=-Werror` |
| `CMAKE_GENERATOR` | Chosen generator when none is given through `-G` | `Ninja`, `Unix Makefiles`, `Xcode` | `cmake -S. -Bbuild -DCMAKE_GENERATOR=Ninja` |
| `CMAKE_EXPORT_COMPILE_COMMANDS` | Emits a `compile_commands.json` file | `ON` | `cmake -S. -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=ON` |
| `BUILD_TESTING` (from `CTest`) | Enable or skip compiling tests | `ON`/`OFF` | `cmake -S. -Bbuild -DBUILD_TESTING=OFF` |
| Project-specific opts | Anything defined in your own `option()` calls | e.g. `MYLIB_WITH_CUDA=ON` | `cmake -S. -Bbuild -DMYLIB_WITH_CUDA=ON` |

> Tip: use `cmake -LH build` to print *all* cached variables with current values.

---

## Putting those variables in a toolchain file

A *toolchain file* is just another CMake script that runs **before** your project’s `project()` call.  
Typical cross-compile example (`cmake/toolchains/arm-none-eabi.cmake`):

```cmake
# Target triple and compilers
set(CMAKE_SYSTEM_NAME        Generic)
set(CMAKE_SYSTEM_PROCESSOR   cortex-m4)

set(CMAKE_C_COMPILER   arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# Sysroot & search paths
set(CMAKE_SYSROOT       "/opt/arm-none-eabi")
set(CMAKE_FIND_ROOT_PATH "/opt/arm-none-eabi")

# Default build flags
set(CMAKE_C_FLAGS_INIT   "-mcpu=cortex-m4 -mthumb")
set(CMAKE_CXX_FLAGS_INIT "-mcpu=cortex-m4 -mthumb")

# Don’t look in host paths
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
```

Use it from the CLI:

```bash
cmake -S . -B build/arm -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/arm-none-eabi.cmake \
      -DCMAKE_BUILD_TYPE=Release
cmake --build build/arm
```

---

## Moving command-line options to **CMakePresets.json**

Since CMake 3.19, *presets* give you reproducible, check-in-able command lines.

```jsonc
{
  "version": 5,
  "configurePresets": [
    {
      "name": "default-debug",
      "displayName": "Debug / Ninja",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/out/build/debug",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON",
        "BUILD_TESTING": "ON"
      }
    },
    {
      "name": "release-install",
      "displayName": "Release + install prefix",
      "inherits": "default-debug",
      "binaryDir": "${sourceDir}/out/build/release",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",
        "CMAKE_INSTALL_PREFIX": "${sourceDir}/out/install"
      }
    },
    {
      "name": "arm-release",
      "displayName": "Cross-compile ARM Release",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/out/build/arm-release",
      "toolchainFile": "cmake/toolchains/arm-none-eabi.cmake",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",
        "BUILD_SHARED_LIBS": "OFF"
      }
    }
  ],

  "buildPresets": [
    { "name": "default", "configurePreset": "default-debug" },
    { "name": "release", "configurePreset": "release-install" },
    { "name": "arm",     "configurePreset": "arm-release" }
  ],

  "testPresets": [
    { "name": "default", "configurePreset": "default-debug" }
  ]
}
```

How to use:

```bash
# Configure exactly like the preset describes
cmake --preset arm-release        # or: cmake --preset default-debug

# Build or test using their build/test presets
cmake --build --preset arm
ctest --preset default
```

Advantages:

1. No one has to remember long `-D…` spells.  
2. IDEs (VS Code, CLion, Visual Studio) show the presets graphically.  
3. `--clean` rebuilds are a single command: `cmake --build --preset release --clean-first`.

---

### Cheatsheet: When to choose what

• Ad-hoc one-off compilation ➜ `cmake -D…` on the command line.  
• Same options every time for a **host** build ➜ put them in a *configurePreset*.  
• Cross-compile or special SDK ➜ write a *toolchain file* and reference it from the preset.  
• Project options that users may toggle ➜ provide `option()` defaults, document them, let users override with either `-D` or presets.

Now your CMake invocations can shrink from:

```bash
cmake -S . -B build -GNinja -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DCMAKE_INSTALL_PREFIX=$HOME/sw -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      -DBUILD_TESTING=OFF -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/arm-none-eabi.cmake
```

to the far friendlier:

```bash
cmake --preset arm-release
```


## CMAKE_SOURCE_DIR vs PROJECT_SOURCE_DIR

## `CMAKE_SOURCE_DIR` vs `PROJECT_SOURCE_DIR`

| Variable | What it is | Lifetime | Typical consumer |
|----------|------------|----------|------------------|
| `CMAKE_SOURCE_DIR` | The *absolute* path to the **top-level directory that the very first `cmake` command was pointed at**. | Exists from the first line of the top‐level `CMakeLists.txt` to the end of the run; same value in every sub-directory and in every included `.cmake` file. | Global settings that must be identical for the whole build: output locations, formatting tools, top-level docs, etc. |
| `PROJECT_SOURCE_DIR` | The *absolute* path to the source directory of the **nearest `project()` command currently in effect**. | Re-evaluated every time CMake starts processing a file that belongs to a different project scope. | Sub-project logic that should stay self-contained: unit-test data, private headers, generated code, etc. |

If a CMake tree contains only one `project()` call (the most common case), the two variables have identical values, so many people never notice the distinction—until they start nesting projects.

---

### Directory layout example

```
repo/
├── CMakeLists.txt          ← project("Host")        (A)
├── tools/
│   └── CMakeLists.txt      ← add_subdirectory()     (still A)
└── firmware/
    ├── CMakeLists.txt      ← add_subdirectory()
    └── board/
        └── CMakeLists.txt  ← project("BoardFW")     (B)
```

While parsing each file …

| current file | `CMAKE_SOURCE_DIR` | `PROJECT_SOURCE_DIR` |
|--------------|--------------------|----------------------|
| `repo/CMakeLists.txt` | `repo` | `repo`            |
| `repo/tools/CMakeLists.txt` | `repo` | `repo`      |
| `repo/firmware/CMakeLists.txt` | `repo` | `repo`   |
| `repo/firmware/board/CMakeLists.txt` | `repo` | `repo/firmware/board` |

`BoardFW` is its *own* project. Inside it, paths like

```cmake
configure_file(${PROJECT_SOURCE_DIR}/cfg.h.in cfg.h)
```

resolve to `repo/firmware/board/cfg.h.in` even though the overall build started in `repo/`.  
Yet a command such as

```cmake
file(WRITE ${CMAKE_SOURCE_DIR}/build-info.txt "Commit: ${GIT_SHA}")
```

still reaches out to the root of the entire repository.

---

### When to choose which variable

1. **Use `CMAKE_SOURCE_DIR`**  
   • Anything that should be visible or shared globally.  
   • Example: installing top-level documentation, defining a formatting target, creating a configure summary.

2. **Use `PROJECT_SOURCE_DIR`**  
   • Whenever you are writing code that could be pulled into another tree with `add_subdirectory()` and is expected to work out-of-the-box.  
   • Keeps a module self-contained and relocatable.

---

### Minimal hands-on demo

```cmake
# top/CMakeLists.txt
cmake_minimum_required(VERSION 3.21)
project(Top)
message(STATUS "[top] CMAKE_SOURCE_DIR   = ${CMAKE_SOURCE_DIR}")
message(STATUS "[top] PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")

add_subdirectory(sub)

# top/sub/CMakeLists.txt
project(SubLib)                 # new project scope!
message(STATUS "[sub] CMAKE_SOURCE_DIR   = ${CMAKE_SOURCE_DIR}")
message(STATUS "[sub] PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
```

Configure:

```
cmake -S top -B build
```

Output:

```
[top] CMAKE_SOURCE_DIR   = /abs/path/top
[top] PROJECT_SOURCE_DIR = /abs/path/top
[sub] CMAKE_SOURCE_DIR   = /abs/path/top
[sub] PROJECT_SOURCE_DIR = /abs/path/top/sub
```

The behaviour is identical for the binary-directory counterparts `CMAKE_BINARY_DIR` and `PROJECT_BINARY_DIR`.

Keeping the distinction clear will save you a lot of headaches when you start publishing CMake packages, writing reusable modules, or cross-building firmware next to host utilities in the same repository.H'/R58.˚8K 