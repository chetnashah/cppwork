
## installing folly

```bash
brew install folly
```

## compiling and linking with folly

`-L` is used to specify the path to the library and `-I` is used to specify the path to the header files.

Since we installed through brew, the path to the library is `/opt/homebrew/lib` and the path to the header files is `/opt/homebrew/include`.

```bash
clang++ follyexample.cpp  -lfolly -L/opt/homebrew/lib -I/opt/homebrew/include -std=c++17 -o follyexample
```

## Code

```cpp
#include<iostream>
#include<folly/dynamic.h>

int main() {
	folly::dynamic twelve = 12;
	folly::dynamic str = "string";

	std::cout << " str is " << str << std::endl;
	return 0;
}
```