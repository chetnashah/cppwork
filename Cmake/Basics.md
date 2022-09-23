

## Printing message

```cmake
message([<mode>] "message to display")
```

## Variables

These are either CMake- or user-defined. You can obtain the list of CMake-defined variables with:

Cmake defined variables - `$ cmake --help-variable-list`

### Important builtin variables

```
message("Project binary dir is = ${PROJECT_BINARY_DIR}")
message("PROject source dir is = ${PROJECT_SOURCE_DIR}")
```

1. `PROJECT_SOURCE_DIR` - where root `CmakeLists.txt` file is present.
2. `PROJECT_BINARY_DIR` - build folder for this project.


## add_library(libname files)

builds a library with libname, defaults to a static library i.e. `libname.a`.

e.g.
```
add_library(sayhello hello.h hello.cpp)
```

To create shared lib:
```
add_library(sayhello SHARED hello.h hello.cpp)
```


## target_link_libraries

