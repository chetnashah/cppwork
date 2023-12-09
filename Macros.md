
# Macros
**Macros** are a way to define constants or functions that are expanded inline by the preprocessor.

## Const macros are simple

```cpp
#define PI 3.14
```

## function like macros

```cpp
#define MIN(a, b) ((a) < (b) ? (a) : (b))
```

## Get raw string via `#`

```cpp
#define LOG(param) std::cout << #param << param << std::endl;
```