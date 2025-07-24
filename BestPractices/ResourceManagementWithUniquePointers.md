# C++ Resource Management with Custom Deleters and unique_ptr

## Understanding the Basics

[studyraid.com](https://app.studyraid.com/en/read/11857/376982/default-deleter-and-custom-deleters) explains that `std::unique_ptr` uses a default deleter (`std::default_delete`) that calls `delete` on the managed object. However, for custom resource management beyond simple memory allocation, you can define custom deleters using functions, lambdas, or functors.

## When to Use Custom Deleters

Custom deleters are essential when managing resources that require specialized cleanup beyond simple `delete`:

- **File handles** - Need `fclose()` instead of `delete`
- **Network connections** - Require proper socket closure
- **Third-party library resources** - May need specific cleanup functions
- **Arrays allocated with `new[]`** - Need `delete[]` instead of `delete`
- **C-style resources** - malloc/free, custom allocators

## Implementation Strategies

### 1. Function Pointer Deleters
```cpp
void fileDeleter(FILE* f) {
    if (f) fclose(f);
}

std::unique_ptr<FILE, decltype(&fileDeleter)> file(fopen("data.txt", "r"), &fileDeleter);
```

### 2. Lambda Deleters
```cpp
auto file = std::unique_ptr<FILE, decltype([](FILE* f) { 
    if (f) fclose(f); 
})>(fopen("data.txt", "r"), [](FILE* f) { if (f) fclose(f); });
```

### 3. Functor Deleters
```cpp
struct FileDeleter {
    void operator()(FILE* f) {
        if (f) fclose(f);
    }
};

std::unique_ptr<FILE, FileDeleter> file(fopen("data.txt", "r"));
```

## Best Practices

### 1. Choose the Right Deleter Type
- **Function pointers**: Simple, but add runtime overhead
- **Lambdas**: Zero overhead if stateless, but can complicate type deduction
- **Functors**: Most flexible, zero overhead for empty classes due to Empty Base Optimization

### 2. Handle Null Pointers
Always check for null in your custom deleters:
```cpp
auto deleter = [](MyResource* r) {
    if (r) {
        cleanup_resource(r);
        delete r;
    }
};
```

### 3. Use Type Aliases for Complex Types
```cpp
using FilePtr = std::unique_ptr<FILE, decltype([](FILE* f) { if (f) fclose(f); })>;
FilePtr openFile(const char* filename) {
    return FilePtr(fopen(filename, "r"), [](FILE* f) { if (f) fclose(f); });
}
```

### 4. RAII Wrapper Classes
For frequently used resources, consider creating dedicated RAII wrapper classes:
```cpp
class File {
    std::unique_ptr<FILE, decltype([](FILE* f) { if (f) fclose(f); })> file_;
public:
    explicit File(const char* filename) 
        : file_(fopen(filename, "r"), [](FILE* f) { if (f) fclose(f); }) {}
    
    FILE* get() const { return file_.get(); }
    explicit operator bool() const { return static_cast<bool>(file_); }
};
```

## Performance Considerations

[andreasfertig.com](https://andreasfertig.com/blog/2024/08/understanding-the-inner-workings-of-cpp-smart-pointers-the-unique_ptr-with-custom-deleter/) notes that `unique_ptr` uses a `compressed_pair` implementation that leverages Empty Base Optimization. When your custom deleter is an empty class (stateless functor), it takes up no additional memory.

## Common Pitfalls to Avoid

### 1. Don't Recreate Default Behavior
[stackoverflow.com](https://stackoverflow.com/questions/64732246/how-unique-ptr-can-use-the-custom-deleter-function) warns against creating custom deleters that just call `delete` - this provides no benefit over the default deleter.

### 2. Avoid Redundant Cleanup Functions
Don't create multiple functions that do the same cleanup operation. Consolidate your deletion logic.

### 3. Be Careful with Type Deduction
When using lambdas, the type can become complex. Consider using `auto` or type aliases to simplify code.

## Practical Examples

### Array Management
```cpp
auto array = std::unique_ptr<int[], std::default_delete<int[]>>(new int[100]);
// Or with custom allocator
auto custom_array = std::unique_ptr<int[], decltype([](int* p) { 
    custom_free(p); 
})>(custom_alloc<int>(100), [](int* p) { custom_free(p); });
```

### Third-Party Library Integration
```cpp
struct SDL_Deleter {
    void operator()(SDL_Surface* surface) { SDL_FreeSurface(surface); }
    void operator()(SDL_Texture* texture) { SDL_DestroyTexture(texture); }
};

using SurfacePtr = std::unique_ptr<SDL_Surface, SDL_Deleter>;
using TexturePtr = std::unique_ptr<SDL_Texture, SDL_Deleter>;
```

## Key Takeaways

1. **Use custom deleters for any resource that needs specialized cleanup**
2. **Prefer stateless functors for zero-overhead resource management**
3. **Always handle null pointers in your deleters**
4. **Create type aliases for complex deleter types**
5. **Consider RAII wrapper classes for frequently used resources**

Custom deleters with `unique_ptr` provide a powerful, zero-overhead abstraction for managing any type of resource while maintaining RAII principles and exception safety.