
## Using with cmake

```cmake
set (CMAKE_BUILD_TYPE Debug)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)
```

## What it detects

### use-after-free

### double-free

### index-out-of-bounds