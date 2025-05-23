
## cin/read error handling

When you do something like:

```cpp
int x;
std::cin >> x;
```

and the next characters in the input buffer are non-numeric (e.g. “abc”), here’s what happens:

1. Extraction tries to parse an integer from the next characters.  
2. It looks for an optional sign, then digits.  
3. If *no* digits are consumed at all, the stream’s **failbit** is set:  
   - `x` is left unchanged.  
   - The offending characters (“abc”) remain in the buffer for the next I/O.  
   - `std::cin` will compare equal to `false` in a boolean context.  
4. Further I/O on that stream will do nothing until you clear the error state.

Concretely:

```text
Input buffer:  "abc\n"
int x =  99;   // some old value
std::cin >> x; // fails immediately
// x == 99 still
// std::cin.fail() == true
// buffer still contains "abc\n"
```

—whereas if the buffer were `"123abc\n"`, then `x` would become `123`, leave `"abc\n"`, and the stream stay good.

How to handle it in practice
---------------------------

1. **Check the stream state**  
   Always write:
   ```cpp
   if (!(std::cin >> x)) {
     // parse failed
   }
   ```
2. **Clear the error flags**  
   ```cpp
   std::cin.clear();            // clear failbit/… so future I/O works
   ```
3. **Discard the bad input**  
   ```cpp
   std::cin.ignore(
     std::numeric_limits<std::streamsize>::max(), 
     '\n'
   );
   ```
   This skips to the end of the current line (or up to some max).

4. **Re-prompt in a loop**  
   ```cpp
   #include <limits>
   int x;
   while (true) {
     std::cout << "Enter a number: ";
     if (std::cin >> x) break;                   // success!
     std::cin.clear();                           // clear the failure
     std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                     '\n');                      // discard bad input
     std::cout << "Invalid input; please try again.\n";
   }
   std::cout << "You entered " << x << "\n";
   ```

Alternative: Read a full line and parse it yourself:

```cpp
#include <iostream>
#include <sstream>
#include <string>

int read_int() {
  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream ss(line);
    int x;
    char c;
    if (ss >> x && !(ss >> c))  // succeeded and no extra garbage
      return x;
    std::cout << "Invalid input; try again: ";
  }
  throw std::runtime_error("EOF reached");
}

int main() {
  std::cout << "Enter a number: ";
  int n = read_int();
  std::cout << "Got " << n << "\n";
}
```

Both patterns—checking `cin`’s state and either clearing/ignoring or switching to a line-based parse—are widely used to robustly handle non‐numeric input.