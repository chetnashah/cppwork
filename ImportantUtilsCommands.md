

## ldd

prints the shared libraries required by each program or shared library specified on the command line.
e.g. `ldd helloworldbin`

Use `otool -L` on mac:
```
% otool -L helloworld 
helloworld:
        /usr/lib/libc++.1.dylib (compatibility version 1.0.0, current version 1300.23.0)
        /usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1311.100.3)
```

## file

Prints binary info.

```
% file helloworld 
helloworld: Mach-O 64-bit executable arm64
```

## objdump

### Print file header using -f

### print assembly

`objdump -d helloworldbin`

## size

`size helloworldbin`

## ar

`ar` can be used for building and inspecting static libraries. Large projects will often build subcomponents into static libraries or wind up including a `.a` provided by a vendor.

## strings

print all hardcoded strings in binary