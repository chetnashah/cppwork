
## Type alias declarations (via "using" keyword)

**Always prefer type aliases over typedefs**. They are more readable and can be used with templates.

```cpp
using alias_identifier = type;
```
An alias doesn't introduce a new type and can't change the meaning of an existing type name. It's just a new name for an existing type. It's useful when the type is long or complex, or when it's used multiple times.

```cpp
// C++11
// useful for creating short forms
using fmtfl = std::ios_base::fmtflags;

// C++03 equivalent:
// typedef std::ios_base::fmtflags fmtfl;

fmtfl fl_orig = std::cout.flags();
fmtfl fl_hex = (fl_orig & ~std::cout.basefield) | std::cout.showbase | std::cout.hex;
// ...
std::cout.flags(fl_hex);
```

**Aliases** are much more readable for function pointer aliases:

```cpp
// C++11
using func = void(*)(int); // easy to read!

// C++03 equivalent:
// typedef void (*func)(int); // hard to read!

// func can be assigned to a function pointer value
void actual_function(int arg) { /* some code */ }
func fptr = &actual_function;
```

### Another advantage: "using" alias works with templates

A limitation of the typedef mechanism is that it doesn't work with templates. However, the type alias syntax in C++11 enables the creation of alias templates:

```cpp
template<typename T> using ptr = T*;

// the name 'ptr<T>' is now an alias for pointer to T
ptr<int> ptr_int;
```