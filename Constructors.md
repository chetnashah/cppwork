https://www.youtube.com/watch?v=ZfP4VAK21zc

Credit: Timur Doulmer for this image:
![init table](images/init_in_cpp17_table.png "Init table")


Move semantics: https://www.youtube.com/watch?v=vLinb2fgkHk

## User vs compiler declared constructors

![table](images/uservscompilerdeclaredconstructors.png)

`Note`: no `new` keyowrd is needed to construct on stack, `new` keyword is only needed to construct on heap and get a pointer.


## all constructors init example

```cpp

#include<iostream>
#include<utility>

struct S {
    int x;

    S(int new_x) : x(new_x) {
        std::cout << "S(int new_x) constructor called" << std::endl;
    }

    // copy constructors are declared by const reference
    S(const S& some_s) : x(some_s.x) {
        std::cout << "S(const int& new_x)  copy constructor called" << std::endl;
    }

    // move constructors are declared by rvalue reference
    S(S&& moved_s) : x(moved_s.x) {
        std::cout << "S(int&& new_x) move constructor called" << std::endl;
    }
};

int main(){
    S s1{1}; // invokes direct/universal constructor
    S s2 = s1; // invokes copy constructor
    S s3(std::move(s1)); // invokes move constructor
    return 0;
}
```


## Types of initialization invocations

Depending on context, the initializer may invoke:

1. Value/uniform initialization, e.g. `std::string s{};`
2. Direct initialization, e.g. `std::string s("hello");`
3. Copy initialization, e.g. `std::string s = "hello";`
4. List initialization, e.g. `std::string s{'a', 'b', 'c'};`
5. Aggregate initialization, e.g. `char a[3] = {'a', 'b'};`
6. Reference initialization, e.g. `char& c = a[0];`


### How to write a constructor ?

1. access will usually be public, i.e. in public section
2. no return value
3. same name as class
4. optionally can have member initializer list
5. constructor body `{ ... }`

```cpp
	class Dummy {
	public: // 1.
		Dummy() { // 2,3,5
			std::cout << "Dummy Default constructor" << std::endl;
		}
	};

	Dummy dummy;
```

### Default constructors
A “default constructor” is a constructor that can be called with no arguments.
Constructors with no arguments needed (this can also be constructors that have default values for all arguments).

```cpp
class Dummy {
public:
	Dummy() {
		std::cout << "Dummy Default constructor" << std::endl;
	}
};

Dummy dummy;// "Dummy Default constructor" called
```

**Note**: 
1. Only one default constructor is allowed.
2. if no other constructors are specified, compiler generates/adds a default constructor, but the default constructor will not do any initialization.
3. default constructor is invalid, when only user defined non-default constructors are specified.

Default constructors can be deleted by calling `ConstructorName() = delete`, to prevent users from constructing class instance.

#### Default constructor with default values

```cpp
class Dummy {
		int m{};
	public:
		Dummy(int mparam = 2) : m{ mparam } {

		}
		void print() {
			std::cout << m << endl;
		}
	};
```

### Aggregate initialization

Occurs when using `= {Args}` or `o {args}`(since c++11, o being aggregate)

Aggregate initialization initializes aggregates. It is a form of list-initialization (since C++11) or direct initialization (since C++20)

means the use of brace-enclosed initializer lists to initialize all members of an aggregate (i.e. an array or struct [in C++, only certain types of structs count as aggregates]).
```
T object = {arg1, arg2, ...};	(1)	
T object {arg1, arg2, ... };	(2)	(since C++11)
T object = { .designator = arg1 , .designator { arg2 } ... };	(3)	(since C++20)
T object { .designator = arg1 , .designator { arg2 } ... };	(4)	(since C++20)
T object (arg1, arg2, ...);	
```
```cpp
struct Foo{
	int i;
	float j;
}
Foo f = {1, 3.14f};// aggregate initialization
```
An aggregate is one of the following types:
* array type
* class type (typically, struct or union), that has
	- no private or protected direct (since C++17)non-static data members
no user-declared constructors(until C++11)
	- no user-provided constructors (explicitly defaulted or deleted constructors are allowed)(since C++11)(until C++17)

	- no user-provided, inherited, or explicit constructors (explicitly defaulted or deleted constructors are allowed)(since C++17)(until C++20)

	- no user-declared or inherited constructors(since C++20)
	- no virtual, private, or protected (since C++17) base classes
	- no virtual member functions
	- no default member initializers(since C++11)(until C++14)

#### Nested aggregate initialization

```cpp
struct Foo{
	int i;
	int j;
}
struct Bar{
	Foo f;
	int k;
}
int main(){
	Bar b = {{1,2}, 1};
	return b.k;// 1
}
```

### Copy constructor

A copy constructor of class T is a non-template constructor whose first parameter is `T&‍`, `const T&‍`, `volatile T&‍`, or `const volatile T&‍`, and either there are no other parameters, or the rest of the parameters all have default values.

