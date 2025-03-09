
## fstream for file input/output

Usable from `#include <fstream>`

## Buffered I/O and flushing

Output in C++ may be buffered. This means that anything that is output to a file stream may not be written to disk immediately. Instead, several output operations may be batched and handled together. This is done primarily for performance reasons. 

When a buffer is written to disk, this is called flushing the buffer. 

One way to cause the buffer to be flushed is to close the file -- the contents of the buffer will be flushed to disk, and then the file will be closed.

### What causes buffer flushing

It is possible to flush the buffer manually using the 
1. `ostream::flush()` function or sending `std::flush` to the output stream. Either of these methods can be useful to ensure the contents of the buffer are written to disk immediately, just in case the program crashes.

2. One interesting note is that `std::endl`; also flushes the output stream. Consequently, overuse of std::endl (causing unnecessary buffer flushes) can have performance impacts when doing buffered I/O where flushes are expensive (such as writing to a file). For this reason, performance conscious programmers will often use ‘\n’ instead of std::endl to insert a newline into the output stream, to avoid unnecessary flushing of the buffer.



## ofstream for outpu

## ifstream for input

## When is the file opened?

1. Automatic - **Constructors with parameters**: When you pass a filename (and optionally, the desired file mode), the constructor attempts to open the file automatically.
```cpp
std::string filename = "example.txt";
// Note: automatically opened for reading and writing
std::fstream file(filename, std::ios::in | std::ios::out);
std:: string word;
file >> word;
std::cout << word << std::endl;
```
2. Manual `.open()` call Default constructor: If you use the default constructor (i.e., std::fstream file;), the file is not opened immediately. You must call the .open() method later to open a file.

```cpp
std::fstream file;
// Note: since fstream was created with no params, needs to be explicitly opened by providing name and mode
file.open("example.txt", std::ios::in | std::ios::out);
```

## Exception handling (turned off by default)

**Note** - By default, you must check file open failure via a method `file.is_open()`.

There are a couple of reasons why your program might not crash even if a file open attempt “fails” and an exception is possible:

1. **Exceptions Are Not Thrown by Default:**  
   By default, C++ file stream objects (such as `std::ifstream`, `std::ofstream`, or `std::fstream`) do not throw exceptions. Instead, they set error flags (like `failbit` or `badbit`) when something goes wrong. If you haven't explicitly enabled exceptions using the `exceptions()` member function, no exception will be thrown on an error, and your program will continue to run normally.

   ```cpp
   std::ifstream file("nonexistent.txt");
   // No exception is thrown here; instead, file.fail() returns true.
   if (!file) {
       std::cerr << "File failed to open." << std::endl;
   }
   ```

2. **Exception Handling Is in Place:**  
   If you have enabled exceptions on your file stream (using the `exceptions()` method) and you wrap your file operations in a try-catch block, the exception will be caught instead of causing the program to crash. This allows your program to handle the error gracefully.

   ```cpp
   #include <fstream>
   #include <iostream>
   #include <exception>

   int main() {
       std::ifstream file;
       // Enable exceptions for failbit and badbit
       file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
       try {
           file.open("nonexistent.txt");
           // Use the file stream...
       } catch (const std::ios_base::failure& e) {
           std::cerr << "Exception caught: " << e.what() << std::endl;
           // The program doesn't crash because the exception was caught.
       }
       return 0;
   }
   ```

In summary, if your program doesn't crash when file open fails, it's likely because one of the following is true:

- **File Streams by Default:** Aren't set to throw exceptions; instead, they simply update internal state flags.
- **Catching Exceptions:** Your code is catching exceptions properly, preventing the exception from propagating and terminating the program.

By controlling exception behavior and handling errors explicitly—either by checking stream state or using try-catch blocks—you ensure that your program handles file open errors gracefully without crashing.