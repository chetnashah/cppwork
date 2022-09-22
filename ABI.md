happens as a result of separate compilation.

https://accu.org/conf-docs/PDFs_2015/JonathanWakely-What%20Is%20An%20ABI%20And%20Why%20Is%20It%20So%20Complicated.pdf

https://github.com/react-native-community/discussions-and-proposals/issues/257

##

An ABI specification should define at least:
* function calling conventions and symbol naming
* representation of data types (layout, size, alignment, floating point format)
* object file format e.g. ELF, PE/COFF, Mach-O
* how programs are loaded, dynamic linking, symbol relocations, thread-local storage

## Why does it matter?

The ABI is what allows you to link to other code, either other files in your own project or third-party libraries
Your toolchain + OS define an ABI, but any C++ library defines its own ABI too, which is affected by:

* The functions that define the public API, which will be compiled to some mangled name that the linker uses
* The types which use expose their details such as the order of data members, and any virtual functions

## When do ABI issues arise?

When a C++ library gets compiled from sources together with an application that relies on that, an ABI is naturally stable because all the functions inside the library and the application share the same compilation environment. 

Things start to get challenging when the library and the application are compiled separately and then linked together.

## Things that might affect ABI

Interface of the library;
* `CPU architecture`. Parts of an ABI are intrinsically architecture-specific.
* `Standard library version` The standard library does not guarantee ABI stability therefore having public interfaces relying on it makes them ABI-unstable. Also, types from Standard Library are not compatible among different vendors;
* `Compilation flags`. Many compilation flags change ABI (e.g. -DNDEBUG, exception support, optimizations, etc);
* Particular compiler and platform-specific choices. vtable layout, exception tables, RTTI, and calling conventions are parts of an ABI.

## Why ABI stability matters

* `New versions of your library have to be linkable with other apps originally compiled with older versions of your library`. In some cases, it's inconvenient to rebuild the whole app just to test it with a new version of a library. It can take several hours to recompile everything instead of less than a minute for compiling the only changed parts. Slowing down the developer iteration cycle can be disruptive to some projects especially in cases where the app is needed to be recompiled with many versions of the library regularly (e.g. for different platforms).

* `Your library needs to be distributed (and updated) independently from apps based on it.` In some cases, it's just impossible or too dangerous to distribute a library with all apps that use it on a particular machine.
That mostly happens because of two reasons:
It would be too wasteful to deliver, store, load and initialize almost the same low-level library (e.g. networking, SSL, image decoding) for all apps that use it on a machine. (Imagine if all iOS apps would have a built-in on-screen keyboard inside their binaries.)
It would make it impossible to apply security patches to such libraries. The situation when fixing something like Heartbleed would require updating all apps on the platform. It would be a nightmare.

* `Your library must not impose language-specific limitations on other apps using your library`. A library might introduce some not so obvious limitations to the code of the apps. For example, if your library exposes interfaces that rely on a more modern version of C++ than the application uses, it might not even compile. In the opposite case, if an app uses a more modern version of the language than the library, it also might not compile (but it’s rare).


## Source compatibility vs Binary Compatibility

A library is **binary compatible**, if a program linked dynamically to a former version of the library continues running with newer
versions of the library without the need to recompile.

If a program needs to be recompiled to run with a new version of library but doesn't require any further modifications, the library is **source compatible**.

Binary compatibility saves a lot of trouble. It makes it much easier to distribute soware for a certain platform. Without ensuring binary
compatibility between releases, people will be forced to provide statically linked binaries.

## Calling conventions

Calling conventions dictate how functions are called, specifically which registers are used and how.
* Which direction the stack grows and where the stack pointer is
* Any alignment requirements for the stack
* How arguments are passed:
* Registers might be used for simple types, so which ones get used in which order must be known
* Other arguments are placed on the stack, so their positions and order must be known
* Which registers are used for return values
* Which registers are preserved across the call and which are not

## Symbol naming

When the compiler generates code to call a function it needs to know how the function's name is represented, so the linker can find it and the right function can be called.

Usually a C function such as `void frob()` is simply called `frob` in the assembly code, but some ABIs (or other languages) might use something different, like `_frob`

## Representation of types and values

* C data types are mapped to some native representation supported by the CPU
* On x86 short is a signed 16-bit object ("halfword") and int is a signed 32-bit object ("word")
* Floating point types typically use IEEE single-, double- or extended-precision formats
* Alignment restrictions on types will affect stack layout, and padding in structs
* Bit-field layout must be specified
* How is a null pointer represented?

## C & C++ ABI

C++ complicates ABI due to introdcution of inheritance/virtual functions, 
 how to arrange the layout for types with base classes.

 With multiple inheritance, should bases be laid out le to right, or right to le?

### Namespaces and overloading and name mangling

In C++ we can have `void blargle()` and `void blargle(int)` and `void argle::blargle()` in the same program
e compiler needs to generate a unique name for each entity with external linkage, so it uses a name mangling scheme to encode scope and
argument types into symbol names:
```
_Z7blarglev
_Z7blarglei
_ZN5argle7blargleEv
```

### Virtual functions.

e ABI dictates the layout and use of the `vtable` and `vptr`.

### Exception handling

When you throw and exception in your C++ code the compiler translates that into a call to some function provided by the C++ runtime (called `__cxa_throw` in the Itanium C++ ABI)

The runtime allocates an exception object, looks for a matching `catch` (handler) and starts unwinding the stack
If a handler is found another function in the runtime is called to get access to the exception object that was created, in order to initialize the handler
The ABI dictates the names and semantics of the runtime functions, and how the information about active handlers is represented

## Itanium C++ ABI

http://itanium-cxx-abi.github.io/cxx-abi/

GCC and Clang and some other compilers follow that specification, so code compiled with GCC and Clang can be linked together and interoperate.'


## Standard library ABI

For C programs there are a number of structs such as div_t (and POSIX types like stat and timeval) that must have a fixed layout for a given
ABI

C++ adds many more types in its standard library, all of which can be affected by the ABI complications we've been looking at

Inline functions and templates expose implementation details to users, and private members and the order in which virtual functions are declared is
also visible in headers

This means that almost everything in the Standard Library has to be considered as part of the implementation's ABI.

## What breaks a library ABI?
What breaks a library's ABI?

* Removing (or renaming) exported functions or classes.
* Adding, removing or reordering member variables or virtual functions.

