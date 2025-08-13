# Test Discovery and Registration in CMake + Conan Projects (Colocated Tests)

Great question! Here's how Conan/CMake figured out where tests are located and in which order to execute them with the **colocated test structure**:

## **Project Structure Overview** 📁

```
cppplayground/
├── CMakeLists.txt                 # Root build configuration
├── conanfile.py                   # Conan dependencies
├── libs/                          # Source libraries
│   ├── math/                      # Math library
│   │   ├── include/math/
│   │   │   ├── calculator.h
│   │   │   └── vector.h
│   │   ├── src/
│   │   │   ├── calculator.cpp
│   │   │   └── vector.cpp
│   │   ├── tests/                 # ⬅︎ Colocated tests
│   │   │   ├── test_calculator.cpp
│   │   │   └── test_vector.cpp
│   │   └── CMakeLists.txt         # Builds lib + its tests
│   └── utils/                     # Utils library
│       ├── include/utils/
│       │   ├── file_utils.h
│       │   └── string_utils.h
│       ├── src/
│       │   ├── file_utils.cpp
│       │   └── string_utils.cpp
│       ├── tests/                 # ⬅︎ Colocated tests
│       │   ├── test_string_utils.cpp
│       │   └── test_file_utils.cpp
│       └── CMakeLists.txt         # Builds lib + its tests
└── apps/                          # Application executables
    ├── calculator/
    └── file-manager/
```

## **How CMake Found the Tests** 🔍

### **1. Test Discovery Process**
CMake discovered tests through this hierarchical chain:

1. **Root CMakeLists.txt** → `enable_testing()` → Enables CTest integration
2. **Root CMakeLists.txt** → `option(BUILD_TESTING "Build tests" ON)` → Enables test building by default
3. **Root CMakeLists.txt** → `add_subdirectory(libs)` → Includes the libs directory
4. **libs/CMakeLists.txt** → `add_subdirectory(math)` → Includes math library
5. **libs/math/CMakeLists.txt** → `if(BUILD_TESTING)` → Conditionally builds tests
6. **libs/math/CMakeLists.txt** → `add_test(NAME MathTests COMMAND test_math)` → Registers test with CTest

### **2. Colocated Test Configuration**
Each library's CMakeLists.txt now handles its own tests:

```cmake
# In libs/math/CMakeLists.txt

# Math library
add_library(math
    src/calculator.cpp
    src/vector.cpp
)

# Math library tests (colocated)
if(BUILD_TESTING)
    find_package(GTest REQUIRED)
    
    add_executable(test_math
        tests/test_calculator.cpp    # Tests math::Calculator
        tests/test_vector.cpp        # Tests math::Vector3D
    )
    
    target_link_libraries(test_math
        PRIVATE
            math              # Links to this library
            GTest::gtest      # Links to Conan-provided GTest
            GTest::gtest_main
    )
    
    add_test(NAME MathTests COMMAND test_math)
endif()
```

```cmake
# In libs/utils/CMakeLists.txt

# Utils library
add_library(utils
    src/file_utils.cpp
    src/string_utils.cpp
)

# Utils library tests (colocated)
if(BUILD_TESTING)
    find_package(GTest REQUIRED)
    
    add_executable(test_utils
        tests/test_string_utils.cpp  # Tests utils::StringUtils
        tests/test_file_utils.cpp    # Tests utils::FileUtils
    )
    
    target_link_libraries(test_utils
        PRIVATE
            utils             # Links to this library
            GTest::gtest      # Links to Conan-provided GTest
            GTest::gtest_main
    )
    
    add_test(NAME UtilsTests COMMAND test_utils)
endif()
```

## **How CMake Determined Test Order** 📋

### **1. Build Dependency Graph**
CMake automatically determined the build order through dependency analysis:

```
libs/math (math library)     ← Built first
libs/utils (utils library)   ← Built first
    ↓
test_math executable         ← Built second (depends on math + GTest)
test_utils executable        ← Built second (depends on utils + GTest)
    ↓
CTest registration           ← Registered in order of add_test() calls
```

### **2. Test Execution Order**
From your test output:
```
test 1: MathTests (test_math executable)
test 2: UtilsTests (test_utils executable)
```

This order was determined by:
- **Registration sequence** in each library's CMakeLists.txt (MathTests registered first)
- **No explicit dependencies** between test executables themselves
- **CTest's default behavior** of running tests in registration order

### **3. Conan's Role in Test Dependencies**
Conan provides the testing framework but doesn't control test discovery:

```cmake
# Conan generates these targets automatically
find_package(GTest REQUIRED)  # Uses Conan-generated GTestConfig.cmake
target_link_libraries(test_math PRIVATE GTest::gtest GTest::gtest_main)
```

## **Test-Source Relationship** 🔗

### **1. Header Inclusion**
Tests include the headers of the library they're testing (same directory level):

```cpp
// libs/math/tests/test_calculator.cpp
#include <math/calculator.h>  // Tests math::Calculator

// libs/math/tests/test_vector.cpp  
#include <math/vector.h>      // Tests math::Vector3D

// libs/utils/tests/test_string_utils.cpp
#include <utils/string_utils.h>  // Tests utils::StringUtils

// libs/utils/tests/test_file_utils.cpp
#include <utils/file_utils.h>    // Tests utils::FileUtils
```

### **2. Library Linking**
Tests link against the compiled library in the same directory:

- `test_math` → links to `libmath.a` (contains calculator.cpp + vector.cpp)
- `test_utils` → links to `libutils.a` (contains file_utils.cpp + string_utils.cpp)

### **3. Build Artifacts Location**
In the build directory structure:
```
build/debug/
├── libs/
│   ├── math/
│   │   ├── libmath.a              # Compiled math library
│   │   └── test_math              # Test executable (links libmath.a + GTest)
│   └── utils/
│       ├── libutils.a             # Compiled utils library
│       └── test_utils             # Test executable (links libutils.a + GTest)
```

## **Benefits of Colocated Tests** 🎯

### **1. Proximity**
- Tests are **physically close** to the code they test
- Easier to find and maintain tests
- Natural organization by library/component

### **2. Encapsulation**
- Each library **owns its tests**
- Tests are built **only when the library is built**
- Clear separation of concerns

### **3. Modularity**
- Tests can be **enabled/disabled per library**
- Independent test execution
- Easier to exclude tests from production builds

### **4. IDE Integration**
- Better IDE support for test discovery
- Tests appear alongside source code
- Improved developer experience

## **Key Points** 🎯

- **CMake** handles the **build system** and **test discovery** through directory traversal
- **CTest** handles the **test execution** and **ordering** based on registration
- **Conan** provides **testing framework** (GTest) but doesn't control test discovery
- **Test order** is primarily determined by the order of `add_test()` calls
- **Build order** is determined by CMake's dependency analysis of libraries → test executables
- **Colocated tests** provide better organization and maintainability
- **BUILD_TESTING** option controls whether tests are built (enabled by default)

The beauty is that CMake automatically figured out the complete dependency graph: source libraries → colocated test executables → test registration, and CTest executed them in the logical order! The colocated structure makes the relationship between tests and source code much clearer and more maintainable.