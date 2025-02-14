Thinking...
> 
> Okay, let's try to untangle these CMake commands. The user is confused about when to use target_include_directories, target_link_libraries, add_library, add_executable, add_subdirectory, and include_directories. Let's break each down and find some rules of thumb.
> 
> First, add_library and add_executable are about declaring what you're building. add_library creates a library (static, shared, etc.), and add_executable creates an executable. These should come first because they define the targets.
> 
> Next, target_include_directories and target_link_libraries are about setting properties for those targets. The include directories tell the compiler where to find headers, and link libraries specify dependencies. These come after declaring the targets because they modify them.
> 
> Now, include_directories is a older command that sets include paths globally for all targets in the directory, which isn't as precise as using target_include_directories. Modern CMake prefers the target-specific approach to avoid polluting other targets.
> 
> add_subdirectory is used to include another CMakeLists.txt from a subdirectory, which helps in structuring larger projects. It's different from the others because it's about project organization rather than target properties.
> 
> Putting this together, the order should be: start with add_subdirectory if you have subprojects, then declare targets with add_library/add_executable, then set their includes and links with the target-specific commands. Avoid the older include_directories in favor of target_include_directories for better scope control.
> 
> So the rules of thumb would be about order, scope, and preferring target-specific commands over directory-wide ones. This should help clarify when each command is appropriate.
> 

Here are key distinctions and rules of thumb to clarify these CMake commands:

**1. Target Declaration (What to Build)**
- `add_library()`: Use FIRST when creating *any type of library*
- `add_executable()`: Use FIRST when creating an *executable program*
- **Rule**: Always start by declaring your targets - these form the foundation of your build structure

**2. Target Relationships (How to Connect)**
- `target_link_libraries()`: Use AFTER target declaration to specify:
  - Direct dependencies (A needs B)
  - Transitive dependencies (A needs B which needs C)
  - Interface requirements (header-only libraries)
- **Rule**: Think "this target requires these other compiled components"

**3. Header Management (Where to Find)**
- `target_include_directories()`: Use AFTER target declaration to specify:
  - Private headers (implementation-only)
  - Public headers (interface for others)
  - Interface headers (header-only libraries)
- **Rule**: Answer "where are the headers for *this specific target* located?"

**4. Directory Scope (Where to Look)**
- `add_subdirectory()`: Use to add *subprojects* with their own CMakeLists.txt
  - Creates new scope (variables may not propagate up)
  - Good for modular project organization
- `include_directories()`: (Legacy) Affects *all targets* in current directory
  - Prefer `target_include_directories()` instead
- **Rule**: Use `add_subdirectory` for structural organization, avoid `include_directories`

**Key Mental Model**
1. **Declare** (What exists)
   - `add_library()`, `add_executable()`
2. **Configure** (How to build it)
   - `target_include_directories()`, `target_compile_options()`
3. **Connect** (What it needs)
   - `target_link_libraries()`

**Common Workflow Pattern**
```cmake
# 1. Declare targets first
add_library(math STATIC src/math.cpp)
add_executable(calculator src/main.cpp)

# 2. Configure headers for the library
target_include_directories(math
    PUBLIC include   # Users of math need these headers
    PRIVATE src      # Internal headers
)

# 3. Connect dependencies
target_link_libraries(calculator PRIVATE math)
```

**Critical Distinction**
- `target_*` commands: Affect specific, named targets
- Non-`target_*` commands: Affect all targets in current directory scope

**When to Use Which**
- Need to create something? → `add_library`/`add_executable`
- Need to find headers? → `target_include_directories`
- Need to connect dependencies? → `target_link_libraries`
- Need to add a subproject? → `add_subdirectory`
- Modern projects should avoid `include_directories` in favor of target-specific versions

Remember: CMake is declarative - declare your components first, then specify their relationships and properties.