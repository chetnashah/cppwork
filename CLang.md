
## C++ version compatibility and clang versions

https://clang.llvm.org/cxx_status.html

## See the pre-processor output

```bash
clang++ -E your_file.cpp -o preprocessed_output.cpp
```

## Compiling with a particular language standard

```sh
// compile with cpp20 syntax
clang++ eva-vm.cpp -o eva-vm -std=c++20
```