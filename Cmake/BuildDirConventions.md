### Directory Organization: Why Use a Separate Build Directory?

A **separate build directory** (often named `build/`) is a widely recommended practice in C++ development, especially when using tools like **CMake** and **Conan**. Here's why:

- **Clean source tree:** Keeps all generated files (object files, binaries, cache files) out of your source directory. The source tree contains only code, scripts, configs, and documentation—making it easy to browse, commit, and share.
- **Easy clean-up:** To clean the build, you can simply delete the entire `build/` directory without touching your source files.
- **Multiple configurations:** You can create different build directories for various build types or configurations (`build-debug/`, `build-release/`, `build-static/`, etc.), letting you build the same codebase with different settings without conflicts.
- **Out-of-Source Builds:** Most C++ build tools (CMake, Conan, Meson, Ninja, etc.) support and encourage *out-of-source builds* for these reasons.

### **Current Working Directory (CWD) Nuances**

The **Current Working Directory (CWD)** is where a build tool "starts" its job. This has several implications:

- **Relative paths:** Many scripts/config files reference paths relative to the CWD.
- **Cache/temporary files:** CMake and Conan will drop all their `.cmake`, `.ninja`, `.conan`, and other temporary files into the CWD.
- **Reproducibility/conflicts:** Running CMake or Conan from the *source* directory pollutes it and risks conflicting, interleaved build artifacts if you run builds targeting different configurations.
- **Isolation:** Running from `build/` ensures that if you rerun or reconfigure, the process is isolated—no cross-contamination.

Conventions:

- **cd build && cmake ..**: This is the most common pattern to start a CMake/Conan build.
    - `cmake ..`: The `..` tells CMake to treat the *parent* directory as the "source," generating build files into the current (`build/`) directory.
    - Similarly, **Conan** is often run as `conan install ..` from within `build/`, so any install/cache artifacts land in `build/`.

Example:
```bash
# Source tree:
project/
  src/
  include/
  CMakeLists.txt
  conanfile.txt
  build/           # out-of-source build directory

cd project
mkdir build && cd build
conan install ..
cmake ..
cmake --build .
```

### **Nuances & Gotchas**

- **Never mix build and source:** Avoid running `cmake .` or `conan install .` from the source directory unless you understand the ramifications—these clutter your repo and make cleaning up harder.
- **CWD in scripts/plugins:** Some CMake scripts or Conan hooks expect certain files (like `conanfile.py` or `CMakeLists.txt`) to be found relative to where you invoke the tool. Using the wrong CWD or a deep build tree can break these expectations.
- **IDEs may override CWD:** Integrated Development Environments (like CLion, VSCode) sometimes configure their own build directories, often named `.cmake-build-debug`, `.vscode-build`, etc., to emulate this best practice.
- **.gitignore tip:** It’s convention to add `build*/` to .gitignore so you’ll never accidentally add build artifacts to version control.
- **Multiple targets:** For cross-builds or multi-arch builds, you might create specialized build directories (e.g., `build-arm/`, `build-windows/`) and run Conan/CMake from there.

### **First Principles & Motivation**

- **Reproducibility:** Clean, reproducible builds every time, for any configuration.
- **Automation:** CI/CD workflows depend on isolated, predictable build directories.
- **Simplicity:** One command to clean: `rm -rf build/`.

**Summary**:  
*Running CMake and Conan from a separate build directory (not the source tree) is critical to maintaining clean, reproducible, and manageable C++ projects. It prevents pollution of your repo, handles path and configuration isolation, and aligns with professional best practices.*

#### References
> : CMake Best Practices: Clang, GitHub Actions, Conan, and ...  
> : Conan documentation: Out of source builds  
> : Modern CMake practices and conventions