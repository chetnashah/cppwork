# Where Conan Stores Things & How To Peek Inside  
*(covers both Conan 1.x and Conan 2.x; note the small differences)*

---

## 1. Cache & Configuration Folders  

| Conan version | Default location on Linux / macOS | Default on Windows | How to override |
|---------------|-----------------------------------|--------------------|-----------------|
| **1.x**       | `~/.conan`                        | `C:\Users\<user>\.conan` | `CONAN_USER_HOME=/custom/path` |
| **2.x**       | `~/.conan2`                       | `C:\Users\<user>\.conan2` | `CONAN_HOME=/custom/path` |

Inside that root you will find:

```
└─ <home>
   ├─ settings.yml         # global settings
   ├─ remotes.json         # configured remotes
   ├─ profiles/            # named profiles
   ├─ data/                # **recipes & binaries**
   │   └─ <name>/<ver>/<user>/<channel>/
   │        ├─ export/           (recipe)
   │        ├─ export_source/    (exported sources)
   │        ├─ source/           (un-patched src)
   │        ├─ build/<pkgid>/    (build dirs)
   │        └─ package/<pkgid>/  (final artifacts)
   ├─ logs/                # one log per command run
   └─ ...                  # hooks, plugins, etc.
```

Conan 2 keeps the same ideas but flattens some sub-folders (`pkg/<pkgid>` instead of `package/<pkgid>`).

---

## 2. Quick Commands to Discover Where Things Are  

| What you want | Conan 1.x | Conan 2.x |
|---------------|-----------|-----------|
| Absolute path to current cache      | `conan config home`            | `conan config home` |
| Path to a particular recipe/binary  | `conan cache path <ref|pkg>`   | `conan cache path <ref|pkg>` |
| List everything in the cache        | `conan search "*" --table out.html` | `conan list "*" --graph out.html` |
| List remotes                        | `conan remote list`            | `conan remote list` |
| Show a profile                      | `conan profile show <name>`    | `conan profile show <name>` |
| Show current configuration values   | `conan config list`            | `conan config list` |

> Tip: add `--format=json` (1.x) or `--json file` (2.x) to any of the above if you prefer machine-readable output.

---

## 3. Inspecting Recipes & Dependency Graphs  

1. **Static recipe data**  
   ```
   conan inspect path/to/conanfile.py
   conan inspect <name>/<version>@<user>/<channel> -a options -a default_options
   ```

2. **Full dependency graph**  
   *Conan 1.x*  
   ```
   conan info . -pr clang-release --graph graph.html
   ```  
   *Conan 2.x*  
   ```
   conan graph info . -pr:b=default -pr:h=default --format html > graph.html
   ```

3. **Which binaries will be built / downloaded?**  
   ```
   conan install . -pr default --build missing --dry-run
   ```

---

## 4. Debugging & Verbosity  

• On any command append **`-v [quiet|error|warning|info|debug|trace]`**  
  ```
  conan create . -pr default -v debug
  ```

• Turn on tracing to a file (shows every API call):  
  ```
  export CONAN_TRACE_FILE=/tmp/conan_trace.log
  ```

• Increase log level globally:  
  ```
  conan config set log.level=debug      # 1.x
  conan config set core.logging.level=debug   # 2.x
  ```

• Environment variables that help:  

  | Variable | Effect |
  |----------|--------|
  | `CONAN_REVISIONS_ENABLED=1` (1.x) | show recipe revisions |
  | `CONAN_PRINT_RUN_COMMANDS=1`      | prints all `self.run()` commands |
  | `CONAN_SKIP_BROKEN_SYMLINKS=1`    | handy on Windows when cleaning cache |

---

## 5. Cleaning Up  

| Task | Conan 1.x | Conan 2.x |
|------|-----------|-----------|
| Remove a recipe & all binaries | `conan remove pkg/1.0@user/channel -f` | `conan remove pkg/1.0 -c -f` |
| Remove unused packages | `conan remove "*" --locks -f` | `conan cache clean` |
| Nuke entire cache (be careful!) | `rm -rf ~/.conan` | `rm -rf ~/.conan2` |

---

### TL;DR

1. Cache lives in `~/.conan` (1.x) or `~/.conan2` (2.x); override with `CONAN_USER_HOME` / `CONAN_HOME`.  
2. `conan config home`, `conan cache path`, `conan search`/`list`, and `conan inspect` are your best friends for spelunking.  
3. Add `-v debug` and `CONAN_TRACE_FILE` to see everything Conan is doing.