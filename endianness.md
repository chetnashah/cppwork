
## core methods

```cpp
uint16_t read_uint16_be(const unsigned char* p) {
    return (static_cast<uint16_t>(p[0]) << 8) |
           (static_cast<uint16_t>(p[1]));
}

uint32_t read_uint32_be(const unsigned char* p) {
    return (static_cast<uint32_t>(p[0]) << 24) |
           (static_cast<uint32_t>(p[1]) << 16) |
           (static_cast<uint32_t>(p[2]) << 8)  |
           (static_cast<uint32_t>(p[3]));
}

uint64_t read_uint64_be(const unsigned char* p) {
    return (static_cast<uint64_t>(p[0]) << 56) |
           (static_cast<uint64_t>(p[1]) << 48) |
           (static_cast<uint64_t>(p[2]) << 40) |
           (static_cast<uint64_t>(p[3]) << 32) |
           (static_cast<uint64_t>(p[4]) << 24) |
           (static_cast<uint64_t>(p[5]) << 16) |
           (static_cast<uint64_t>(p[6]) << 8)  |
           (static_cast<uint64_t>(p[7]));
}
```

## endianness (only applies to multi-byte types, not array items placement)

## For array items, the array is stored in contiguous memory locations and has nothing to do with endianness.

So, endianness only applies to multi-byte types, not to arrays.
E.g. `uint32_t arr[4];` is stored as 4 contiguous `uint32_t`s one after the other, each `uint32_t` is stored according to the endianness of the system.
