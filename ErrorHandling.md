
https://isocpp.org/wiki/faq/exceptions

https://www.youtube.com/watch?v=0ojB8c0xUd8

## Unhandled exceptions do not do stack unwinding.

Without a catch, i.e. unhnadled exceptions do not do stack unwinding.

As a result:
1. no destructor/resource cleanup happens
2. program terminates/aborts via `std::terminate`

A catch-all handler in `main` will probably guarantee that stack is going to unwind.

## Catch by reference (catch by value creates unnecessary copy)

```cpp
try {
    // code that may throw an exception
}
catch (const std::exception& e) {
    // handle the exception
}
```

catching exception by const reference - if you do not plan to modify exception:

```cpp
try {
    // code that may throw an exception
}
catch (std::exception const& e) {
    // handle the exception
}
```

## Stack unwinding during exception

For example, if someone says throw Foo(), the stack will be unwound so all the stack frames between the
```cpp
    throw Foo()
```
and the
```cpp
  }
  catch (Foo e)
  {
```
will get popped. This is called **stack unwinding.**

During stack unwinding, 
**all the local objects in all those stack frames are destructed, if the exception is caught**.

**If the exception is not caught, then the program will terminate/abort without the descructors being called.**

```cpp
#include "common.h"
#include <iostream>
#include <memory>

class SampleClass {
    public: 
        SampleClass() {
            std::cout << "SampleClass constructor called" << std::endl;
        }

        void displayValue() const {
            std::cout << "Value: " << "asdf" << std::endl;
        }

        ~SampleClass() {
            std::cout << "SampleClass destructor called" << std::endl;
        }
};

void d() {
    std::cout << "d() execution starts"<< std::endl;
    throw std::runtime_error("d() throws an exception");
    std::cout<< "d() execution ends"<< std::endl;
}

void c() {
    std::unique_ptr<SampleClass> s = std::make_unique<SampleClass>();
    std::cout << "c() execution starts"<< std::endl;
    d();
    std::cout<< "c() execution ends"<< std::endl;
}

void b() {
    std::cout << "b() execution starts"<< std::endl;
    c();
    std::cout<< "b() execution ends"<< std::endl;
}

void a() {
    std::cout << "a() execution starts"<< std::endl;
    b();
    std::cout<< "a() execution ends"<< std::endl;
}

int main(int argc, const char* argv[]) {
    try {
        a();
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    return 0;
}
```

Stack Unwinding:

When an exception is thrown, the C++ runtime looks for a suitable catch block to handle the exception.
During this process, the stack is "unwound," meaning that the destructors for local objects are called as the control flow goes out of scope.
If the exception is caught at some level, the stack unwinding proceeds, and destructors are called for objects in the stack frames being unwound.
Unhandled Exception:

If the exception is not caught and there is no suitable catch block, the program typically terminates.
In the case of program termination due to an unhandled exception, the C++ Standard does not mandate the calling of destructors for objects with automatic storage duration.

## Resource handling and self cleaning members

https://isocpp.org/wiki/faq/exceptions#selfcleaning-members


## Assert vs Exception handling

### Assert
In summary, use assert for debugging and checking assumptions during development, and assert is often disabled in release builds.

### Exception handling
use exceptions for handling runtime errors or exceptional situations during the normal execution of the program. 

assert is often disabled in release builds, while exceptions are an integral part of error handling in both development and production code.

Do not use throw to indicate a coding error in usage of a function. Use assert or other mechanism to either send the process into a debugger or to crash the process and collect the crash dump for the developer to debug.

Exceptions, when done right, separate the happy path from the error path.

The “good path” (sometimes called the “happy path”) is the control-flow path that happens when everything goes well — when there are no problems.

The “bad path” (or “error path”) is the path that control-flow takes when something goes wrong — when there is a problem.

## Exceptions are good for constructor validation

Constructors don’t have a return type, so it’s not possible to use return codes. **The best way to signal constructor failure is therefore to throw an exception**

For constructors, yes: You should throw an exception from a constructor whenever you cannot properly initialize (construct) an object. There is no really satisfactory alternative to exiting a constructor by a throw. For more details, see here.


## Handling destructor failures

Write a message to a log-file. Terminate the process. **But do not throw an exception!**

## Commonly thrown exceptions

In C++, exceptions are typically thrown in response to runtime errors or exceptional situations. The Standard Library provides a set of standard exception classes, declared in the `<stdexcept>` header, that cover common error scenarios. Here are some common exceptions thrown in C++:

### std::invalid_argument:

Thrown to indicate that a function has been called with an invalid argument.
Example: Trying to convert a string to a numeric type, and the string is not a valid representation of a number.
```cpp
throw std::invalid_argument("Invalid argument");
```

### std::out_of_range:

Thrown to indicate that an attempt was made to access an element outside the valid range of a container (e.g., vector, array).
Example: Accessing an element at an index that is beyond the size of a container.
```cpp
throw std::out_of_range("Index out of range");
```
### std::runtime_error:

A generic exception class representing errors that can be detected only at runtime.
Example: Throwing a runtime error when a file cannot be opened.
```cpp
throw std::runtime_error("Runtime error occurred");
```
### std::logic_error:

A generic exception class representing errors that are logic errors, i.e., violations of logical preconditions or class invariants.
Example: Throwing a logic error when a precondition of a function is not met.
```cpp
throw std::logic_error("Logic error occurred");
```

### std::bad_alloc:

Thrown by the new operator to indicate a failure to allocate memory.
Example: Memory allocation failure.
```cpp
throw std::bad_alloc();
```


