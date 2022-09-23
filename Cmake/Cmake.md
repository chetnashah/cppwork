
## What is a target?

Anything that cmake can build/run.

* Executable
* Static library
* Shared Library
* Object library
* Custom target

All code files are compiled as part of one target or another.

Dependencies are encoded in target specification. 

## Static library

## Object library

similar to static library


## Imp config file

`CMakeLists.txt`

### Specifying source and build directories from cli

Convention: In-source builds not allowed, source and build dirs should be separate.

```sh
## just generates build system files in build dir, does not build targets
cmake -S src -B build
```

### Asking cmake to build targets

Invoke underlying build system that was already generated, in order to build actual targets.
```
// build is the build dir where it should make targets
cmake --build build
```

## Run cmake with different variables (overriden in CLI)

```shell
cmake -D BUILD_SHARED_LIBS=TRUE -B build
```

## Cmake configure step & generate step

CMake configures and generates the project. Cmake is considered a build system generator, which generates makefiles,
which can finally be used to build projects.

The command line cmake tool doesn't allow you to separate out running the configure and generate steps individually. Rather, it always runs configure and then generate.


### Configure step

The first stage is the configure step where the CMakeLists.txt file is read in. CMake builds up an internal representation of the project during this stage.

When you run the configure step, the GUI shows all cache variables (see below) which changed their values since the last time configure was run or since CMake GUI was started if this is the first configure run.

### Generate step

project files are written out based on that internal representation

## CmakePresets.json

One problem that CMake users often face is **sharing settings with other people for common ways to configure a project**. This may be done to support CI builds, or for users who frequently use the same build. 

CMake supports two main files, `CMakePresets.json` and `CMakeUserPresets.json`, that allow users to specify common configure options and share them with others. CMake also supports files included with the include field


### Cmake variants

CMake Tools introduces the concept of CMake Variants, a way to group together and combine a common set of build options and give them a useful name. The main interface for creating and building variants is `cmake-variants.json`, or `cmake-variants.yaml`.

The idea of “variants” are separate from the concept of toolchains/toolsets, which are handled via Kits.

By default, if no variants file is present, CMake Tools will load four variants, each corresponding to a default CMake build type: `Release`, `Debug`, `MinSizeRel`, and `RelWithDebInfo`. **Selecting one of these variants will configure and build with that build type.**




### Using libraries with Cmake find_package

`find_package(libName)`
relies on presence of a file named: `findLibName.cmake`.
For common such files for common libraries e.g. Boost, they are already
included with cmake:
https://github.com/Kitware/CMake/tree/master/Modules
