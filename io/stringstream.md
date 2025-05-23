
## Take a line of text and parse it out

Use it via `#include <sstream>`, not in `<string>`.

## Useful for both reading and writing to strings (instead of console or files).


The class template std::basic_stringstream implements input and output operations on string based streams. It effectively stores an instance of std::basic_string and performs the input and output operations on it.

1. What Is It?  
   - A **string-backed stream**.  
   - Behaves like `std::cin`/`std::cout`, but reads from and writes to an in-memory `std::string` instead of the console or a file.

2. Why Use It?  
   - **Format data into strings** without juggling `std::to_string` or `sprintf`.  
   - **Parse data from strings** using the same `>>` operators you use with `std::cin`.  
   - Handy for quick conversions, CSV parsing, logging, or building text.

3. The Three Flavors  
   - `std::stringstream`: both input & output  
   - `std::istringstream`: input-only  
   - `std::ostringstream`: output-only  

4. How to Use It  
   a. Include the header:  
      ```cpp
      #include <sstream>
      ```  
   b. Create one:  
      ```cpp
      std::stringstream ss;           // read & write
      std::ostringstream out_ss;      // write only
      std::istringstream in_ss("42"); // read only
      ```  
   c. Write (format) with `<<`:  
      ```cpp
      ss << "Point(" << x << ", " << y << ")";
      std::string s = ss.str();      // e.g. "Point(3.14, 2.72)"
      ```  
   d. Read (parse) with `>>`:  
      ```cpp
      std::istringstream in("10 20 30");
      int a, b, c;
      in >> a >> b >> c;             // a=10, b=20, c=30
      ```  

5. Resetting / Reusing  
   - `ss.str()` returns the current buffer string.  
   - `ss.str(new_string)` replaces the buffer.  
   - `ss.clear()` resets error flags (e.g. after EOF) so you can reuse the stream.

6. A Complete Example  
   ```cpp
   #include <iostream>
   #include <sstream>

   int main() {
       // 1) Format numbers and text into one string:
       std::ostringstream builder;
       builder << "ID=" << 123 << "; Name=" << "Alice";
       std::string record = builder.str();
       std::cout << record << "\n";  // prints: ID=123; Name=Alice

       // 2) Parse fields back out of that string:
       std::istringstream parser(record);
       std::string token;
       while (std::getline(parser, token, ';')) {
           std::cout << "Field: '" << token << "'\n";
       }

       // 3) Reuse a single stringstream for mixed I/O:
       std::stringstream ss;
       ss << "3.14 42 foo";
       double d; int i; std::string word;
       ss >> d >> i >> word;
       std::cout << "d=" << d << ", i=" << i << ", word=" << word << "\n";

       return 0;
   }
   ```

7. Memory & Performance  
   - All operations happen in RAM—no disk I/O overhead.  
   - For very large or performance-critical loops, specialized parsing may be faster—but for most tasks, `stringstream` is clean and fast enough.

---

**Remember**:  
- Think of `stringstream` as **cin/cout** for strings.  
- Use `<<` to build/format, `>>` or `getline(...)` to parse.  
- `str()` to get or set the underlying text, and `clear()` to reset state.