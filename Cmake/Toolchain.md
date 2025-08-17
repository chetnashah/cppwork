
## What is a toolchain `.cmake` file?

Mainly sets up environment e.g. `sdk`, `compiler`, `c++ version` etc.

This is just an ordinary CMake script that typically contains
mostly `set(…)` commands. These would define the variables that CMake uses to describe the target
platform, the location of the various toolchain components, and so on.

## toolchain vs presets

1. A preset is a recipe for running CMake; it can reference a toolchain file.
2. A toolchain file is the recipe for building on a particular platform; it is referenced by the preset.
3. Swapping presets is a user-level convenience; swapping toolchains is an environment-level change.

## Toolchain file

# CMake *toolchain file* explained

A **toolchain file** is simply a CMake script that is read **before** the first
`project()` (or `enable_language()`) command.  
It is the place where you describe *how* CMake should build the project:

* Which compiler to invoke?  
* Which “host” platform are we producing binaries for?  
* Where is the SDK / sys-root / libraries?  
* Which dependency provider(s) should be active?

You pass the file on the command line or in a configure preset:

```bash
cmake -S . -B build \
      -DCMAKE_TOOLCHAIN_FILE=/path/to/my-toolchain.cmake
# or
cmake --preset linux-clang
```

Because the file executes **before tool-chain detection is frozen**, the
variables you set there become the authoritative description of the build
environment.

---

## 1. Anatomy of a toolchain file

```cmake
# my-toolchain.cmake
# ------------------------------------------------------
# 1) Tell CMake what *target* platform we intend to build for
set(CMAKE_SYSTEM_NAME       Linux)              # Windows, Darwin, Generic, …
set(CMAKE_SYSTEM_PROCESSOR  x86_64)             # armv7, aarch64, riscv32…

# 2) Name the compilers (absolute or just a prog name if on PATH)
set(CMAKE_C_COMPILER   clang)
set(CMAKE_CXX_COMPILER clang++)

# 3) Optional: where headers and libs for that target live
set(CMAKE_SYSROOT       /opt/sysroots/my-sysroot)
set(CMAKE_FIND_ROOT_PATH /opt/sysroots/my-sysroot)

# 4) Optional: pass extra flags for every file
set(CMAKE_C_FLAGS_INIT   "-O2 -pipe")
set(CMAKE_CXX_FLAGS_INIT "-O2 -pipe")

# 5) Optional: register a dependency provider
list(APPEND CMAKE_DEPENDENCY_PROVIDERS
     ${CMAKE_CURRENT_LIST_DIR}/fetchcontent_provider.cmake)
```

Important rules:

1. *Use `set()` **without** `CACHE`* – the values go into the cache later
   automatically.
2. Do **not** call `project()` in the toolchain file.
3. You can execute any CMake commands, include other `.cmake` files, etc.

---

## 2. Minimal native-build example

You only need a toolchain when you want **non-default** behaviour, e.g. force a
different compiler than the system default:

```cmake
# gcc12.cmake — build with GCC 12 installed in /opt/gcc-12
set(CMAKE_C_COMPILER   /opt/gcc-12/bin/gcc)
set(CMAKE_CXX_COMPILER /opt/gcc-12/bin/g++)
```

Invoke:

```bash
cmake -S . -B build-gcc12 -DCMAKE_TOOLCHAIN_FILE=gcc12.cmake
```

CMake now detects include/lib paths relative to `/opt/gcc-12`.

---

## 3. Cross-compiling for embedded ARM

```cmake
# arm-none-eabi.cmake
set(CMAKE_SYSTEM_NAME        Generic)           # “bare metal”
set(CMAKE_SYSTEM_PROCESSOR   cortex-m4)

# toolchain executables
set(CMAKE_C_COMPILER   arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

# where headers/libs come from
set(ARM_SDK              /opt/gcc-arm-none-eabi-10-2020-q4-major)
set(CMAKE_SYSROOT        ${ARM_SDK}/arm-none-eabi)
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})

# flags typical for bare-metal
set(CPU_OPTS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
set(CMAKE_C_FLAGS_INIT   "${CPU_OPTS} -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS_INIT "${CPU_OPTS} -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-Wl,--gc-sections -T${CMAKE_SOURCE_DIR}/link.ld")
```