A copy constructor is a special type of constructor used to create a new object as a copy of an existing object. (possibly deep copy recommended).

The copy constructor is called whenever an object is initialized (by direct-initialization or copy-initialization) from another object of the same type (unless overload resolution selects a better match or the call is elided), which includes

1. `initialization`: `T a = b`; or `T a(b)`;, where `b` is of type `T`;
2. `function argument passing`: `f(a)`;, where `a` is of type `T` and `f` is `void f(T t)`;
3. `function return`: `return a;` inside a function such as `T f()`, where `a` is of type `T`, which has `no move constructor`.


**Note**- If you do not provide a copy constructor for your classes, C++ will create a public copy constructor for you, which does a shallow mem cpy of members.

#### copy constructor vs copy assignment operation

a `T x = t` will invoke **copy constructor**, because `x` is being constructed for first time.
a `x = t` will invoke **copy assignment operator** because `x` is already constructed.


#### What does copy constructor signature look like
```cpp
Fraction(const Fraction& fraction){

}
```

### Memberwise Initialization

`Memberwise initialization` simply means that each member of the copy is initialized directly from the member of the class being copied.

### Uniform initialization

Using braces `{}` for initialization,also called `list initialization`.
The braces are also called `braced-init-list` in the spec.

* For aggregate types:
	- aggregate init
* For built-in types:
	- `{a}` is direct init, `= {a}` is copy init
* for class types
	- First greedily try to call a ctor that takes a std::initializer_list
	- If there is none, direct-init (or copy-init if `={a}` and a is single element)

Comes in two flavors:
1. direct-list-initialization
```cpp
Foo foo{1, 2};
```
2. copy-list-initialization
```cpp
Foo f = {1,2};
```
* Uniform initialization prevents implicit narrowing.
E.g. converting floating point to integral values or unsigned to singed
```cpp
// Compiler ERror! converting from double to int needs a narrowing conversion!
int jj{ 4.2 };
```

### Direct initialization

Using round braces `()` for initialization
e.g. 
```cpp
Foo foo(1,2);
int i(3);
```

Compared to copy initialization, this approach
* does not perform a conversion sequence
* can take more than one arguments
* works fine when constructors are declared explicit

PRoblems:
**Most vexing parse**
```cpp
struct Foo{}

struct Bar {
	Bar(Foo){}
}

int main(){
	Bar bar(Foo()); // this is a function declaration, not an initialization :(
}
```

### Value initialization

```
Classname c = ClassName()
```
also has most vexing parse problem.
Not commonly used.

### copy initialization

Occurs when using `= value`, `pass by value`, `return by value`
```
T object = other;	(1)	
T object = {other};	(2)	(until C++11)
f(other)	(3)
return other;	(4)
T array[N] = {other};	(6)	
```
If `T` is a class type and tthe type of `other` is `T` or a class `derived from T`, the `non-explicit constructors of T` are examined and the `best match` is selected by overload resolution. The constructor is then called to initialize the object.

```cpp
int main()
{
    Fraction six = Fraction(6);// implicit conversion to Fraction six(Fraction(6)); which will invoke copy constructor
    std::cout << six;
    return 0;
}
```

Avoid using copy initialization, and use uniform initialization instead.

**Note** - When you pass or return a class by value, that process uses copy initialization.



### default initialization (no initializer)

* variables defined outside function body are given value 0.
* variables of built-in types defined inside a function are unintialized. (random value)
```cpp
int main( ){
    int lll;
    std::cout <<lll <<std::endl;// random value
    return 0;
}
```
* Objects of class type that we do not explicitly initialize have a value that is defined by the class.



### declarations and definitions

variables must be defined once, can be declared many times
```cpp
extern int i; // declaration only, definition elsewhere
int j; // declaraiton and definition
```


### initializer-list based constructors
When a compiler sees an initializer list e.g. `MyIntArray arr{ 5, 4, 3, 2, 1 }; // initializer list`, it automatically converts it into an object of type `std::initializer_list`. Therefore, if we create a constructor that takes a `std::initializer_list` parameter, we can create objects using the initializer list as an input.

`std::initializer_list` lives in the `<initializer_list>` header.


Main usage:
```cpp
#include "Vector.h"

int main()
{
	Vector v1{ 2,3,4,5};
	std::cout << "SEcond element is : " << v1[1] << std::endl;
}
```

```cpp
#pragma once
#include <initializer_list>
//expose vector interface
class Vector {
public:
	Vector(int s);
	Vector(std::initializer_list<double> els);
	double& operator[](int i);
	int size();
private:
	int sz;
	double* elem;
};
```
Implementation:
```cpp
#include "Vector.h"

Vector::Vector(int s) :elem {new double[s]}, sz{s} {}

Vector::Vector(std::initializer_list<double> els)
{
	elem = new double[els.size()];
	int j = 0;
	for (const double* d = els.begin(); d != els.end(); ++d,j++) {
		elem[j] = *d;
	}
}

double& Vector::operator[](int i) {
	return elem[i];
}

int Vector::size() {
	return sz;
}
```

