# Understanding `CMAKE_PREFIX_PATH`

`CMAKE_PREFIX_PATH` is the *universal hint list* that tells CMake **where to start looking** for external software:  
headers, libraries, executables *and* CMake package config files.  
If you master only one search-related variable, make it this one.

---

## 1. What exactly is it?

* Type   : semicolon-separated list of absolute paths  
* Scope  : normal CMake cache variable (can be set from the CLI, env-var, or `set()`)  
* Purpose : Acts as an *extra* set of “install prefixes” that CMake prepends to its built-in search paths.

```
/opt/mystuff        ─┬─ include/
                    ├─ lib/
                    └─ lib/cmake/Foo/FooConfig.cmake
```

If you push `/opt/mystuff` to `CMAKE_PREFIX_PATH`, CMake will look underneath it exactly as if it were `/usr` or `C:\Program Files`.

quickly check it:
```sh
# Show current value
cmake --system-information | grep 'PREFIX_PATH'
```

---

## 2. Which commands use it?

`CMAKE_PREFIX_PATH` is consulted by every *find* command:

```
find_package()      # FooConfig.cmake, FindFoo.cmake
find_library()
find_path()
find_file()
find_program()
```

Internally these commands iterate over “prefixes” in this order:

1. HINTS / PATHS arguments passed explicitly in the call  
2. `CMAKE_PREFIX_PATH`  ← **you are here**  
3. Toolchain / system prefixes (`CMAKE_SYSTEM_PREFIX_PATH`, `/usr`, `C:\Program Files\…`, …)

For each prefix it checks typical sub-folders:

```
${prefix}/lib        ${prefix}/lib64
${prefix}/include    ${prefix}/cmake
${prefix}/share      ${prefix}/bin …
```

---

## 3. Impact on `find_package()`

### 3.1 Config mode  
If a directory in `CMAKE_PREFIX_PATH` contains …/cmake/Foo/FooConfig.cmake, then

```cmake
find_package(Foo CONFIG REQUIRED)
```

will pick that copy first, create imported targets such as `Foo::Foo`, and later  
`target_link_libraries(app PRIVATE Foo::Foo)` will link to libraries **inside the same prefix**.  
CMake additionally auto-generates the correct run-time RPATH for ELF/Mach-O binaries.

### 3.2 Module mode  
With

```cmake
find_package(Foo)      # falls back to FindFoo.cmake
```

`CMAKE_PREFIX_PATH` is still used: the variables you set in the Find module (`FOO_LIBRARY`, `FOO_INCLUDE_DIR`, …) are searched beneath every prefix you listed.

---

## 4. Relation to other variables

| Variable | Typical use | Interaction |
|----------|-------------|-------------|
| `CMAKE_INSTALL_PREFIX` | Where **your** project installs *to* | Independent; does *not* influence searches |
| `CMAKE_FIND_ROOT_PATH` | Toolchain root when cross-compiling | Consulted **before** `CMAKE_PREFIX_PATH` |
| `CMAKE_SYSTEM_PREFIX_PATH` | Built-in defaults (`/usr;/usr/local`) | Searched **after** `CMAKE_PREFIX_PATH` |
| `PKG_CONFIG_PATH` | Only pkg-config | Ignored unless Find module calls `pkg-config` |

---

## 5. How to set it

1. Command line (most common):

```bash
cmake -DCMAKE_PREFIX_PATH=/opt/mystuff;/home/me/myconda ..
```

2. Environment variable (`cmake` picks it up automatically):

```bash
export CMAKE_PREFIX_PATH=/opt/Qt/6.7.0/gcc_64:$HOME/vcpkg/installed/x64-linux
```

3. Inside a `CMakeLists.txt` (last resort):

```cmake
list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/third_party/install")
```

---

## 6. Practical scenarios

### 6.1 Home-brew, vcpkg, Conan, Conda …

| Package manager | What you usually set |
|-----------------|----------------------|
| Homebrew on mac | `-DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)` |
| vcpkg (classic) | `-DCMAKE_TOOLCHAIN_FILE=$VCPKG/scripts/buildsystems/vcpkg.cmake` (toolchain already **injects** its prefix path) |
| Conda env       | `export CMAKE_PREFIX_PATH=$CONDA_PREFIX` |

### 6.2 Finding a locally built dependency

```
~/dev/mylibs/install/include/myfoo.h
                           /lib/libmyfoo.a
                           /lib/cmake/MyFoo/MyFooConfig.cmake
```

```bash
cmake -DCMAKE_PREFIX_PATH=$HOME/dev/mylibs/install ..
```

No custom Find modules, no extra variables—life is good.

### 6.3 Override a system library

You need OpenSSL 3 instead of the system’s 1.1:

```bash
/opt/openssl-3.3
├─ include/openssl
├─ lib/libcrypto.so
└─ lib/cmake/OpenSSL/OpenSSLConfig.cmake
```

```bash
cmake -DCMAKE_PREFIX_PATH=/opt/openssl-3.3 ..
```

Every `find_package(OpenSSL REQUIRED)` in the dependency graph will resolve to 3.3 and propagate secure compile options.

### 6.4 Cross-compiling

Toolchain file:

```cmake
set(CMAKE_SYSTEM_NAME      Linux)
set(CMAKE_SYSROOT          /opt/aarch64-sysroot)
set(CMAKE_FIND_ROOT_PATH   /opt/aarch64-sysroot)

# Add extra SDK on top
set(CMAKE_PREFIX_PATH      /opt/qt6-aarch64)
```

First the root path is scanned, then Qt’s prefix; host paths are skipped.

---

## 7. Influence on Linking & RPATH

1. Imported targets created from a *Config.cmake* store absolute locations:  
   `${_prefix}/lib/libfoo.so`.
2. When you later link to that target, CMake automatically:
   • Adds the library to the link line  
   • Extends `INTERFACE_INCLUDE_DIRECTORIES` with `${_prefix}/include`  
   • Generates an `rpath` equal to `${_prefix}/lib` (unless you disable it)

Therefore one well-chosen `CMAKE_PREFIX_PATH` value keeps include paths, link paths and runtime search paths coherent.

---

## 8. Common pitfalls

* Forgetting that it is **semicolon-separated** on every platform:
  `"/opt/one;/opt/two"`, not `"/opt/one:/opt/two"`.
* Accidental **overwrite** instead of append:

```bash
cmake -DCMAKE_PREFIX_PATH=/some/path           # good
cmake -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH};/extra  # wrong in Bash
```

* Setting it inside a sub-directory after some `find_package()` calls have already run—too late.
* Mixing host & target prefixes when cross-compiling.

---

## 9. Quick cheat-sheet

```sh
# Show current value
cmake --system-information | grep CMAKE_PREFIX_PATH

# Append a path (CLI, POSIX shell)
cmake -DCMAKE_PREFIX_PATH="$(pwd)/install;$CMAKE_PREFIX_PATH" ..

# Typical pattern in CMakeLists when vendoring dependencies
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/_deps/zlib-install")
```

---

### TL;DR

`CMAKE_PREFIX_PATH` is the **one-stop variable** to teach CMake about *additional* installation roots.  
Set it early, list every prefix you care about, and both `find_package()` and the other `find_*` commands will resolve, link and rpath your targets correctly. Happy hunting!