Here’s a quick rundown:

## Commonalities Between `fstream` and `stringstream`

1. Both are full-fledged **`std::iostream`**s  
   - `std::fstream` inherits `std::basic_iostream<char>`  
   - `std::stringstream` inherits the same  

2. Both own a **`std::basic_streambuf`** (buffer) under the hood  
   - `fstream` → `std::filebuf`  
   - `stringstream` → `std::stringbuf`  

3. Same public interface:  
   - Formatted I/O: `operator<<`, `operator>>`  
   - Unformatted I/O: `.read()`, `.write()`, `.getline()`, `.get()`, `.put()`  
   - Positioning: `.seekg()`, `.seekp()`, `.tellg()`, `.tellp()`  
   - State flags & error handling: `.good()`, `.fail()`, `.eof()`, `.exceptions()`, `.clear()`  
   - Tying & flushing: `.tie()`, `.flush()`, `.sync()`  

4. Same open‐mode / flag machinery (`std::ios::in`, `out`, `app`, `binary`, etc.)  

5. RAII semantics: they open in the ctor (or via `.open()`) and close in the dtor  

6. Locale support & manipulators all work identically  

---

## Other Common C++ Stream Classes

### Console Streams  
- `std::cin`, `std::cout`, `std::cerr`, `std::clog`  
- Wide‐char versions: `std::wcin`, `std::wcout`, …

### File Streams  
- `std::ifstream`  (read-only)  
- `std::ofstream`  (write-only)  
- `std::fstream`   (read/write)  
- Wide‐char variants: `std::wifstream`, etc.

### String Streams  
- `std::istringstream`   (input only)  
- `std::ostringstream`   (output only)  
- `std::stringstream`    (both)  
- Wide‐char variants: `std::wistringstream`, …

### Base & Buffer Classes  
- `std::istream`, `std::ostream`, `std::iostream` (generic bases)  
- `std::streambuf` (pure buffering interface)  
- `std::filebuf` (disk-file buffer)  
- `std::stringbuf` (in-memory string buffer)

### Iterator Adapters  
- `std::istream_iterator<T>`  
- `std::ostream_iterator<T>`

---

**In practice** you’ll almost always use the high-level typedefs (`ifstream`, `ostringstream`, `cout`, etc.), but it’s good to know they all share the same interface-and‐buffer design inherited from `basic_iostream` + `basic_streambuf`.