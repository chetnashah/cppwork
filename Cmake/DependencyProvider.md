# CMake *dependency provider* – a hook into find_package

1. `find_package()` is the usual CMake command a project calls to obtain a
   dependency.

2. A *dependency provider* is a small, **pluggable piece of CMake code that
   gets the first shot at satisfying a `find_package()` request**.  
   • If it can supply the package, it does so and tells CMake “I’ve got it – stop
   searching”.  
   • If it cannot, CMake falls back to its normal search on disk
   (`CMAKE_PREFIX_PATH`, `/usr/lib/cmake`, …).

3. The mechanism was added in CMake 3.24 to give package-managers
   (vcpkg, Conan, CPM, Hunter, FetchContent, your own scripts, …) a *formal*
   way to hook into `find_package()` instead of relying on ad-hoc tricks.

---

## How it works (conceptually)

1. **Register a provider** – put the path to a CMake file in  
   `CMAKE_DEPENDENCY_PROVIDERS` (or set it in a toolchain file, preset, cache
   entry, …).

2. **Implement one function** in that file:

```cmake
# Signature fixed by CMake
function(cmake_dependency_provider package_name version_range result_var)
  # … decide whether you can supply 'package_name' …
  if(package_name STREQUAL "fmt")      # <— example
      # 1. Obtain the package (download, build, install, etc.)
      include(FetchContent)
      FetchContent_Declare(fmt
          GIT_REPOSITORY https://github.com/fmtlib/fmt.git
          GIT_TAG        10.2.1)
      FetchContent_MakeAvailable(fmt)

      # 2. Make the IMPORTED targets that callers expect
      #    (FetchContent already did it for fmt)

      # 3. Tell CMake the provider succeeded
      set(${result_var} TRUE PARENT_SCOPE)
      return()
  endif()

  # Couldn’t (or didn’t want to) provide it → let CMake fall back
  set(${result_var} FALSE PARENT_SCOPE)
endfunction()
```

3. **During configure** each `find_package(<Pkg> …)` makes CMake call every
   registered provider in order.  
   • The first provider that returns `TRUE` short-circuits the search.  
   • If all providers return `FALSE`, CMake resumes its ordinary search logic.

---

## Minimal concrete example (FetchContent as provider)

```
project(demo)

# 1. Register the provider script (could also be cached / preset / toolchain)
list(APPEND CMAKE_DEPENDENCY_PROVIDERS
     ${CMAKE_CURRENT_LIST_DIR}/cmake/fetchcontent_provider.cmake)

# 2. The usual call – looks no different to your project
find_package(fmt CONFIG REQUIRED)

add_executable(demo main.cpp)
target_link_libraries(demo PRIVATE fmt::fmt)
```

`cmake/fetchcontent_provider.cmake` is exactly the function shown above.  
The first configure run:

```
-- Fetching fmt...
--  - downloading...
--  - configuring...
--  - building...
-- Dependency provider supplied fmt 10.2.1
```

Subsequent configures find `fmt` immediately because the imported targets now
live inside the build tree.

---

## Real-world providers you may already use

• **vcpkg** – its toolchain file registers a provider that installs the port
  on-the-fly and then exposes the `::` targets.  
• **Conan 2’s `conan_toolchain.cmake`** – same idea but calls `conan install`
  to build the recipe.  
• **CPM.cmake** – can be switched to provider mode so `find_package(fmt)`
  triggers `CPMAddPackage(fmtlib/fmt@10.2.1)`.

---

## Why it matters

1. **Uniform CMake code in projects** – you keep writing `find_package()` no
   matter which dependency manager a user chooses.  
2. **No more INCLUDE hacks** – provider is an officially documented hook.  
3. **Opt-in** – users that do not need the provider just do nothing and CMake
   behaves exactly as before.

---

### TL;DR

A *dependency provider* is a plug-in that lets you teach CMake how to obtain a
package **before** the regular `find_package()` machinery runs.  
It is the clean, first-class mechanism that package managers now use to make
CMake projects “just work” without the project itself caring where the
dependencies really come from.