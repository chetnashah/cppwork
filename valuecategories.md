
### delete nullptr is ok i.e. no-op


### lvalue

Locator value - l-values have assigned memory address.

#### lvalue reference

L-value references can only be initialized with modifiable `l-values`. i.e. does not accept temporaries.

|-- L-value reference ---|---	Can be initialized with ---|--- Can modify ---|
|Modifiable l-values	|Yes	|Yes|
|Non-modifiable l-values	|No	|No|
|R-values	|No	|No|


#### lvalue reference to const 

L-value references to const objects can be initialized with `l-values` and `r-values` alike. However, those values can’t be modified. Think of them as read-only references and are compatible with both variables and temporaries(i.e. lvalues and rvalues)

| L-value reference to const	| Can be initialized with	| Can modify|
|Modifiable l-values	|Yes	|No|
|Non-modifiable l-values	|Yes	|No|
|R-values	|Yes	|No|

### rvalue

everything that is not lvalue, e.g.
literals like `5`
temporary value slike `x + 1`
anonymous objects `Fraction(5,2)`

#### rvalue references

C++11 adds a new type of reference called an r-value reference, denoted via double ampersand `&&`. 
An r-value reference is a reference that is designed to be initialized with an r-value (only) i.e. temporaries. i.e. create overloaded functions that only accept temporaries.

| R-value reference	| Can be initialized with	|Can modify|
|Modifiable l-values	|No	|No|
|Non-modifiable l-values	|No	|No|
|R-values	|Yes	|Yes|


Rvalue references cannot be initalized with lvalues, i.e. only temporaries
```cpp
int x { 5};
int &lref {x};// lref is lvalue reference
int && rref {5}; // cannot use x here, rref is rvalue reference
rref = 7; // free to modify values

int&& rr2 {rref};  // compiler error
```

r-value references extend the lifespan of the object they are initialized with to the lifespan of the r-value reference.
 r-value references allow you to modify the r-value

typical use case is function overloads:
```cpp
void fun(const int &lref) // l-value arguments will select this function
{
	std::cout << "l-value reference to const\n";
}

void fun(int &&rref) // r-value arguments will select this function, temporaries select this also this is modifiable so useful
{
	std::cout << "r-value reference\n";
}

int main()
{
	int x{ 5 };
	fun(x); // l-value argument calls l-value version of function
	fun(5); // r-value argument calls r-value version of function
	return 0;
}

```

#### rvalue references to const
non modifiable rvalue references

| R-value reference to const	|Can be initialized with	|Can modify|
|Modifiable l-values	|No	|No|
|Non-modifiable l-values	|No	|No|
|R-values	|Yes	|No|

### move semantics
C++11 defines two new functions in service of move semantics: a `move constructor`, and a `move assignment operator`. Whereas the goal of the copy constructor and copy assignment is to make a copy of one object to another, the goal of the move constructor and move assignment is to move ownership of the resources from one object to another (which is typically much less expensive than making a copy).

`copy` flavors of these functions take a `const l-value reference parameter`, 
the `move` flavors of these functions use `non-const r-value reference parameters`

if we construct an object or do an assignment where the argument is an r-value, then we know that r-value is just a temporary object of some kind. Instead of copying it (which can be expensive), we can simply transfer its resources

### When are the move constructor and move assignment called?

The move constructor and move assignment are called when those functions have been defined, and the argument for construction or assignment is an r-value. Most typically, this r-value will be a literal or temporary value.

Move constructor take a non-const rvalue reference as an argument
e.g.

```cpp
	// Move constructor
	// Transfer ownership of a.m_ptr to m_ptr
	Auto_ptr4(Auto_ptr4&& a) noexcept
		: m_ptr(a.m_ptr)
	{
		a.m_ptr = nullptr; // we'll talk more about this line below
	}
```
Move assignment operator:
```cpp
	// Move assignment
	// Transfer ownership of a.m_ptr to m_ptr
	Auto_ptr4& operator=(Auto_ptr4&& a) noexcept
	{
		// Self-assignment detection
		if (&a == this)
			return *this;

		// Release any resource we're holding
		delete m_ptr;

		// Transfer ownership of a.m_ptr to m_ptr
		m_ptr = a.m_ptr;
		a.m_ptr = nullptr; // we'll talk more about this line below

		return *this;
	}
```
Usage:
```cpp

Auto_ptr4<Resource> generateResource()
{
	Auto_ptr4<Resource> res(new Resource);
	return res; // this return value will invoke the move constructor
}

int main()
{
	Auto_ptr4<Resource> mainres;
	mainres = generateResource(); // this assignment will invoke the move assignment

	return 0;
}
```


### std::move

takes in a lvalue, and converts it to rvalue, i.e. temporary, so that correct 
move constructor (which takes in rvalue reference) will be invoked(that does the stealing.)

### How do I write my move constructor?

Write a function with argument `ClassName(ClassName&& startTemp)` and do all the 
stealing, i.e. shallow copy and cleanup the other.

Goal of move constructor should be:
1. steal contents of other/temp into this.
2. leave other/temp in a valid undefined state.

### How do I write my move assignment operator?

Similar to move constructor, but properly empty out/cleanup the other reference, 
e.g. any pointers should be set to `nullptr` , so that `delete nullptr` is ok i.e. no-op.

### Difference

```cpp
MyString apple ="Apple";
MyString a2 = std::move(apple); // move constructor called.

MyString a3;
a3 = std::move(a2); // move assignment called  
```