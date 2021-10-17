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

* does not have copy/assignment operator, will throw a compiler error.

The unique_ptr itself is a stack allocated, and cleans up the memory it manages when goes out of scope/stack.


**A unique_ptr can only be moved**. This means that the ownership of the memory resource is transferred to another unique_ptr and the original unique_ptr no longer owns it.

```cpp
std::unique_ptr<std::string> s = std::make_unique<std::string>("abc"); 
std::cout <<  *s<< std::endl;
    
//compiler error    
//    std::unique_ptr<std::string> s2 = s;
//    std::cout << *s2 <<std::endl;
std::unique_ptr<std::string> s2 = std::move(s); // ok, valid
```

* Remember that `std::unique_ptr` may not always be managing an object -- either because it was created empty (using the default constructor or passing in a `nullptr` as the parameter), or because the resource it was managing got moved to another `std::unique_ptr`. So before we use either of these operators, we should check whether the `std::unique_ptr` actually has a resource. 

e.g.
```cpp
	std::unique_ptr<Resource> res{ new Resource{} };

	if (res) // use implicit cast to bool to ensure res contains a Resource
		std::cout << *res << '\n'; // print the Resource that res is owning
```

Favor std::array, std::vector, or std::string over a smart pointer managing a fixed array, dynamic array, or C-style string.

* Use std::make_unique() instead of creating std::unique_ptr and using new yourself.

e.g.
```cpp
int main()
{
	// Create a single dynamically allocated Fraction with numerator 3 and denominator 5
	// We can also use automatic type deduction to good effect here
	auto f1{ std::make_unique<Fraction>(3, 5) };// same arguments as that for new
	std::cout << *f1 << '\n';

	// Create a dynamically allocated array of Fractions of length 4
	auto f2{ std::make_unique<Fraction[]>(4) };
	std::cout << f2[0] << '\n';

	return 0;
}
```

Usage with functions:
If you want the function to take ownership of the contents of the pointer, pass the `std::unique_ptr` by value. Note that because copy semantics have been disabled, you’ll need to use `std::move` to actually pass the variable in.
```cpp
void takeOwnership(std::unique_ptr<Resource> res)
{
     if (res)
          std::cout << *res << '\n';
} // the Resource is destroyed here

int main()
{
    auto ptr{ std::make_unique<Resource>() };
//    takeOwnership(ptr); // This doesn't work, need to use move semantics
    takeOwnership(std::move(ptr)); // ok: use move semantics
    std::cout << "Ending program\n";
    return 0;
}
```

#### Common issues with unique pointer

1. multiple unique pointer to same resource
```cpp
//1. multiple unique pointers managing same resource
Resource *res{ new Resource() };
std::unique_ptr<Resource> res1{ res };
std::unique_ptr<Resource> res2{ res };// this will crash due to double delte
```

2. deleting already deleted resource
```cpp
Resource *res{ new Resource() };
std::unique_ptr<Resource> res1{ res };
delete res;
```

Both of them can be fixed by manually avoiding `new`,`delete`
and use `make_unique` instead.
#### shared_ptr
 `std::shared_ptr` keeps track of how many `std::shared_ptr` are sharing the resource. 
 
 As long as at least one `std::shared_ptr` is pointing to the resource, the resource will not be deallocated, even if individual std::shared_ptr are destroyed. 
 
 As soon as the last `std::shared_ptr` managing the resource goes out of scope (or is reassigned to point at something else), the resource will be deallocated.

```cpp
#include <iostream>
#include <memory> // for std::shared_ptr

class Resource
{
public:
	Resource() { std::cout << "Resource acquired\n"; }
	~Resource() { std::cout << "Resource destroyed\n"; }
};

int main()
{
	// allocate a Resource object and have it owned by std::shared_ptr
	auto ptr1 { std::make_shared<Resource>() };
	{
		auto ptr2 { ptr1 }; // create ptr2 using copy of ptr1
		std::cout << "Killing one shared pointer\n";
	} // ptr2 goes out of scope here, but nothing happens
	std::cout << "Killing another shared pointer\n";
	return 0;
} // ptr1 goes out of scope here, and the allocated Resource is destroyed
```


#### shared_ptr <-> unique_ptr

A `std::unique_ptr` can be converted into a `std::shared_ptr` via a special `std::shared_ptr constructor` that accepts a `std::unique_ptr r-value`. The contents of the `std::unique_ptr` will be moved to the `std::shared_ptr`.

However, `std::shared_ptr` can not be safely converted to a `std::unique_ptr`. This means that if you’re creating a function that is going to return a smart pointer, you’re better off returning a `std::unique_ptr` and assigning it to a `std::shared_ptr` if and when that’s appropriate.



#### weak_ptr
`std::weak_ptr` was designed to solve the “cyclical ownership” problem described above. A `std::weak_ptr` is an observer -- it can observe and access the same object as a `std::shared_ptr`(or other `std::weak_ptrs`) but it is not considered an owner. Remember, when a `std::shared` pointer goes out of scope, it only considers whether other `std::shared_ptr` are co-owning the object. `std::weak_ptr` does not count!

Using it:
The downside of `std::weak_ptr` is that `std::weak_ptr` are not directly usable (they have no operator`->`). To use a `std::weak_ptr`, you must first convert it into a `std::shared_ptr`. Then you can use the `std::shared_ptr`. To convert a `std::weak_ptr` into a `std::shared_ptr`, you can use the `lock()` member function

### Move cleanup

As we noted in the previous lesson, it’s a good idea to always leave the objects being stolen from in some well-defined (deterministic) state. 

Ideally, this should be a `null state`, where the object is set back to its uninitiatized or zero state. Now we can talk about why: with `std::move`, the object being stolen from may not be a temporary after all. The user may want to reuse this (now empty) object again, or test it in some way, and can plan accordingly.

