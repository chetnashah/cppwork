## FetchContent Approach

Where code lives:

   * Downloaded to: `build/_deps/imgui-src/` (source)
   * Built to: `build/_deps/imgui-build/` (build artifacts)
   * Key point: Content is downloaded to build directory, not your source tree

CMakeLists.txt would look like:
```cmake
    include(FetchContent)

    # Declare the dependency
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        v1.89.9  # or specific commit hash
    )

    # Make it available
    FetchContent_MakeAvailable(imgui)

    # ImGui doesn't provide CMakeLists.txt, so create library manually
    add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )

    target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
    )

    target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLEW)

    target_link_libraries(LogViewer PRIVATE imgui glfw GLEW::GLEW)
```
FetchContent variables available:
```cmake
   * ${imgui_SOURCE_DIR} → build/_deps/imgui-src
   * ${imgui_BINARY_DIR} → build/_deps/imgui-build
```
Pros:

   * No repo bloat (downloaded to build dir)
   * Full control over build configuration
   * Exact upstream structure
   * Automatic download and integration
   * CMake native (no external tools)

Cons:

   * Downloads on every clean build
   * Requires internet connectivity during configure
   * Slower configure step
   * Need to manually handle libraries without CMakeLists.txt

## Key Differences Summary

┌───────────────────────────────────────────────────────┬───────────────────────────────────────────────────────┬───────────────────────────────────────────────────────┬───────────────────────────────────────────────────────┐
│ Aspect                                                │ Conan                                                 │ Submodules                                            │ FetchContent                                          │
├───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┤
│ **Storage**                                           │ User cache                                            │ Source tree                                           │ Build tree                                            │
├───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┤
│ **Build time**                                        │ Fast (pre-built)                                      │ Medium                                                │ Medium                                                │
├───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┤
│ **Configure time**                                    │ Fast                                                  │ Fast                                                  │ Slow (downloads)                                      │
├───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┤
│ **Repo size**                                         │ Small                                                 │ Large                                                 │ Small                                                 │
├───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┤
│ **Offline builds**                                    │ Yes (cached)                                          │ Yes                                                   │ No (needs download)                                   │
├───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┼───────────────────────────────────────────────────────┤
│ **Version control**                                   │ conanfile.py                                          │ .gitmodules                                           │ CMakeLists.txt                                        │
└───────────────────────────────────────────────────────┴───────────────────────────────────────────────────────┴───────────────────────────────────────────────────────┴───────────────────────────────────────────────────────┘
