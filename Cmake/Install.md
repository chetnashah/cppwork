The install commands in CMake are not part of the normal build (compile) process. Instead, they are executed during a separate installation step, **after** the project has been built. Here’s a comprehensive breakdown of what that means and how these commands work:

---

### When Are Install Commands Run?

- **Post-Build Step:**  
  The `install()` commands in your CMakeLists.txt are executed only when you explicitly run an installation target. They **do not** run automatically when you compile the project.

- **How to Trigger Installation:**  
  - **Makefile Generators:** If you’re using a Makefile-based generator (like on Linux), you can run `make install` after building your project.
  - **CMake Command:** You can also run the command:
    ```
    cmake --install <build_directory>
    ```
    This tells CMake to execute all the install commands described in your CMake configuration.
  - **CMake Build Targets:** Alternatively, use:
    ```
    cmake --build . --target install
    ```
    which builds the install target.

- **Installation Prefix:**  
  The actual location where everything is installed is determined by the `CMAKE_INSTALL_PREFIX` variable. By default, this is usually `/usr/local` on Unix-like systems, but you can change it (e.g., to `/custom/path`) with:
  ```
  cmake -DCMAKE_INSTALL_PREFIX=/custom/path ..
  ```

---

### What Do the Install Commands Do?

1. **Target Installation (`install(TARGETS ...)`):**
   - **Purpose:** Installs the compiled output (e.g., libraries or executables).
   - **Example:**
     ```cmake
     install(TARGETS my_library DESTINATION lib)
     ```
   - **What Happens:**  
     This command takes the built library (which could be a shared library like `.so`, `.dll`, or `.dylib`, or a static library) and copies it into the `lib` subdirectory of the install prefix.

2. **Header or Directory Installation (`install(DIRECTORY ...)`):**
   - **Purpose:** Installs files such as public header files.
   - **Example:**
     ```cmake
     install(DIRECTORY include/ DESTINATION include)
     ```
   - **What Happens:**  
     This copies the entire contents of your `include/` directory (which contains headers intended for library users) into the destination include directory (e.g., `/usr/local/include`). Private headers (often kept in directories like `src/`) are not installed so that only the intended public interface is exposed.

3. **Exporting Targets (Optional Advanced Step):**
   - While not covered exhaustively in the basic setup, additional commands (like `install(EXPORT ...)`) can export the target definitions so that downstream projects can use your library with CMake’s `find_package` support.

---

### Summary

- **Triggering Installation:**  
  The install commands run only when you explicitly invoke the installation target (via `make install`, `cmake --install`, or similar).

- **Role of CMake Install:**  
  They handle copying the compiled artifacts (libraries, executables) and selected source files (like public headers) to designated directories under the installation prefix. This provides users of your library with the necessary files (and only the necessary files) for using your library.

- **Separation of Concerns:**  
  Using install commands helps keep the build process separate from the distribution process, ensuring that only the chosen public components (e.g., public headers and libraries) end up in the final installed package, while private implementation details remain hidden.

In essence, the install commands are a post-build step that packages your project’s output into a usable format for installation and distribution.