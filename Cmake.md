

### Using libraries with Cmake find_package

`find_package(libName)`
relies on presence of a file named: `findLibName.cmake`.
For common such files for common libraries e.g. Boost, they are already
included with cmake:
https://github.com/Kitware/CMake/tree/master/Modules