#### Corner cases
`auto` used with `=` and brace initialiser `{}`
```cpp
// assignment operator has different meaning for curly braces initialization of primitive
auto i = { 2 };// type of i is std::initializer_list<int>
auto i2 { 3 }; // type of i2 is int
```


### Constructor matching with initialiser_list

`std::initializer_list<>` constructors have higher priority but the `default constructor` has the highest priority.

```cpp
class X {
	X (int i, const std::string& s);// 1st constructor
	X (const std::string& const std::string& s2);// 2nd constructor
	X (std::initializer_list<int> i1);// 3rd constructor
}

X a(7,"hi");// 1st constructor matches
X b("hi","guy")// 2nd constructor matches
X c(42,43,33); // 3rd constructor matches
X d{}; // 3rd constructor matches

// X e; // not allowed because no default constructor available
```

## Overload resolution of constructors


## Public vs private constructors

Here's a comparison of use cases for public vs private constructors:

Public Constructors:
```cpp
class UserProfile {
public:
    // Public constructor when:
    // 1. Direct object creation is safe and desired
    // 2. No special initialization logic needed
    // 3. All parameters can be validated simply
    UserProfile(std::string name, int age) 
        : name(name), age(age) {}

private:
    std::string name;
    int age;
};

// Usage:
UserProfile user("John", 25); // Direct creation is fine
```

Private Constructors:
```cpp
class DatabaseConnection {
private:
    std::string connectionString;
    bool isConnected;

    // Private constructor when:
    // 1. Complex initialization required
    // 2. Resource management needed
    // 3. Creation should be controlled/validated
    DatabaseConnection(const std::string& connStr) 
        : connectionString(connStr), isConnected(false) {}

public:
    static std::shared_ptr<DatabaseConnection> create(const std::string& config) {
        // Validate configuration
        if (!isValidConfig(config)) {
            throw std::invalid_argument("Invalid configuration");
        }
        
        // Create connection string
        std::string connStr = buildConnectionString(config);
        
        // Create and return connection
        return std::shared_ptr<DatabaseConnection>(
            new DatabaseConnection(connStr)
        );
    }

private:
    static bool isValidConfig(const std::string& config) {
        // Validation logic
        return true;
    }

    static std::string buildConnectionString(const std::string& config) {
        // Build connection string
        return "connection_string";
    }
};

// Usage:
auto db = DatabaseConnection::create("config.json");
```

Here are specific scenarios for each:

Public Constructors Use Cases:
```cpp
// 1. Simple Data Objects
class Point {
public:
    Point(int x, int y) : x(x), y(y) {}
private:
    int x, y;
};

// 2. Value Objects
class Money {
public:
    Money(double amount, std::string currency) 
        : amount(amount), currency(currency) {}
private:
    double amount;
    std::string currency;
};

// 3. Immutable Objects
class Configuration {
public:
    Configuration(std::string host, int port) 
        : host(host), port(port) {}
    
    std::string getHost() const { return host; }
    int getPort() const { return port; }
private:
    const std::string host;
    const int port;
};
```

Private Constructors Use Cases:
```cpp
// 1. Singleton Pattern
class Logger {
private:
    Logger() {}
    static Logger* instance;

public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }
};

// 2. Factory Pattern
class Document {
private:
    Document(const std::string& type) {}
    friend class DocumentFactory;

public:
    static Document createPDF() {
        return Document("PDF");
    }
    
    static Document createWord() {
        return Document("Word");
    }
};

// 3. Object Pool
class ConnectionPool {
private:
    ConnectionPool(size_t size) {}
    std::vector<Connection> connections;

public:
    static ConnectionPool& init(size_t size) {
        static ConnectionPool pool(size);
        return pool;
    }
};

// 4. Builder Pattern
class Computer {
private:
    Computer() {}
    friend class ComputerBuilder;

    std::string cpu;
    int ram;
    int storage;

public:
    class ComputerBuilder {
    public:
        ComputerBuilder() : computer(new Computer()) {}
        
        ComputerBuilder& setCPU(const std::string& cpu) {
            computer->cpu = cpu;
            return *this;
        }
        
        ComputerBuilder& setRAM(int ram) {
            computer->ram = ram;
            return *this;
        }
        
        Computer build() {
            return *computer;
        }
        
    private:
        Computer* computer;
    };
};
```

Guidelines for choosing:

Use Public Constructors when:
1. Object creation is straightforward
2. No special initialization required
3. All parameters can be validated easily
4. Direct instantiation is safe
5. No need to control number of instances

Use Private Constructors when:
1. Implementing design patterns (Singleton, Factory, Builder)
2. Complex initialization logic required
3. Need to control object creation
4. Resource management is important
5. Want to enforce creation through factory methods
6. Need to maintain object pools
7. Want to ensure proper initialization sequence

