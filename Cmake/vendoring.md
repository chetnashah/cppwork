# “Dependency Vendoring” in C++

## 1. What does *vendoring* mean?

In the C/C++ ecosystem a *vendored* (or *embedded*, *snapshot*, *mirrored*) dependency is an **exact copy of someone else’s library that lives inside your repository’s source tree** and is built together with your own code.

```
my-app/
 ├─ third_party/
 │   └─ fmt/          ← full source of fmtlib copied in
 ├─ src/
 └─ CMakeLists.txt
```

No package manager, no `apt install`, no git submodules that point somewhere else: everything needed to compile the project is already present in the repo.

## 2. Why people vendor

1. **Reproducible / hermetic builds**  
   No matter which host OS or package manager is used, the exact same sources are compiled.

2. **Air-gapped or fire-walled environments**  
   Common in defense, automotive, medical, or build farms with no Internet access.

3. **Audit & supply-chain security**  
   All third-party code is pinned to a reviewed commit that can be scanned together with first-party code.

4. **Local fixes & forks**  
   You can apply emergency patches without waiting for upstream.

5. **Build systems that dislike external paths**  
   E.g., embedded toolchains or Bazel rules that expect everything to live below workspace root.

## 3. Typical vendoring workflow (CMake example)

Assume we want to vendor [fmt](https://github.com/fmtlib/fmt):

```bash
# 1. Add dependency as subtree or plain copy
git subtree add --prefix third_party/fmt https://github.com/fmtlib/fmt.git 10.2.1

# 2. Pin it with a commit hash (recorded in your repo)

# 3. (Optional) Patch it
patch -p1 < patches/fmt-visibility.patch
```

`CMakeLists.txt`:

```cmake
# Top–level CMakeLists.txt
add_subdirectory(third_party/fmt)
add_executable(my_app src/main.cpp)
target_link_libraries(my_app PRIVATE fmt::fmt-header-only)
```

The CI pipeline needs nothing else; it just clones your repo and runs CMake.

## 4. Real-world use cases

| Project / Domain               | Reason for vendoring                                            |
|--------------------------------|-----------------------------------------------------------------|
| Linux kernel (bundled zlib, libcrc) | Kernel must build without host libraries                     |
| Chrome / Chromium (`third_party/`)  | Security review, cross-platform hermetic builds              |
| Embedded firmware (e.g. Zephyr RTOS samples) | Offline, cross-compiled toolchains                    |
| Game studios’ proprietary engines | Deterministic builds over console SDK lifetimes               |

## 5. Drawbacks

* Repository bloat, slower clones.
* You must manually track CVEs and update copies.
* Possible license incompatibility if notices aren’t propagated.
* Fork divergence makes upstream rebases painful.

## 6. Alternatives to vendoring

1. **System package managers**

   * `apt`, `dnf`, `brew`, `pacman`.  
   * Pros: Small repo size, automatic security updates.  
   * Cons: Different package names/versions across distros; Windows support is weak.

2. **C++-specific package managers**

   * **vcpkg** (Microsoft)  
     `vcpkg.json` lists dependencies; CI installs exact versions via binary caches.
   * **Conan 2.x**  
     Declarative `conanfile.py`/`conanfile.txt`; supports lockfiles for reproducibility.
   * **Hunter**, **Buckaroo**, **Biicode** (legacy)  
   * Pros: Reproducible and lighter repos; many libraries pre-packaged.  
   * Cons: Extra bootstrap step; enterprise approval sometimes needed.

3. **Git submodules / Git subtrees**

   * Submodule keeps dependency in a separate repo but pinned to a commit.  
   * Similar reproducibility to vendoring with *less* snapshot duplication.  
   * Requires `git clone --recursive` and tooling that understands submodules.

4. **CMake FetchContent / ExternalProject**

   ```cmake
   include(FetchContent)
   FetchContent_Declare(
       fmt
       GIT_REPOSITORY https://github.com/fmtlib/fmt.git
       GIT_TAG 10.2.1
   )
   FetchContent_MakeAvailable(fmt)
   ```  
   *Downloads at configure time*; can be cached in CI artifacts.  
   Pros: No repo bloat. Cons: Needs network (unless mirrored), complicates offline builds.

5. **Pre-built binary bundles**

   * Ship `.a` / `.lib` / `.dll` in an “SDK” directory.  
   * Reduces compile times but may require multiple variants per platform/config.

## 7. Choosing between approaches

Decision matrix (✓ good, ✗ bad):

| Requirement / Approach            | Vendoring | Submodule | FetchContent | vcpkg/Conan | System pkg |
|-----------------------------------|-----------|-----------|--------------|-------------|------------|
| Works offline                     | ✓         | ✓         | ✗ (unless mirrored) | ✓ | ✓ |
| Small repository                  | ✗         | ✓         | ✓            | ✓           | ✓ |
| Easy to update                    | ✗         | ✓         | ✓            | ✓           | ✓ |
| Zero extra tooling for users      | ✓         | ✓         | ✗            | ✗           | ✗ |
| Auditable snapshot committed      | ✓         | ✓         | ✗            | Lockfile†   | ✗ |

†Conan/vcpkg lockfiles pin package hashes but the tarballs live elsewhere.

## 8. References & further reading

1. fmtlib documentation – https://github.com/fmtlib/fmt
2. Chromium’s “third_party” policy – https://chromium.googlesource.com/chromium/src/+/main/docs/adding_to_third_party.md
3. Git subtree vs submodule – Atlassian blog: https://www.atlassian.com/git/tutorials/git-subtree
4. Conan package manager – https://docs.conan.io/2/
5. vcpkg manifest mode – https://learn.microsoft.com/vcpkg/users/manifests
6. CMake `FetchContent` docs – https://cmake.org/cmake/help/latest/module/FetchContent.html
7. Supply-chain attacks in OSS – “Reflections on Trusting Trust” (K. Thompson, 1984) and recent SolarWinds analyses.

---

### TL;DR

*Vendoring* copies third-party source code into your repository so your project can build in isolation. It gives maximum control and reproducibility at the cost of larger repos and extra maintenance. Modern C++ projects often combine strategies: vendoring tiny headers, using vcpkg/Conan for bigger libs, and relying on system packages for OS-provided components.