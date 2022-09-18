
## Imp config file

`CMakeLists.txt`

### Specifying source and build directories from cli

Convention: In-source builds not allowed, source and build dirs should be separate.

```sh
cmake -S src -B build
```

## CmakePresets.json

One problem that CMake users often face is **sharing settings with other people for common ways to configure a project**. This may be done to support CI builds, or for users who frequently use the same build. 

CMake supports two main files, `CMakePresets.json` and `CMakeUserPresets.json`, that allow users to specify common configure options and share them with others. CMake also supports files included with the include field



### Using libraries with Cmake find_package

`find_package(libName)`
relies on presence of a file named: `findLibName.cmake`.
For common such files for common libraries e.g. Boost, they are already
included with cmake:
https://github.com/Kitware/CMake/tree/master/Modules
