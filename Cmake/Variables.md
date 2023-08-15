

## `<PROJECT_NAME>_SOURCE_DIR`

Top level source directory for the named project.

A variable is created with the name used in the `project()` command, and is the source directory for the project. This can be useful when `add_subdirectory()` is used to connect several projects.

e.g.
```cmake
project (NUMCPP CXX) // introduces NUMCPP_SOURCE_DIR

include_directories (${NUMCPP_SOURCE_DIR}/numcpp)
```

## `<PROJECT_NAME>_BINARY_DIR`

