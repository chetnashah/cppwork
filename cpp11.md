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

### smart pointers and move semantics

By default `new` allocates on heap and returns a pointer to it.
It has to be manually cleaned up using `delete` operator.

Idea: Consider a class whose sole job was to hold and “own” a pointer passed to it, and then deallocate that pointer when the class object went out of scope. As long as objects of that class were only created as local variables, we could guarantee that the class would properly go out of scope (regardless of when or how our functions terminate) and the owned pointer would get destroyed.

A **Smart pointer** is a composition class that is designed to manage dynamically allocated memory and ensure that memory gets deleted when the smart pointer object goes out of scope

A sample smart pointer class:
minor problem: copy constructor does not work. i.e.
```cpp
#include <iostream>

template<class T>
class Auto_ptr1
{
	T* m_ptr;
public:
	// Pass in a pointer to "own" via the constructor
	Auto_ptr1(T* ptr=nullptr)
		:m_ptr(ptr)
	{}

	// The destructor will make sure it gets deallocated
	~Auto_ptr1()
	{
		delete m_ptr;
	}

	// Overload dereference and operator-> so we can use Auto_ptr1 like m_ptr.
	T& operator*() const { return *m_ptr; }
	T* operator->() const { return m_ptr; }
};

// A sample class to prove the above works
class Resource
{
public:
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
    void sayHi() { std::cout << "Hi!\n"; }
};

void someFunction()
{
    Auto_ptr1<Resource> ptr(new Resource()); // ptr now owns the Resource
    int x;
    std::cout << "Enter an integer: ";
    std::cin >> x;
    if (x == 0)
        return; // the function returns early
    // do stuff with ptr here
    ptr->sayHi();
}

int main()
{
    someFunction();
    return 0;
}
```
Problem case:
Because we haven’t supplied a copy constructor or an assignment operator, C++ provides one for us. And the functions it provides do shallow copies. So when we initialize res2 with res1, both Auto_ptr1 variables are pointed at the same Resource. When res2 goes out of the scope, it deletes the resource, leaving res1 with a dangling pointer. When res1 goes to delete its (already deleted)
```cpp
// problem
Auto_ptr1<Resource> res1(new Resource());
	Auto_ptr1<Resource> res2(res1); // Alternatively, don't initialize res2 and then assign res2 = res1;
```
Idea: `move semantics`:
instead of having our copy constructor and assignment operator copy the pointer (“copy semantics”), we instead transfer/move ownership of the pointer from the source to the destination object.

Add a copy constructor that implements move semantics:
```cpp
	// A copy constructor that implements move semantics
	Auto_ptr2(Auto_ptr2& a) // note: not const, because we want to modify it
	{
		m_ptr = a.m_ptr; // transfer our dumb pointer from the source to our local object
		a.m_ptr = nullptr; // make sure the source no longer owns the pointer
	}
```

Assignment operator that implements move semantics:
```cpp
	// An assignment operator that implements move semantics
	Auto_ptr2& operator=(Auto_ptr2& a) // note: not const
	{
		if (&a == this)
			return *this;

		delete m_ptr; // make sure we deallocate any pointer the destination is already holding first
		m_ptr = a.m_ptr; // then transfer our dumb pointer from the source to the local object
		a.m_ptr = nullptr; // make sure the source no longer owns the pointer
		return *this;
	}
```

#### auto_ptr

`std::auto_ptr`, introduced in C++98 and removed in C++17, was C++’s first attempt at a standardized smart pointer. `std::auto_ptr` opted to implement move semantics, but there was no clarity at the time.

#### unique_ptr

* Memory cleans up when pointer goes out of scope.
* only one pointer managing the memory.
#### shared_ptr

#### weak_ptr
