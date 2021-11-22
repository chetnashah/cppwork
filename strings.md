
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

### `sizeof` vs `strlen` on `char array` vs `char pointer`

`sizeof` ignores `char pointer`, and includes `null terminator` for `char arrays`.

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
