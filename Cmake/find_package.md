https://stackoverflow.com/questions/35815850/getting-imported-targets-through-find-package

find_package is a two-headed beast these days:

CMake provides direct support for two forms of packages:
1. Config-file Packages and 
2. Find-module Packages

Source

Now, what does that actually mean?

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