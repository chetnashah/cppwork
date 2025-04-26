CMake's package system is a powerful feature that helps you manage dependencies in a CMake-based project. It allows you to find and use external libraries and packages in a consistent and portable manner. Here’s an explanation of how it works, particularly focusing on the `find_package` command:

###

Okay, let's break down the roles and differences between CMake's `find_package` and `WorkspaceContent` facilities in detail. Both are mechanisms for handling external dependencies in your CMake project, but they operate very differently.

**`find_package`**

* **Role:** To **locate** libraries, packages, and development tools that are **already installed** on the system or within the build environment. It *finds* things that exist *outside* your project's source tree.
* **Mechanism:**
    1.  **Search:** When you call `find_package(SomeLib REQUIRED)`, CMake searches various predefined locations for evidence of `SomeLib`. These locations include standard system installation directories (`/usr/lib`, `/usr/local/lib`, etc.), CMake's own module directory, paths specified by environment variables (`CMAKE_PREFIX_PATH`), and paths provided by the user (`-DCMAKE_PREFIX_PATH=...`).
    2.  **Config/Find Modules:** CMake looks for specific files to understand how to use the found package:
        * **`<PackageName>Config.cmake` or `<lowercase-packagename>-config.cmake` (Config-Mode):** This is the *modern and preferred* way. Well-behaved libraries install these files when they are built/installed. They typically define **imported targets** (e.g., `SomeLib::SomeLib`) which encapsulate usage requirements like include directories, library paths, and compile definitions.
        * **`Find<PackageName>.cmake` (Module-Mode):** This is the older approach. CMake provides many `Find*.cmake` modules for common libraries (like `FindBoost.cmake`, `FindThreads.cmake`). You can also write your own. These modules typically perform more complex searching (checking registry keys on Windows, running helper programs, looking for specific headers/libraries) and usually set variables (e.g., `SomeLib_INCLUDE_DIRS`, `SomeLib_LIBRARIES`) rather than imported targets, although modern Find modules often define imported targets too.
* **Outcome:**
    * If successful, `find_package` makes the dependency available, ideally by defining **imported targets**. You then use these targets with `target_link_libraries(myexe PRIVATE SomeLib::SomeLib)`. CMake automatically applies the necessary include directories, linker flags, compile definitions etc., associated with the imported target.
    * If using older `Find*.cmake` modules, it might just set variables (e.g., `SomeLib_FOUND`, `SomeLib_INCLUDE_DIRS`, `SomeLib_LIBRARIES`) that you need to manually use with `target_include_directories` and `target_link_libraries`.
    * If the package is marked `REQUIRED` and not found, CMake stops configuration with an error. If not `REQUIRED`, the `SomeLib_FOUND` variable (or equivalent) will be false, and your CMake code needs to handle this case.
* **What it *doesn't* do:** `find_package` does *not* download, build, or install the dependency for you. It assumes the dependency is already present and correctly installed.

**`WorkspaceContent`**

* **Role:** To **download** (fetch) the source code of a dependency and **integrate its build** directly into your project's build process. It makes the dependency a *part* of your build tree.
* **Mechanism:** It's typically a two-stage process:
    1.  **`WorkspaceContent_Declare(<name> ...)`:** You declare *how* to get the dependency's source code. This usually involves specifying a source location like a Git repository (`GIT_REPOSITORY`, `GIT_TAG`), a URL to an archive (`URL`, `URL_HASH`), or potentially others. This step only records the information; it doesn't download anything yet.
    2.  **`WorkspaceContent_MakeAvailable(<name>)` (or `WorkspaceContent_GetProperties` + `WorkspaceContent_Populate`):** This command triggers the actual download (if not already populated) and integration.
        * **Download/Populate:** It fetches the source code from the specified location into a subdirectory within your build directory (`_deps/<name>-src` by default).
        * **`add_subdirectory()`:** Crucially, `WorkspaceContent_MakeAvailable` then calls `add_subdirectory()` on the populated source directory (`_deps/<name>-src`) and a corresponding binary directory (`_deps/<name>-build`). This processes the dependency's *own* `CMakeLists.txt` file.
* **Outcome:**
    * Because the dependency's `CMakeLists.txt` is processed via `add_subdirectory`, any targets (libraries, executables) defined by the dependency become **native targets** within your main build.
    * You can then directly use these targets, for example: `target_link_libraries(myexe PRIVATE dependency_target_name)`.
    * The dependency is built using the *exact same* compiler, flags, configuration (Debug/Release), C++ standard, and other settings as your main project.
* **What it *doesn't* inherently do:** `WorkspaceContent` doesn't look for pre-existing installations on the system. It always fetches the source (unless already populated from a previous run).

**Key Differences Summarized:**

| Feature                  | `find_package`                                      | `WorkspaceContent`                                              |
| :----------------------- | :-------------------------------------------------- | :---------------------------------------------------------- |
| **Primary Action** | Locates existing installation                       | Downloads and builds source code                            |
| **Dependency Source** | External (pre-installed on system/env)              | Defined source (URL, Git repo, etc.)                        |
| **Build Integration** | Uses Imported Targets or variables from install     | Builds dependency via `add_subdirectory`                    |
| **Build Configuration** | Dependency built separately (potential mismatch)    | Dependency built with *identical* configuration to parent |
| **Version Control** | Uses whatever version is installed                  | Can pin specific version (commit, tag, URL hash)            |
| **Requires Installation**| Yes, dependency must be installed beforehand       | No, only build tools (CMake, compiler, Git/download tool) |
| **Build Time Impact** | Config time (if found); No build time for dep       | Slower initial configure/build (download + build dep)     |
| **Self-Contained Build** | No, relies on external environment                  | Yes, build is more self-contained                           |
| **Typical Use Case** | System libs (Boost, OpenSSL), Package manager installs | Header-only libs, specific versions needed, build consistency |