Result:

```
$ cmake -S . -B build-arm -DCMAKE_TOOLCHAIN_FILE=arm-none-eabi.cmake
-- The C compiler identification is GNU 10.2.1
-- Check for working C compiler: /usr/bin/arm-none-eabi-gcc … OK
-- Target system: Generic / cortex-m4
```

Now the same CMakeLists can produce both native and embedded firmware simply by
switching presets.

---

## 4. Toolchain file for Apple iOS

```cmake
# ios.cmake
set(CMAKE_SYSTEM_NAME          iOS)
set(CMAKE_OSX_ARCHITECTURES    arm64)
set(CMAKE_OSX_SYSROOT          iphoneos)

# Xcode is usually the generator of choice
set(CMAKE_GENERATOR "Xcode")

# Optional: sign / minimum version
set(CMAKE_OSX_DEPLOYMENT_TARGET 15.0)
```

Invoke:

```bash
cmake -S . -B build-ios -DCMAKE_TOOLCHAIN_FILE=ios.cmake
cmake --build build-ios --config Release
```

---

## 5. Using presets to hide the `-D` flag

`CMakePresets.json` (CMake ≥ 3.23):

```jsonc
{
  "configurePresets": [
    {
      "name": "linux-clang",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build-clang",
      "toolchainFile": "toolchains/clang.cmake",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "RelWithDebInfo"
      }
    }
  ]
}
```

User experience:

```
cmake --preset linux-clang
cmake --build --preset linux-clang
```

---

## 6. Frequently used variables in a toolchain

| Variable | Purpose |
|----------|---------|
| `CMAKE_SYSTEM_NAME` | Tells CMake the *target* OS; triggers cross-compile mode when different from host. |
| `CMAKE_SYSTEM_PROCESSOR` | Target CPU (x86_64, aarch64, armv7, …). |
| `CMAKE_C_COMPILER`, `CMAKE_CXX_COMPILER`, … | Full path or executable name for each language. |
| `CMAKE_SYSROOT` | Root directory that mimics the target file-system hierarchy. |
| `CMAKE_FIND_ROOT_PATH` | List of root paths that `find_package()` / `find_library()` must prefix when searching. |
| `CMAKE_TRY_COMPILE_TARGET_TYPE` | Set to `STATIC_LIBRARY` for embedded builds to avoid test executables. |
| `CMAKE_DEPENDENCY_PROVIDERS` | Registers provider scripts (vcpkg, Conan, custom). |

---

## 7. Relation to dependency providers

A toolchain file is the *ideal* place to register provider code because:

* The path is known before any `find_package()` runs.
* Users can swap package managers by swapping toolchains.

Example: enable [vcpkg] for every `find_package()` automatically

```cmake
# vcpkg-toolchain.cmake
set(CMAKE_SYSTEM_NAME Linux)
# tell CMake to include vcpkg’s toolchain *after* ours
set(CMAKE_TOOLCHAIN_FILE_INIT 1)  # internal guard
include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
```

---

## 8. Golden rules

1. A toolchain should **describe the environment**, not the project.  
   Put per-project options in the normal cache or presets.
2. Keep separate files for each distinct target platform; do **not** try to
   toggle dozens of options inside one huge script.
3. Remember it is just CMake code—use `if()`, `find_program()` and even
   `execute_process()` when necessary.

---

### TL;DR

* A **toolchain file** is an early-executed CMake script that defines the
  compilers, sys-root, flags, search paths and even dependency providers.
* Activate it with `-DCMAKE_TOOLCHAIN_FILE=…` or via configure presets.
* By swapping toolchain files you can build the identical source tree for
  desktop, mobile, embedded, containerised or *any* other environment without
  touching the project’s own `CMakeLists.txt`.