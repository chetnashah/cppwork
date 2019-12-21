Ways of initialization:

```cpp
Depending on context, the initializer may invoke:

//Value initialization (empty necessary), e.g. 
std::string s{};
//Direct initialization, e.g. 
std::string s("hello");
std::string s2{"hey"};

//Copy initialization, e.g. 
std::string s = "hello";

//List initialization, e.g. 
std::string s{'a', 'b', 'c'};
//Aggregate initialization, e.g. 
char a[3] = {'a', 'b'};
// Reference initialization, e.g. 
char& c = a[0];
```

#### list initialization
https://en.cppreference.com/w/cpp/language/list_initialization