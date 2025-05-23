
## What is `std::string`?

The `std::string` class is an instantiation of the `basic_string` class template that uses char (i.e., bytes) as its character type, with its default char_traits and allocator types (see basic_string for more info on the template).

```cpp
typedef basic_string<char> string;
typedef basic_string<char16_t> u16string;
```