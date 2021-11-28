
### string literal

Type for string literal is `const char *`

### char array representation

```cpp
char message[] {"hi world"};// note [] after variable name instead of after char like java
char msg2[] {'H','e', 'y','\0'};// same as above
```

### const char* representation

```cpp
const char* mm = "hey world";// no need to delete this, since no new was used
cout << mm << endl;
```

### std::string

Convinience class to be used instead of managing raw string
data via `char arrays or pointers`. Can also be used in range based for loop.

Comes from `#include <string>`.

Has useful convinience operators like `+` for concatenating strings.

1. convert `std::string` to a `const char *` string - use `str.c_str()`.
If you need modifiable `char *`, you should use `str.data()`

Note: `std::string.size()` excludes `null terminator in size`. Also `length()` is exactly same as `size()`

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
