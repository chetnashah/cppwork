https://learn.microsoft.com/en-us/cpp/cpp/how-to-create-and-use-weak-ptr-instances?view=msvc-170

https://www.youtube.com/watch?v=dNaQoz1jCCg

**It is a non-owning smart pointer!**

Sometimes an object must store a way to access the underlying object of a shared_ptr without causing the reference count to be incremented. Typically, this situation occurs when you have cyclic references between shared_ptr instances.

1. The best design is to avoid shared ownership of pointers whenever you can. 
2. However, if you must have shared ownership of shared_ptr instances, avoid cyclic references between them. 
3. When cyclic references are unavoidable, or even preferable for some reason, use weak_ptr to give one or more of the owners a weak reference to another shared_ptr.


## Semantics

By using a weak_ptr, you can create a shared_ptr that joins to an existing set of related instances, but only if the underlying memory resource is still valid.

**A weak_ptr itself does not participate in the reference counting, and therefore, it cannot prevent the reference count from going to zero.**

## Use `expired()` to check if referenced object is deleted

```cpp
#include <iostream>
#include <memory> // for std::shared_ptr and std::weak_ptr

int main() {
    auto ptr1 { std::make_shared<int>(100) };
    std::weak_ptr<int> ptr2 { ptr1 };

    ptr1.reset(); // release ownership from ptr1

    if (ptr2.expired()) {
        std::cout << "Resource is no longer available\n";
    } else {
        std::cout << "Resource still available\n";
    }

    return 0;
}
```