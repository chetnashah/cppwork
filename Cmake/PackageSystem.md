CMake's package system is a powerful feature that helps you manage dependencies in a CMake-based project. It allows you to find and use external libraries and packages in a consistent and portable manner. Here’s an explanation of how it works, particularly focusing on the `find_package` command:

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
