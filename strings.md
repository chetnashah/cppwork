
### string literal

Type for string literal is `const char *`

`strlen` is linear time function.

The string object itself is stored on the stack but it points to memory that is on the heap.
But there is an optimization, short string optimization where the data is inline, instead on the heap.


### char array representation

```cpp
char message[] {"hi world"};// note [] after variable name instead of after char like java
char msg2[] {'H','e', 'y','\0'};// same as above
```
for length prefer `strlen`, nothing else works e.g. `sizeof` does not work
reason:
```cpp
char abc[55] = "hello ";

std::cout << "sizeof(abc) " << sizeof(abc) << std::endl; // 55
std::cout << "sizeof(abc)/sizeof(abc[0])" << sizeof(abc)/sizeof(abc[0]) << std::endl; // 55/1 = 55
```

```cpp
char message[] {"hi world"};

std::cout << sizeof(message) << std::endl;// 9 includes null terminator
std::cout << strlen(message) << std::endl;// 8 excludes null terminator
```


### const char* representation

```cpp
const char* mm = "hey world";// no need to delete this, since no new was used
cout << mm << endl;
```
for length, prefer `strlen` nothing else works, `sizeof` does not work

### C-String

C-strings are simply implemented as a `char array` which is terminated by a null character (aka 0). 
This last part of the definition is important: all C-strings are char arrays, but not all char arrays are c-strings


### String literal

String literals are indicated by using the double quote (") and are stored as a constant `(const) C-string`.
The null character is automatically appended at the end for your convenience.

```cpp
const char * str = "This is a string literal. See the double quotes?";
```

### std::string

Convinience class to be used instead of managing raw string
data via `char arrays or pointers`. Can also be used in range based for loop.

Comes from `#include <string>`.

Has useful convinience operators like `+` for concatenating strings.

1. convert `std::string` to a `const char *` string - use `str.c_str()`.
If you need modifiable `char *`, you should use `str.data()`

Note: `std::string.size()` excludes `null terminator in size`. Also `std::string.length()` is exactly same as `std::string.size()`

`str.append(str2/cstr)` to append/concat strings 

`str.empty()` returns a boolean telling if string is empty or not.

`str.capacity()` returns size of internal container array.

Max possible chars possible in a string `str.max_size()`

`str.clear()` cleans up its internal data, but capacity remains the same.

`str.insert(pos, str2)` inserts `str2`(can be either std::string or cstring) starting at `pos`

`str.replace()` - many overload utils

`str.copy(cstrdest, count, pos)` - copy subsection of given `str` to `cstrdest`. `cstrdest` is preallocated.

`str.resize(count)` -

`str_a.swap(str_b)` - 


#### std::string equality

If two strings have same size and 
same charachters in same positions.

For comparision and equality,
`std::string` has standard operators:
`==`, `<=`,`>=`, `!=`, and it can also take in cstrings.

* Comparing `std::string` with `std::string::compare`  - Returns negative, zero or positive value basis lexicographic comparision.

#### std::string searching

`str1.find(search_for)` - return index where `search_for` is found in `str1`, if not found, it will return `std::string::npos`
```cpp
	std::string s1{ "Hello" };
	std::string s2{ "Ehllo world" };
	size_t idx = s2.find(s1);
	if (idx <= s2.size()) {
		std::cout << "found substring: " << idx << std::endl;
	}
	else {
		std::cout << "did not find string" << idx << std::endl;
		std::cout << "did not find substring...: " << std::string::npos << std::endl;
	}
```




### `sizeof` vs `strlen` on `char array` vs `char pointer`

`sizeof` ignores `char pointer` (returns pointer size), and includes `null terminator` for `char arrays`.

`strlen` excludes `null terminator`, and is more reliable, works on both `char array` and `char pointer`.
```cpp
	// strlen vs sizeof
	// also on pointer vs char array
	const char m1[]{ "hello" };
	const char* mp1 = "hey there, what are you what is happening";

	std::cout << "strlen(m1):" << strlen(m1) << std::endl;// 5, excludes null char
	std::cout << "sizeof(m1):" << sizeof(m1) << std::endl;// 6, includes null char

	std::cout << "strlen(mp1):" << strlen(mp1) << std::endl;// 41, excludes null char
	std::cout << "sizeof(mp1):" << sizeof(mp1) << std::endl;// 8, pointer size
```

### converting to and from strings

Use `std::to_string(val)` to coerce any value to a string.
To convert from string to other types use `stoX` set of functions.
```cpp
	std::cout << std::string{ "Hi " }.append(std::to_string(11.11f)) << std::endl;

	float f = 11.2f;
	f += std::stof("2.2f");

	std::cout << std::string{ "AFter addition: " } << f << std::endl;
```

### raw string literals

prefix a quoted string with `R"(actualstring)"` to signify it as a raw string literal.
```cpp
std::string m_str{ R"(abc)" };
std::string todo_list = R"( // can be useful for multiline strings
	Hey 
	hello world
)"
```

### Saving memory with `std::string_view`

introduced in `C++17`

```cpp
#include <iostream>
#include <string_view>

int main()
{
  std::string_view text{ "hello" }; // view the text "hello", which is stored in the binary
  std::string_view str{ text }; // view of the same "hello"
  std::string_view more{ str }; // view of the same "hello"

  std::cout << text << ' ' << str << ' ' << more << '\n';

  return 0;
}

### std::string_view

If you are using C++17, you can avoid memory allocation and still enjoy the C++ string interfaces by using `std::string_view`. 
The entire purpose of `std::string_view` is to avoid copying data which is already owned and of which only a fixed view is required. 
A `std::string_view` can refer to both a C++ string or a C-string. All that `std::string_view` needs to store is a pointer to the character sequence and a length.

`std::string_view` provides the same API that std::string does, 
so it is a perfect match for C-style string literals.

The only catch with `std::string_view` is that it is non-owning, 
so the programmer is responsible for making sure the `std::string_view` does not outlive the string which it points to

### Return value optimizations (RVO) for string return

```cpp
std::string stringadder(std::string a, std::string b){
    std::string result = a + b;
    std::cout << "address of result = " << &result << std::endl;
    return result;
}

int main(int argc, const char * argv[]) {
    std::string res = stringadder("hi ", "hello");
    std::cout << "in main res address: " << &res << std::endl;
    return 0;
}
// address of result = 0x16fdff298
// in main res address: 0x16fdff298
```