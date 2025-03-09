
## Stream boolean checking

This is for conditions like `if(file)` or `while(mystream)`.

When you use a stream object in a boolean context (like a condition), it is implicitly converted to a boolean value that represents the state of the stream.
1. If the stream is in a good state (no errors and not at the end), it converts to true.
2. If the stream has reached the end (EOF) or encountered an error, it converts to false.

## Extraction `<<` and Insertion `>>` operators

1. `<<` (Insertion Operator): Sends data to an output stream and supports chaining by returning a reference to the same stream.
2. `>>` (Extraction Operator): Reads data from an input stream into variables and supports chaining similarly.

You can use boolean checking as mentioned in the earlier section on the returned stream.

### when to use which operator?

Here's a simple way to remember:

- **`<<` (Insertion Operator):**  
  Think of it as “pushing data **out** of your program.” When you write:  
  ```cpp
  std::cout << "Hello!";
  ```  
  The arrow points left, so the data flows out from the right side into the stream on the left.

- **`>>` (Extraction Operator):**  
  Think of it as “pulling data **in** to your program.” When you write:  
  ```cpp
  std::cin >> myVar;
  ```  
  The arrow points right, meaning the data is extracted from the stream on the left and stored into your variable on the right.

**Mnemonic:**  
- For output, remember **"out goes <<"**  
- For input, remember **"in comes with >>"**

These directional hints should make it easier to keep track of which operator to use!

## getLine()

Available in `#include <string>` header. 

`getline` reads characters from an input stream and places them into a string

**Syntax: `std::getline(input2, line, ';')`, where third optional argument is the separator**

### Parameters
`input`	-	the stream to get data from
`str`	-	the string to put the data into
`delim`	-	the delimiter character

### Return value
`input` - either ignored or useful for re-reading.

```cpp
#include <sstream>
#include <string>
 
int main()
{
    // greet the user
    std::string name;
    std::cout << "What is your name? ";
    std::getline(std::cin, name);
    std::cout << "Hello " << name << ", nice to meet you.\n";
 
    // read file line by line
    std::istringstream input;
    input.str("1\n2\n3\n4\n5\n6\n7\n");
    int sum = 0;
    for (std::string line; std::getline(input, line);)
        sum += std::stoi(line);
    std::cout << "\nThe sum is " << sum << ".\n\n";
 
    // use separator to read parts of the line
    std::istringstream input2;
    input2.str("a;b;c;d");
    for (std::string line; std::getline(input2, line, ';');)
        std::cout << line << '\n';
}
```