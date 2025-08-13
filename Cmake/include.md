# `include()` in CMake – Is for running `.cmake` scripts

`include()` is one of the most frequently used CMake commands, yet it is often confused with other “add something” commands.  
At its core, **`include()` simply tells CMake to execute another CMake script at processing time**.  
Everything else— where the script comes from, whether it is optional, and what it produces—depends on the arguments you give.

Below you will find succinct, self-contained sections that cover every-day patterns and some advanced tricks.

---

## 1. Basic Syntax

```cmake
include(<file|module>  [OPTIONAL]  [RESULT_VARIABLE <var>]  [NO_POLICY_SCOPE])
```

• `<file|module>` – Either a path to a `.cmake` file or the name of a *module* that CMake can find on `CMAKE_MODULE_PATH`.  
• `OPTIONAL` – Do not error out if the file cannot be found.  
• `RESULT_VARIABLE <var>` – A cache/local variable that receives `TRUE` on success or an error string on failure.  
• `NO_POLICY_SCOPE` – Keep policy settings from leaking into/out of the included script.

---

## 2. Including a Local Script

### Example tree

```
CMakeLists.txt
cmake/
 └─ MyMacros.cmake
```

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.30)
project(Demo)

include(cmake/MyMacros.cmake)   # executes that file
add_executable(app main.cpp)
```

Anything defined in `MyMacros.cmake` (functions, variables, macros) becomes visible in the parent file *after* the `include()` line.

---

## 3. Using Modules Provided by CMake

CMake ships many helper modules (e.g. `CheckCXXCompilerFlag`, `FindThreads`, `ExternalProject`).

```cmake
include(CheckCXXCompilerFlag)     # no .cmake extension needed
check_cxx_compiler_flag("-march=native" HAS_NATIVE)
```

CMake looks for `CheckCXXCompilerFlag.cmake` inside its own `Modules/` directory, then inside each directory listed in `CMAKE_MODULE_PATH`.

---

## 4. Augmenting the Search Path

```cmake
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(MyCompanyMacros) # now found in our custom folder
```

The pattern above keeps the system modules available **after** your own entries, avoiding accidental shadowing.

---

## 5. Handling Optional Includes

When supporting multiple toolchains or optional dependencies you often want *graceful degradation*.

```cmake
include(Toolchain/AppleClang OPTIONAL RESULT_VARIABLE inc_ok)

if(NOT inc_ok)
  message(STATUS "AppleClang extras not found – falling back to generic flags")
endif()
```

`RESULT_VARIABLE` receives “NOTFOUND” if the file is missing (or an explanatory message on other failures).

---

## 6. Difference from `add_subdirectory()`

| Aspect                 | `include()`                       | `add_subdirectory()`                       |
|------------------------|-----------------------------------|--------------------------------------------|
| Input                  | A single `.cmake` script          | Another CMake project (needs `CMakeLists.txt`) |
| Build directories      | None created                      | Generates its own binary dir               |
| Targets inside appear  | Immediately (in current dir)      | Namespaced in their own dir                |
| Intended purpose       | Re-usable macros / checks         | Nested build trees (libraries, programs)   |

---

## 7. Relation to `include_directories()`

These commands have unrelated purposes:

* `include()` – run a script.  
* `include_directories()` – add header search paths for the compiler.

---

## 8. Guarding Against Double Inclusion

CMake provides a tiny helper inside every script file:

```cmake
include_guard(GLOBAL) # or DIRECTORY or FILE
```

Put this at the top of `MyMacros.cmake`; the script will silently return if it was already processed.

---

## 9. Managing Policies with `NO_POLICY_SCOPE`

Normally each `include()` creates a **new policy scope** so policy settings inside the child do not affect the parent.

If you actually *want* a setting to bleed through (rare), turn that off:

```cmake
include(SomeOldModule NO_POLICY_SCOPE)
```

---

## 10. Practical Recipe – Feature Probe Library

Suppose you need a lot of feature tests that clutter `CMakeLists.txt`.

`cmake/FeatureChecks.cmake`

```cmake
include_guard(GLOBAL)

include(CheckIncludeFileCXX)   # ships with CMake
include(CheckCXXSourceCompiles)

function(check_and_add feature hdr macro)
    check_include_file_cxx(${hdr} HAVE_${macro})
    if(HAVE_${macro})
        target_compile_definitions(myapp PRIVATE ${feature})
    endif()
endfunction()

check_and_add(USE_SPAN "span" SPAN_HEADER)
check_and_add(USE_FMT  "fmt/core.h" FMT_HEADER)
```

Top-level `CMakeLists.txt`

```cmake
include(cmake/FeatureChecks.cmake)
add_executable(myapp main.cpp)
```

The result is a clean top-level file while all probing logic lives elsewhere.

---

## 11. Debugging: Where Did CMake Look?

```cmake
include(NonExistent OPTIONAL RESULT_VARIABLE rv)
message(STATUS "include result = ${rv}")
```

Run with `cmake --trace-expand` to see the search order and failed paths.

---

## 12. Takeaways

1. `include()` executes another CMake script in the current context.  
2. It can pull in local helper files or built-in/third-party modules.  
3. Use `OPTIONAL` and `RESULT_VARIABLE` for graceful fallbacks.  
4. It is **not** for adding source sub-trees—that’s `add_subdirectory()`.  
5. Combine with `include_guard()` to avoid accidental double execution.

With these patterns you can structure even large CMake projects into tidy, reusable script modules.