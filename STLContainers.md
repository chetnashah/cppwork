
## VEctors

* `emplace` (Since c++ 11): The container is **extended by inserting a new element at position, shifting others at its position** This new element is constructed in place using args as the arguments for its construction.

This effectively increases the container size by one.

Returns: An iterator that points to the newly emplaced element.

```cpp
// vector::emplace
#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> myvector = {10,20,30};

  auto it = myvector.emplace ( myvector.begin()+1, 100 );// 10 100, 20, 30
  myvector.emplace ( it, 200 );// 10, 200, 100, 20, 30
  myvector.emplace ( myvector.end(), 300 );// 10,200,100,20,30,300

  std::cout << "myvector contains:";
  for (auto& x: myvector)
    std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}
```

* `emplace_back`(Since C++11): 		
Appends a new element to the end of the container. Returns a reference to the appended element. This function can directly insert the object without calling the copy constructor.
Unlike push_back, which relies on compiler optimizations to avoid copies, emplace_back uses perfect forwarding to send the arguments directly to the constructor to create an object in-place.
The traditional wisdom is that push_back will construct a temporary object, which will then get moved into v whereas emplace_back will forward the argument along and construct it directly in place with no copies or moves.

code that we began with, `my_vec.push_back("foo")` constructs a temporary string from the string literal, and then moves that string into the container, whereas `my_vec.emplace_back("foo")` just constructs the string directly in the container, avoiding the extra move.
### push_back vs emplace_back

https://stackoverflow.com/questions/10890653/why-would-i-ever-use-push-back-instead-of-emplace-back

https://abseil.io/tips/112

