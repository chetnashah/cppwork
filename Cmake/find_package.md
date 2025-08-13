https://stackoverflow.com/questions/35815850/getting-imported-targets-through-find-package

find_package is a two-headed beast these days:

CMake provides direct support for two forms of packages:
1. Config-file Packages and 
2. Find-module Packages

*Thinking...*

> **Unveiling CMake Paths**
> 
> I'm now focusing on methods for extracting CMake's default search paths. Initially, I'll try to get CMake itself to print these paths directly. After that, I'll hunt for internal variables that hold the paths for packages, libraries, and executables. I'll also check the command line for any helpful options or scripts. I anticipate a clear picture emerging shortly.
> 
> 
> **Exploring Path Retrieval Strategies**
> 
> I'm now delving deeper into direct approaches. My current focus is on using command-line options to directly obtain the path information from CMake. I'm examining the various flags available. If direct options fail, I'll explore the use of auxiliary scripts within CMake to reveal the underlying paths. I anticipate this phase will provide more concrete results.
> 
> 
> **Unpacking CMake's Paths**
> 
> I've been digging into how CMake figures out its default search paths. It's clear that it's a multifaceted process. The `find_*` command, the OS, and various variables all play a role. I've begun to uncover the different factors at play, but there is a great deal more to explore.
> 
> 
> **Charting the Search Orders**
> 
> I'm now structuring my response by breaking down the `find_*` commands individually. I'm delving deeper into the nuances, especially the distinction between Module and Config mode for `find_package`. I'll highlight the role of environment variables like `PATH` and how they influence the process. I am planning to include examples to illustrate the concepts, making it clearer for the user.
> 
> 
> **Structuring the Response**
> 
> I've synthesized the research and have a clear plan. The structure will begin with an overview of CMake's search mechanisms and crucial variables. After that, I will provide a practical method for the user to see the paths, which is the most direct help. The following sections will detail the search orders for `find_package`, `find_library`, and `find_program`. I plan to add some code examples and clear headings for improved comprehension. I'm nearly done with the first draft.
> 
> 
Determining the default paths used by CMake is a multi-faceted question because the search paths depend on the specific command being used (e.g., `find_package`, `find_library`, `find_program`), the operating system environment, and numerous CMake variables.

Here is a comprehensive guide to understanding, printing, and controlling the default search paths in CMake.

### The Most Direct Method: Debugging the `find` Commands

The most effective way to see the exact paths CMake is searching for a specific `find_*` command is to use the debugging options introduced in CMake 3.17.

You can enable this debugging output in two ways:

