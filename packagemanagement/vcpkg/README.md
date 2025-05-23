Here are the most important **Vcpkg** commands and configuration details to help you manage C/C++ libraries effectively:

---

### **1. Basic Commands**
#### **Installation & Setup**
- **Install Vcpkg** (clone & bootstrap):
  ```sh
  git clone https://github.com/microsoft/vcpkg
  ./vcpkg/bootstrap-vcpkg.sh  # Linux/macOS
  .\vcpkg\bootstrap-vcpkg.bat # Windows
  ```
- **Add to PATH** (for easy access):
  ```sh
  export PATH=$PATH:/path/to/vcpkg  # Linux/macOS
  set PATH=%PATH%;C:\path\to\vcpkg  # Windows
  ```

#### **Package Management**
- **Install a library**:
  ```sh
  vcpkg install <package>           # e.g., vcpkg install zlib
  ```
- **Install for a specific triplet** (e.g., `x64-windows`, `arm64-linux`):
  ```sh
  vcpkg install <package> --triplet=<triplet>
  ```
- **Search for packages**:
  ```sh
  vcpkg search <keyword>            # e.g., vcpkg search boost
  ```
- **List installed packages**:
  ```sh
  vcpkg list
  ```
- **Remove a package**:
  ```sh
  vcpkg remove <package>
  vcpkg remove --purge <package>    # Also delete downloaded files
  ```

#### **Updating**
- **Update Vcpkg itself**:
  ```sh
  git pull
  ./bootstrap-vcpkg.sh
  ```
- **Upgrade installed packages**:
  ```sh
  vcpkg upgrade --no-dry-run
  ```

---

### **2. Configuration**
#### **Triplets (Build Targets)**
- **List available triplets**:
  ```sh
  vcpkg help triplet
  ```
- **Custom triplets**:  
  Modify or create files in `vcpkg/triplets/` (e.g., `x64-windows-static.cmake`).

#### **Integration with Build Systems**
- **CMake Integration**:  
  Pass the toolchain file to CMake:
  ```sh
  cmake -B build -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
  ```
- **Visual Studio (MSBuild)**:  
  Run the following to integrate globally:
  ```sh
  vcpkg integrate install
  ```

#### **Environment Variables**
- `VCPKG_ROOT`: Set to the Vcpkg directory.
- `VCPKG_DEFAULT_TRIPLET`: Override the default triplet (e.g., `x64-linux`).

---

### **3. Advanced Commands**
- **Export packages** (for offline use):
  ```sh
  vcpkg export <package> --zip       # Export as .zip
  vcpkg export <package> --nuget     # Export as NuGet package
  ```
- **Build in Release Mode**:
  ```sh
  vcpkg install <package> --triplet=x64-windows-static
  ```
- **Clean build artifacts**:
  ```sh
  vcpkg clean
  ```

---

### **4. Common Issues & Fixes**
- **Version Conflicts**: Use `vcpkg upgrade` to resolve outdated packages.
- **Missing Libraries**: Ensure the triplet matches your target platform.
- **Proxy Issues**: Configure Git/curl proxy settings before bootstrapping.

---

### **5. Documentation & Help**
- **Vcpkg official docs**: [https://vcpkg.io/](https://vcpkg.io/)
- **Help command**:
  ```sh
  vcpkg help
  vcpkg help <command>  # e.g., vcpkg help install
  ```

### **Vcpkg Configuration Files & CMake Integration**  

Vcpkg allows for **project-specific configuration** using manifest files (`vcpkg.json`) and seamless integration with CMake. Below are the key details:

---

## **1. Project Configuration with `vcpkg.json`**  
A `vcpkg.json` file (manifest) defines dependencies and settings for your project.  

### **Basic Example:**
```json
{
  "name": "my-project",
  "version": "1.0",
  "dependencies": [
    "fmt",
    "boost-algorithm",
    "eigen3"
  ],
  "builtin-baseline": "a1a2a3b4c5d6",  // Git commit hash for baseline version
  "overrides": [
    { "name": "zlib", "version": "1.2.12" }  // Force a specific version
  ]
}
```
#### **Key Fields:**
| Field | Purpose |
|--------|---------|
| `"name"` | Project name (optional but recommended) |
| `"version"` | Project version (SemVer) |
| `"dependencies"` | List of required packages |
| `"builtin-baseline"` | Lock versions to a Vcpkg commit (ensures reproducibility) |
| `"overrides"` | Force a specific package version |
| `"features"` | Enable optional library features (e.g., `"features": ["ssl"]` for OpenSSL) |

#### **Generating `vcpkg.json` Automatically**  
Run:  
```sh
vcpkg new --application  # Creates a minimal manifest
vcpkg add port <package> # Adds a dependency
```

---

## **2. CMake Integration**  
Vcpkg works best with **CMake**, providing automatic dependency resolution.  

### **Step 1: Configure CMake with Vcpkg Toolchain**
Pass the Vcpkg toolchain file to CMake:  
```sh
cmake -B build -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```
(Alternatively, set `CMAKE_TOOLCHAIN_FILE` in your `CMakeLists.txt` or as an environment variable.)

### **Step 2: Use `find_package` in CMakeLists.txt**
Vcpkg modifies CMake’s package search paths, so dependencies are found automatically:
```cmake
cmake_minimum_required(VERSION 3.15)
project(MyProject)

find_package(fmt REQUIRED)       # Finds Vcpkg-installed fmt
find_package(Boost REQUIRED COMPONENTS algorithm)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE fmt::fmt Boost::algorithm)
```

### **Step 3: Build & Run**
```sh
cmake --build build
./build/my_app  # (Linux/macOS) or .\build\Debug\my_app.exe (Windows)
```

---

## **3. Advanced Configuration**  
### **Using Version Constraints**
```json
{
  "dependencies": [
    {
      "name": "fmt",
      "version>=": "8.0.0",
      "version<": "9.0.0"
    }
  ]
}
```

### **Conditional Dependencies (Platform-Specific)**
```json
{
  "dependencies": [
    { "name": "libcurl", "platform": "windows" },  # Windows-only
    { "name": "openssl", "platform": "!windows" }  # Non-Windows
  ]
}
```

### **Feature Selection**
Enable optional features (e.g., OpenSSL in `libcurl`):  
```json
{
  "dependencies": [
    {
      "name": "libcurl",
      "features": ["ssl"]
    }
  ]
}
```

---

## **4. Common Issues & Fixes**  
| Issue | Solution |
|-------|----------|
| **CMake can’t find Vcpkg-installed packages** | Ensure `CMAKE_TOOLCHAIN_FILE` is set correctly |
| **Version conflicts** | Use `overrides` in `vcpkg.json` |
| **Missing features** | Specify `features` in `vcpkg.json` |
| **Locking versions** | Use `"builtin-baseline"` for reproducibility |
| **Debug vs. Release builds** | Use `--triplet=x64-windows-static` (static) or `--triplet=x64-windows` (dynamic) |

---

## **5. Best Practices**  
1. **Always use `vcpkg.json`** (instead of manual `vcpkg install`).  
2. **Lock versions with `builtin-baseline`** for reproducible builds.  
3. **Prefer CMake integration** over manual linking.  
4. **Use CI-friendly workflows** (e.g., GitHub Actions + Vcpkg caching).  

---

### **References**  
- [Vcpkg Manifest Mode Docs](https://learn.microsoft.com/en-us/vcpkg/users/manifests)  
- [CMake + Vcpkg Integration Guide](https://learn.microsoft.com/en-us/vcpkg/users/buildsystems/cmake-integration)  