**When to Use Which?**

* **Use `find_package` when:**
    * You want to use standard libraries expected to be present on the system (e.g., Zlib, OpenSSL, Boost).
    * You are relying on a system package manager (apt, yum, brew, vcpkg in manifest mode, Conan) to provide dependencies.
    * The dependency is large and recompiling it often is undesirable.
    * You trust that users/CI environments will have the correct version installed.
* **Use `WorkspaceContent` when:**
    * You need a very specific version of a dependency (pinning a commit hash).
    * You need absolute certainty that the dependency is built with the *exact same* compiler, flags, and configuration (Debug/Release, C++ standard, CRT on Windows) as your project. This is crucial for avoiding subtle ABI compatibility issues.
    * The dependency is header-only (FetchContent just makes headers available easily).
    * You want your project build to be highly self-contained and reproducible across different machines without relying on pre-installed libraries.
    * The dependency doesn't provide good CMake config-files for `find_package`.
* **Hybrid Approach:** You can sometimes try `find_package` first, and if it fails, fall back to `WorkspaceContent`.

```cmake
find_package(MyLib QUIET) # Try to find it quietly
if(NOT MyLib_FOUND)
  FetchContent_Declare(MyLib ...)
  FetchContent_MakeAvailable(MyLib)
  # Note: You might need to map the FetchContent target name
  # if it differs from the expected find_package target/variables.
else()
  # Use the found package (e.g., MyLib::MyLib)
endif()
```

In modern CMake, `WorkspaceContent` is increasingly popular for managing application dependencies due to the reproducibility and build consistency it offers, while `find_package` remains essential for interacting with the system environment and larger, pre-compiled libraries.

### Overview of CMake's Package System

CMake's package system revolves around finding, configuring, and using external libraries or packages. It relies heavily on the `find_package` command to locate and load the necessary package configuration files. These files can be either provided by the package itself or created manually.

### `find_package` Command

The `find_package` command is used to locate and configure external libraries or packages. Here’s a basic syntax:

```cmake
find_package(<PackageName> [version] [REQUIRED] [COMPONENTS components...])
```

- `<PackageName>`: The name of the package you want to find.
- `[version]`: (Optional) The version of the package you need.
- `[REQUIRED]`: (Optional) If specified, CMake will produce an error if the package is not found.
- `[COMPONENTS components...]`: (Optional) Specific components of the package you want to find.

### Types of Package Configurations

1. **Config-file Packages**:
    - These are provided by the package maintainers and usually come with the package itself.
    - They contain detailed configuration information, including paths to libraries, include directories, and other relevant settings.
    - Typically named `PackageNameConfig.cmake` or `package-name-config.cmake`.

2. **Find-module Packages**:
    - These are custom modules written to find and configure packages when a config-file is not available.
    - They follow a naming convention like `FindPackageName.cmake`.
    - CMake provides some built-in find-modules for common libraries.

### Search Procedure

When you call `find_package`, CMake searches for the package in several locations:

1. **Package Registry**: CMake maintains a registry of installed packages.
2. **CMAKE_PREFIX_PATH**: A list of directories where CMake will look for packages.
3. **System Paths**: Typical system directories like `/usr/local`, `/usr`, etc.

### Example Usage

Here’s a practical example to illustrate how you might use `find_package` in a `CMakeLists.txt` file:

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Find the Boost library
find_package(Boost 1.65 REQUIRED COMPONENTS filesystem system)

# Check if Boost was found
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable(MyExecutable main.cpp)
  target_link_libraries(MyExecutable ${Boost_LIBRARIES})
else()
  message(FATAL_ERROR "Boost not found!")
endif()
```

In this example:
- We search for the Boost library, specifically version 1.65 or higher.
- We specify that the `filesystem` and `system` components are required.
- If Boost is found, we include its directories and link the required libraries to our executable.
- If Boost is not found, we generate a fatal error message.

### Custom Find Modules

Sometimes, you may need to write your own find module if one doesn’t exist. Here’s a simple example of a custom `FindFoo.cmake`:

```cmake
# FindFoo.cmake - Custom find module for Foo library

find_path(FOO_INCLUDE_DIR foo.h PATHS /usr/local/foo/include /opt/foo/include)
find_library(FOO_LIBRARY NAMES foo PATHS /usr/local/foo/lib /opt/foo/lib)

if(FOO_INCLUDE_DIR AND FOO_LIBRARY)
  set(FOO_FOUND TRUE)
  set(FOO_LIBRARIES ${FOO_LIBRARY})
  set(FOO_INCLUDE_DIRS ${FOO_INCLUDE_DIR})
else()
  set(FOO_FOUND FALSE)
endif()

mark_as_advanced(FOO_INCLUDE_DIR FOO_LIBRARY)
```

In this module:
- We define paths to search for the `foo.h` header and `foo` library.
- If both are found, we set `FOO_FOUND` to `TRUE` and define variables for include directories and libraries.
- If not found, we set `FOO_FOUND` to `FALSE`.

### Conclusion

CMake's package system and the `find_package` command provide a robust mechanism for managing dependencies. By leveraging configuration files and find-modules, you can ensure your project is portable and can easily incorporate external libraries.


## Write your own package config file

1. **Define Targets**: Create targets for your library.
2. **Export Targets**: Use the export command to export the targets.
3. **Install Exported Targets**: Use the install command to install the exported targets and generate the configuration files.