1.  **Using the command-line option `--debug-find`**:
    This will print detailed information for all `find_*` calls during the CMake run. [[1]](https://stackoverflow.com/questions/54293737/how-do-i-tell-cmake-to-output-the-package-search-paths)

    ```bash
    cmake --debug-find -S . -B build
    ```

2.  **Setting the `CMAKE_FIND_DEBUG_MODE` variable**:
    You can set this variable in your `CMakeLists.txt` or pass it via the command line. This provides the same detailed output as the command-line flag. [[1]](https://stackoverflow.com/questions/54293737/how-do-i-tell-cmake-to-output-the-package-search-paths)

    ```bash
    # Pass via command line
    cmake -DCMAKE_FIND_DEBUG_MODE=1 -S . -B build

    # Or, set in CMakeLists.txt for temporary debugging
    set(CMAKE_FIND_DEBUG_MODE 1)
    find_package(MyPackage REQUIRED)
    set(CMAKE_FIND_DEBUG_MODE 0) # Turn off when no longer needed
    ```

The output from these options will show you the exact locations being probed for the package or library you are trying to find, making it the most reliable way to debug path issues.

---


###

`Find-module` packages are the ones you are probably most familiar with. They execute a script of CMake code (such as this one) that does a bunch of calls to functions like find_library and find_path to figure out where to locate a library.

The big advantage of this approach is that it is extremely generic. As long as there is something on the filesystem, we can find it. The big downside is that it often provides little more information than the physical location of that something. That is, the result of a find-module operation is typically just a bunch of filesystem paths. This means that modelling stuff like transitive dependencies or multiple build configurations is rather difficult.

This becomes especially painful if the thing you are trying to find has itself been built with CMake. In that case, you already have a bunch of stuff modeled in your build scripts, which you now need to painstakingly reconstruct for the find script, so that it becomes available to downstream projects.

This is where `config-file` packages shine. Unlike find-modules, the result of running the script is not just a bunch of paths, but it instead creates fully functional CMake targets. To the dependent project it looks like the dependencies have been built as part of that same project.

This allows to transport much more information in a very convenient way. The obvious downside is that config-file scripts are much more complex than find-scripts. Hence you do not want to write them yourself, but have CMake generate them for you. Or rather have the dependency provide a config-file as part of its deployment which you can then simply load with a find_package call. And that is exactly what Qt5 does.

This also means, if your own project is a library, consider generating a config file as part of the build process. It's not the most straightforward feature of CMake, but the results are pretty powerful.

Here is a quick comparison of how the two approaches typically look like in CMake code:

### Find-module style
```
find_package(foo)
target_link_libraries(bar ${FOO_LIBRARIES})
target_include_directories(bar ${FOO_INCLUDE_DIR})
# [...] potentially lots of other stuff that has to be set manually
```

### Config-file style
```
find_package(foo)
target_link_libraries(bar foo)
```

Okay, let's break down CMake's `find_package` command in detail, starting from first principles.

**1. The Fundamental Problem: Managing Dependencies**

Software projects rarely exist in isolation. They often depend on external libraries or tools (e.g., Boost, Qt, zlib, OpenSSL, a specific compiler tool). When you build your project, the build system (in this case, CMake) needs to:

1.  **Locate:** Find where the necessary dependency files (headers, libraries, executables) are installed on the system. This is complex because they could be in standard system locations, user-defined locations, or bundled with a specific SDK.
2.  **Configure:** Determine how to use the dependency. This includes:
    *   Which header directories to add to the include path (`-I` flag for compilers).
    *   Which library files to link against (`-l` or full path for linkers).
    *   Any necessary preprocessor definitions (`-D` flag).
    *   Sometimes, finding executable tools provided by the dependency.
3.  **Version Check:** Ensure the found dependency meets the version requirements of your project.

Manually managing this for every dependency across different operating systems and build environments would be incredibly tedious and error-prone.

**2. `find_package`: CMake's Solution**

CMake provides the `find_package()` command as a standardized mechanism to handle this dependency management process. Its core purpose is to search for an external package (library or toolset) and load its usage requirements (include directories, link libraries, definitions, etc.) into your CMake project, making them available to your targets (executables, libraries).

```cmake
find_package(<PackageName> [version] [EXACT] [QUIET] [MODULE]
             [REQUIRED] [[COMPONENTS] [components...]]
             [OPTIONAL_COMPONENTS components...]
             [NO_POLICY_SCOPE]
             [CONFIG] # CMake 3.1+
             [PATHS path1 [path2 ...]]
             [HINTS path1 [path2 ...]]
             [NAMES name1 [name2 ...]] # CMake 3.1+
             [CONFIGS config1 [config2 ...]] # CMake 3.1+
             [BYPASS_PROVIDER] # CMake 3.24+
             [REQUIRED_VARS var1 [var2 ...]] # CMake 3.26+
             [VERSION_VAR var] # CMake 3.26+
            )
```

At its heart, `find_package` tries to locate and execute a specific CMake script file that contains the logic for finding and setting up the requested package. There are two primary ways it does this, leading to the two main "modes": **Module Mode** and **Config Mode**.

**3. Module Mode (`Find<PackageName>.cmake`)**

*   **Concept:** Relies on a CMake script file named `Find<PackageName>.cmake`. This script is responsible for actively searching for the package's files (headers, libraries) and setting specific CMake variables or imported targets.
*   **Who Provides the Script?** These `Find<...>.cmake` modules can come from:
    1.  **CMake Itself:** CMake ships with find modules for many common libraries (e.g., `FindBoost.cmake`, `FindThreads.cmake`, `FindZLIB.cmake`).
    2.  **Your Project:** You can write your own `Find<...>.cmake` script for a dependency that doesn't have one provided by CMake or the package itself, and place it in a location CMake searches.
    3.  **Third-Party Collections:** Sometimes other frameworks or projects provide collections of find modules.
*   **Where CMake Looks for `Find<PackageName>.cmake`:** CMake searches for these files in the directories listed in the `CMAKE_MODULE_PATH` variable, followed by the CMake installation's own module directory. You typically add your project-specific find modules to a `cmake/` directory within your source tree and add that path to `CMAKE_MODULE_PATH`:
    ```cmake
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    find_package(MyLegacyLib REQUIRED)
    ```
*   **Implementation (Writing a `Find<PackageName>.cmake` Module):**
    *   **Goal:** Find the necessary components (headers, libraries, executables) and report back.
    *   **Key Commands:**
        *   `find_path()`: Searches for a directory containing a specific header file.
        *   `find_library()`: Searches for a library file.
        *   `find_program()`: Searches for an executable program.
        *   `find_package_handle_standard_args()`: A helper function to handle standard arguments like `REQUIRED`, `QUIET`, and `VERSION`, and to set the standard `<PackageName>_FOUND` variable based on whether required variables (like include dirs and libraries) were successfully populated.
        *   `pkg_check_modules()` (from `FindPkgConfig`): Often used internally to leverage the `pkg-config` utility if available.
    *   **Output Variables (Traditional):** The script traditionally sets variables like:
        *   `<PackageName>_FOUND`: Set to `TRUE` if the package was found, `FALSE` otherwise.
        *   `<PackageName>_INCLUDE_DIRS`: Directory(s) containing header files.
        *   `<PackageName>_LIBRARIES`: Library file(s) to link against.
        *   `<PackageName>_DEFINITIONS`: Compiler definitions required.
        *   `<PackageName>_VERSION`: The detected version string (if version checking is implemented).
    *   **Output Targets (Modern):** Modern `Find` modules *should* define `IMPORTED` targets (e.g., `PackageName::LibraryName`). These targets encapsulate all usage requirements (include directories, link libraries, definitions, transitive dependencies). This is much cleaner than using variables.
    *   **Example (`FindSimpleLib.cmake`):**
        ```cmake
        # FindSimpleLib.cmake - Try to find SimpleLib

        # 1. Search for the header
        find_path(SimpleLib_INCLUDE_DIR simplelib.h
            HINTS /opt/simplelib/include /usr/local/include
            PATH_SUFFIXES simplelib) # Look in simplelib subdirs too

        # 2. Search for the library
        find_library(SimpleLib_LIBRARY NAMES simple simplelib libsimple
            HINTS /opt/simplelib/lib /usr/local/lib)

        # 3. Handle version (if applicable - simplified here)
        if(SimpleLib_INCLUDE_DIR AND EXISTS "${SimpleLib_INCLUDE_DIR}/simplelib_version.h")
            file(STRINGS "${SimpleLib_INCLUDE_DIR}/simplelib_version.h" SimpleLib_VERSION_STRING REGEX "^#define SIMPLELIB_VERSION \"[^\"]+\"")
            string(REGEX MATCH "[0-9]+\\.[0-9]+(\\.[0-9]+)?" SimpleLib_VERSION "${SimpleLib_VERSION_STRING}")
        else()
            set(SimpleLib_VERSION "0.0.0") # Or leave unset if version cannot be determined
        endif()

        # 4. Use the standard handling function
        include(FindPackageHandleStandardArgs)
        find_package_handle_standard_args(SimpleLib
            FOUND_VAR SimpleLib_FOUND # Sets SimpleLib_FOUND
            REQUIRED_VARS SimpleLib_LIBRARY SimpleLib_INCLUDE_DIR # Must be found for success
            VERSION_VAR SimpleLib_VERSION # Handles version check against find_package(SimpleLib [version])
        )

        # 5. Set output variables (traditional) and/or create imported target (modern)
        if(SimpleLib_FOUND)
            set(SimpleLib_LIBRARIES ${SimpleLib_LIBRARY})
            set(SimpleLib_INCLUDE_DIRS ${SimpleLib_INCLUDE_DIR})

            # Modern approach: Define an imported target
            if(NOT TARGET SimpleLib::SimpleLib)
                add_library(SimpleLib::SimpleLib UNKNOWN IMPORTED) # UNKNOWN is okay for non-shared/static distinction if not easily known
                set_target_properties(SimpleLib::SimpleLib PROPERTIES
                    IMPORTED_LOCATION "${SimpleLib_LIBRARY}"
                    INTERFACE_INCLUDE_DIRECTORIES "${SimpleLib_INCLUDE_DIR}"
                    # Add INTERFACE_COMPILE_DEFINITIONS if needed
                )
            endif()
        endif()

        # 6. Hide internal variables from CMake GUI/ccmake
        mark_as_advanced(SimpleLib_INCLUDE_DIR SimpleLib_LIBRARY)
        ```
*   **Consumption (Using a Module Mode Package):**
    ```cmake
    cmake_minimum_required(VERSION 3.15)
    project(MyApp)

    # Tell CMake where to find our custom FindSimpleLib.cmake if it's not built-in
    # list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

    # Find the package, require version 1.2 or higher
    find_package(SimpleLib 1.2 REQUIRED)

    add_executable(my_app main.cpp)

    # --- Option 1: Using Variables (Older style) ---
    # if(SimpleLib_FOUND) # Check is redundant if REQUIRED is used
    #   target_include_directories(my_app PRIVATE ${SimpleLib_INCLUDE_DIRS})
    #   target_link_libraries(my_app PRIVATE ${SimpleLib_LIBRARIES})
    #   # target_compile_definitions(my_app PRIVATE ${SimpleLib_DEFINITIONS})
    # else()
    #   message(FATAL_ERROR "SimpleLib not found but is required!")
    # endif()

    # --- Option 2: Using Imported Target (Modern style - Preferred!) ---
    # The FindSimpleLib.cmake must define the SimpleLib::SimpleLib target for this to work
    target_link_libraries(my_app PRIVATE SimpleLib::SimpleLib)
    # Usage requirements (includes, defines) are automatically propagated from the target

    ```

**4. Config Mode (`<PackageName>Config.cmake` or `<lowercase-package-name>-config.cmake`)**

*   **Concept:** Relies on the *package itself* providing configuration files during its installation process. These files tell CMake exactly where the installed components are and how to use them. This is the **preferred, modern approach** because the package author knows best how their package is structured and should be used.
*   **Who Provides the Script?** The package developer, as part of their own CMake build and install process.
*   **Where CMake Looks for `<PackageName>Config.cmake`:** The search procedure is more complex and designed to find installed packages. CMake searches in various locations, roughly in this order (simplified):
    1.  Specific hints: Directories listed in the `<PackageName>_DIR` CMake variable (if set by the user or another find script).
    2.  Search paths: Paths specified in `CMAKE_PREFIX_PATH`, `CMAKE_FRAMEWORK_PATH`, `CMAKE_APPBUNDLE_PATH` environment variables or CMake variables.
    3.  System paths: Standard installation prefixes on the system (e.g., `/usr/local/`, `/usr/`, `/opt/`).
    CMake looks for specific subdirectories within these paths, such as:
    *   `<prefix>/lib/cmake/<PackageName>/`
    *   `<prefix>/share/cmake/<PackageName>/`
    *   `<prefix>/<PackageName>/cmake/`
    *   `<prefix>/lib/<arch>/cmake/<PackageName>/`
    *   ... and variations using lowercase names and `-config.cmake` suffixes.
*   **Implementation (Making Your Package Provide Config Files):**
    *   **Goal:** Install files that `find_package(YourPackage)` can locate and use, defining imported targets for your installed library/executable.
    *   **Key Commands:**
        *   `include(CMakePackageConfigHelpers)`: Provides helper functions.
        *   `configure_package_config_file()`: Generates the `<PackageName>Config.cmake` file from an input template (`.cmake.in`), handling path relocatability.
        *   `write_basic_package_version_file()`: Generates the `<PackageName>ConfigVersion.cmake` file from an input template (`.cmake.in`) to handle version checks.
        *   `install(TARGETS ... EXPORT ...)`: Installs targets (libraries, executables) and associates them with an "export set".
        *   `install(EXPORT <export-set> ...)`: Installs a CMake script (`<PackageName>Targets.cmake`) that contains the definitions of the exported targets (as `IMPORTED` targets). This file is typically included by `<PackageName>Config.cmake`.
        *   `install(FILES ...)`: Installs the generated `Config.cmake` and `ConfigVersion.cmake` files.
    *   **Example (`SuperLib` Project):**
        *   `SuperLib/CMakeLists.txt`:
            ```cmake
            cmake_minimum_required(VERSION 3.15)
            project(SuperLib VERSION 2.1.0 LANGUAGES CXX)

            add_library(superlib src/superlib.cpp)
            # Assume PUBLIC headers are installed to include/${PROJECT_NAME}
            target_include_directories(superlib PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # Needed during build
                $<INSTALL_INTERFACE:include> # Specifies the relative path after installation
            )
            # Set properties like C++ standard, etc.
            set_target_properties(superlib PROPERTIES PUBLIC_HEADER "include/superlib.h")

            # --- Installation ---
            include(GNUInstallDirs) # Provides standard install path variables (CMAKE_INSTALL_LIBDIR, CMAKE_INSTALL_INCLUDEDIR etc)
            include(CMakePackageConfigHelpers)

            # 1. Install Headers and Library Target
            install(TARGETS superlib
                EXPORT SuperLibTargets # Associate with an export set named SuperLibTargets
                LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # lib or lib64
                ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # for static libs
                RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # for DLLs on Windows
                PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME} # include/SuperLib
            )

            # 2. Generate and Install SuperLibTargets.cmake (contains imported target definitions)
            # This file will define SuperLib::superlib as an IMPORTED target
            install(EXPORT SuperLibTargets
                FILE SuperLibTargets.cmake # The file name to generate
                NAMESPACE SuperLib::       # Prepend SuperLib:: to target names (e.g., superlib becomes SuperLib::superlib)
                DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} # Install location: <prefix>/lib/cmake/SuperLib/
            )

            # 3. Generate SuperLibConfigVersion.cmake
            write_basic_package_version_file(
                "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
                VERSION ${PROJECT_VERSION}
                COMPATIBILITY AnyNewerVersion # Or SameMajorVersion, ExactVersion etc.
            )

            # 4. Generate SuperLibConfig.cmake from a template
            configure_package_config_file(
                "${CMAKE_CURRENT_SOURCE_DIR}/cmake/SuperLibConfig.cmake.in" # Input template
                "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"   # Output file
                INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} # Install location
                PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR # Make these paths relative to install prefix if possible
            )

            # 5. Install the generated Config and Version files
            install(FILES
                "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
                DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} # Install location
            )
            ```
        *   `SuperLib/cmake/SuperLibConfig.cmake.in`:
            ```cmake
            # SuperLibConfig.cmake.in - Config file template for SuperLib

            @PACKAGE_INIT@ # Essential boilerplate provided by configure_package_config_file

            # Include the targets file generated by install(EXPORT ...)
            # It defines the SuperLib::superlib imported target.
            # The path is relative to this file's installed location.
            include("${CMAKE_CURRENT_LIST_DIR}/SuperLibTargets.cmake")

            # Check components (if any were requested by find_package)
            # check_required_components(SuperLib) # Use if you have COMPONENTS
            ```
*   **Consumption (Using a Config Mode Package):**
    ```cmake
    cmake_minimum_required(VERSION 3.15)
    project(MyConsumerApp)

    # Find the package. CMake searches standard install locations.
    # You might need to hint CMake if SuperLib is installed in a non-standard prefix:
    # CMAKE_PREFIX_PATH="/path/to/superlib/install" cmake ..
    # Or set SuperLib_DIR=/path/to/superlib/install/lib/cmake/SuperLib
    find_package(SuperLib 2.0 REQUIRED) # Require version 2.0 or compatible newer (based on AnyNewerVersion)

    add_executable(my_consumer_app main.cpp)

    # Link against the IMPORTED target provided by SuperLib's config files.
    # This automatically handles include directories, definitions, and transitive dependencies.
    target_link_libraries(my_consumer_app PRIVATE SuperLib::superlib)
    ```

**5. How `find_package` Chooses Between Modes**

By default, `find_package` first searches using **Module Mode**. If a `Find<PackageName>.cmake` file is found in `CMAKE_MODULE_PATH` or the CMake installation, it's used.

If no module is found, it then searches using **Config Mode**, looking for `<PackageName>Config.cmake` or `<lowercase-package-name>-config.cmake`.

You can influence this behavior:

*   `find_package(<PackageName> MODULE ...)`: Forces Module Mode search only.
*   `find_package(<PackageName> CONFIG ...)`: (CMake 3.1+) Forces Config Mode search only.
*   `set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)`: (CMake 3.15+) Tells `find_package` to try Config Mode *before* Module Mode for subsequent calls in the current scope and below. This is useful if a package provides both, and you prefer the (usually more accurate) Config mode files.

**6. Key Arguments Explained**

*   `<PackageName>`: The name of the package (case-sensitive, usually matching `Find<PackageName>.cmake` or `<PackageName>Config.cmake`).
*   `[version]`: Optional minimum version required (e.g., `1.2.3`).
*   `[EXACT]`: Requires the exact version specified.
*   `[QUIET]`: Suppresses messages printed by the find module/config file (but not FATAL_ERROR messages if `REQUIRED` is set and it's not found).
*   `[REQUIRED]`: If the package is not found, CMake stops processing with a FATAL_ERROR. If omitted, CMake continues, but the `<PackageName>_FOUND` variable will be false, and any imported targets won't be defined.
*   `[COMPONENTS ...]` / `[OPTIONAL_COMPONENTS ...]` : For packages that have distinct parts (e.g., `find_package(Qt5 COMPONENTS Core Gui Widgets)`). The find/config script must specifically handle component logic. `REQUIRED` applies to `COMPONENTS`, but not `OPTIONAL_COMPONENTS`.
*   `[CONFIG]` / `[MODULE]`: As explained above, forces a specific search mode.
*   `[PATHS path1 ...]` / `[HINTS path1 ...]`: Provide specific directories to search *first* (HINTS) or *instead of* standard system paths (PATHS - use with care). These primarily affect the search for the root of the package installation in Config mode or specific files (`find_path`, `find_library`) in Module mode.

**7. Modern CMake Best Practices**

1.  **Prefer Config Mode:** If you are developing a library, *always* provide `<PackageName>Config.cmake` and `<PackageName>ConfigVersion.cmake` files and install them correctly using `install(EXPORT ...)` etc. This is the most robust and maintainable way for others to use your library.
2.  **Use Imported Targets:** Both when *implementing* find/config scripts and when *consuming* packages, use imported targets (`Namespace::TargetName`). They encapsulate all usage requirements (includes, libraries, definitions, transitive dependencies) cleanly. Avoid using the old `<PackageName>_LIBRARIES`, `<PackageName>_INCLUDE_DIRS` variables directly with `target_link_libraries` and `target_include_directories` if an imported target is available.
3.  **Namespace Targets:** Use namespaces (`PackageName::`) for imported targets defined in Config or Find modules to avoid name collisions.
4.  **Handle Dependencies Transitively:** Imported targets excel at this. If `SuperLib` depends on `ZLIB`, its `SuperLib::superlib` imported target should list `ZLIB::ZLIB` (or similar) in its `INTERFACE_LINK_LIBRARIES`. When a consumer links to `SuperLib::superlib`, CMake automatically knows it also needs to link to `ZLIB::ZLIB`.

In summary, `find_package` is a cornerstone of CMake, providing a powerful and flexible mechanism for locating and integrating external dependencies. Understanding the difference between Module and Config modes, and embracing the modern practices of Config Mode and imported targets, is crucial for writing robust and maintainable CMake-based projects.