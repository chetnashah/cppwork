
`size_t` is `unsinged`.

### reading pointers and references

When reading always refer pointers as `pointers to` and references as `references to`.
e.g. `int *p` is `pointer to an int`
and `int &p` is `reference to an int`.
More details at http://faculty.cs.niu.edu/~mcmahon/CS241/Notes/reading_declarations.html

### new vs normal

`new` allocates on heap.
In the rest of the cases, objects are allocated on stack.

### meta variables

Start and end with `__` e.g. 
`__func__` will return you name of the current function that is running.


### sizeof operator

`sizeof` on a array would return full size (no of elements * element size),
but on a decayed pointer or a pointer returns pointer size.

### std::size vs sizeof

`std::size()` expects a "true" array name with size information bundled in

### common pointer mistakes

1. using uninitialized pointer

2. dereferencing null pointer or uninitialized pointer

3. use after free

4. double delete

5. multiple pointers having same address

6. deleting uninitialized memory

Note `delete nullptr` is fine, i.e no-op.
Note delete does not null out its operand, it instead frees memory pointed to by the operand
### Best practices with pointer

1. initialize pointers, with `nullptr` if you have not decided, else actual memoryaddress.

2. pointer to be set to `nullptr` after delete (reset).

3. `new` and `delete` should be in pairs.

4. check for `nullptr` using if before dereferencing pointer.

### One definition rule

File level `ODR`: Only one definition of any variable, function, class type, enumeration type, concept (since C++20) or template is allowed in any one translation unit (file).

Program level `ODR`: One and only one definition of every non-inline function or variable that is odr-used (see below) is required to appear in the entire program (including any standard and user-defined libraries).

Types (which include `classes`), are exempt from the part of the one-definition rule that says you can only have one definition per program. Therefore, there isn’t an issue #including class definitions into multiple code files (if there was, classes wouldn’t be of much use).
Or you can say that:
If a program contains more than one definition of a `class` (and also of an inline function, or a function template) with external linkage, all these definitions should be identical.
That is why it is ok to include headers that contain class definitions.

This is exactly what happens when you put the definition of your class in a header file and then include this header in two .cpp files. Because of how the header inclusion works in C++ (by copy-and-paste), this renders the situation where the two .cpp files define the same class. There is no other way in C++ to share class definitions between files; therefore the compiler must accept the repeated definition of a given class in different files, but on provision that you do not try to do nasty tricks, like changing the definition of the same class from file to file. You are only expected to include the same header file in each .cpp file. Anything other than that is risky.

https://stackoverflow.com/questions/4955159/is-is-a-good-practice-to-put-the-definition-of-c-classes-into-the-header-file

### implicit conversions

When you pass data of a type different than what the function takes,
the compiler will try to insert an implicit conversion from the type you pass into the type that the compiler takes.
if conversion fails, you'll get compiler error.

* if you have a function that takes non-const reference and you pass in different type as argument,
the compiler will prevent u from doing any implicit conversions

i.e `compiler won't allow implicit conversions when non const reference parameters are involved.`

```cpp
void inc_num(int& num){
    num++;
}
int main(){
    double d_var{11.0};
    inc_num(d_var); // compiler error
    return 0;
}
```

implicit conversion of pointer types - not allowed e.g. double* cannot be converted to int*.

### uninintialized values

A variable that has not been given a known value (usually through initialization or assignment) is called an uninitialized variable.

Value of `uninitialized variables is undefined and implementation dependent`.
Visual studio: it will consistently print -858993460, because that’s the value (interpreted as an integer) that Visual Studio initializes memory with in debug configurations

Most modern compilers will attempt to detect if a variable is being used without being given a value. If they are able to detect this, they will generally issue a compile-time error.

#### Pointer initialization

default brace initialization for a pointer
is nullptr
e.g.
```cpp
// both are same
int* p1 {};
int* p2 {nullptr};
p1 == p2; // true

// uninitialized pointer usage is a code smell
// should have linting to prevent this
int* p_un;
// using p_un without assignment init will result in a runtime crash
```

#### `char *` initialization with a string literal

```cpp
// p_message is start address of a char array
char* p_message { "hello world"};
char* p_m2 = "hi world"; // similar code smell
```
But this is a code smell because
`p_message[0] = 'J';` will crash because
we are trying to write into a read only string.

Better practice is to use a `const char*`
i.e.
```cpp
// read only string
const char* p { "hey world" };
```

If you want to have modifiable char arrays,
Use char arrays instead:
```cpp
char m2[] { "Hi world" };
m2[1] = 'e';// Ok 
```


#### Pointers and const

four types:
1. `non-const pointer to non-const data`: both pointer and data can change e.g. `int* j`: `pointer to int`
2. `non-const pointer to const data`: pointer can change, but pointed data cannot change e.g. `const int* j`: `non const pointer to const int`.
3. `const pointer to non-const data`: pointer cannot change, but pointed data can change e.g. `int *const j`
4. `const pointer to const data`: pointer cannot change and pointed data cannot change `const int *const j`

`*const` is basically `const pointer`, basically `if const shows up to right of *, then pointer is const`.
**Note** - constness is confined only to the pointer declaration and nothing to do with original value.

`Note`: `char const *p` and `const char *p` are exactly the same thing.

How to read declarations: http://faculty.cs.niu.edu/~mcmahon/CS241/Notes/reading_declarations.html

#### pointers and arrays

**similarity**:
The array name can be treated as pointer to first element in the array.
i.e. if you dereference array name, you will get first element of array
if you print array name, it you will see address of first element

Array braces is sugar for pointer arithmetic

**Differences**:
You can't point arrayname to something else like a regular pointer, so arrayname is more of a const pointer `*const`

`std::size` works for arrayname, but not for pointers

In essence, `array` is a const pointer i.e. `*const` with some `size` info.

**Pointer decay** example:
Case when pointer is initialized from array name
```cpp
int parr[] {1,2,3};
int *pp {parr};// parr decayed to a pointer pp, decaying because we lost size info
```

#### passing arrays to functions

Passing arrays to funtions decays the pointer at function definition side

```cpp
void arraytaker(std::string strArr[]) {// strArr is decayed into a pointer, even if there is a number in the square braces
    std::cout << strArr[0] <<std::endl;
    std::cout << sizeof(strArr) << std::endl;
}
```

e.g. xcode will show following warning:
```
Sizeof on array function parameter will return size of 'std::string *' (aka 'basic_string<char> *') instead of 'std::string []'
```

Hence recommended best practice: also pass array size as separate function parameter

Approach2: pass arrays by reference, so size and other info is retained i.e not decayed
Here numbers should match with signature
```cpp
void foo(std::string (&arr)[5])
{
    std::cout << "----foo start----" << std::endl;
    std::cout << arr[0] <<std::endl;// hi
    std::cout << sizeof(arr)/sizeof(arr[0]) << std::endl; // 5
    std::cout << "-----foo end-----" << std::endl;
}


int main(int argc, const char * argv[]) {    
    std::string arr[5] = {"hi", "hello", "how are", "what is", "jj"};
    foo(arr);
    return 0;
}
```


#### Multi dimension array function parameter


### Forward declaration.

A forward declaration allows us to tell the compiler about the existence of an identifier before actually defining the identifier.

In the case of functions, this allows us to tell the compiler about the existence of a function before we define the function’s body. This way, when the compiler encounters a call to the function, it’ll understand that we’re making a function call, and can check to ensure we’re calling the function correctly, even if it doesn’t yet know how or where the function is defined.

A forward declaration can have default values, definition can skip having default values.

```cpp
#include <iostream>
 
int add(int x, int y); // forward declaration of add() (using a function prototype)
 
int main()
{
    std::cout << "The sum of 3 and 4 is: " << add(3, 4) << '\n'; // this works because we forward declared add() above
    return 0;
}
 
int add(int x, int y) // even though the body of add() isn't defined until here
{
    return x + y;
}
```

Declaring two functions differing only in return type is not valid function overloading, and is flagged as a compile-time error
```cpp
void f();
int f();      // error, two definitions differ only in
              // return type
int g()
{
   return f();
}
```

### Determining cpp version in code

It is captured in `__cplusplus` macro by default.

```cpp
int main() {
    if (__cplusplus == 201703L) std::cout << "C++17\n";
    else if (__cplusplus == 201402L) std::cout << "C++14\n";
    else if (__cplusplus == 201103L) std::cout << "C++11\n";
    else if (__cplusplus == 199711L) std::cout << "C++98\n";
    else std::cout << "pre-standard C++\n";
}
```

### Compiler Extensions

The C++ standard defines rules about how programs should behave in specific circumstances. And in most cases, compilers will follow these rules. However, many compilers implement their own changes to the language, often to enhance compatibility with other versions of the language (e.g. C99), or for historical reasons. These compiler-specific behaviors are called compiler extensions.

Writing a program that makes use of a compiler extension allows you to write programs that are incompatible with the C++ standard. Programs using non-standard extensions generally will not compile on other compilers (that don’t support those same extensions), or if they do, they may not run correctly.

#### Expressions

C++ expressions can cause side effects e.g. `printf("x")`
Each C++ expression is charachterized by two independent properties:
a type and a value category.

Value categories(lvalue, rvalue, glvalue, prvalue, xvalue) classify
expressions by their value. (http://www.stroustrup.com/terminology.pdf)

##### Simplistic early definitions:

An lvalue (locator value) represents an object that occupies some identifiable location in memory (i.e. has an address).

rvalues are defined by exclusion, by saying that every expression is either an lvalue or an rvalue. Therefore, from the above definition of lvalue, an rvalue is an expression that does not represent an object occupying some identifiable location in memory.

##### New definitions

http://en.cppreference.com/w/cpp/language/value_category
Each expression has some non-reference type, and each expression belongs
to exactly one of three primary value categories: prvalue, xvalue and lvalue.

1. glvalue (generalized lvalue) is an expression whose evaluation determines identity of object, bit-field or function.

2. a prvalue (pure rvalue) is an expression whose evaluation either:
  - computes value of the operand of an operator
  - initializes an object or a bit-field
  - can be moved

3. an xvalue is a glvalue that denotes an object or bit-field whose resources can be reused.

* an lvalue is a glvalue that is not an xvalue.
* an rvalue is a prvalue or an xvalue.

Examples of lvalues:
* name of variable, function, or data member.
* a string literal like "hello world"

Examples of prvalue:
* a literal (except string literal) such as 42, true or nullptr

### Internal linkage

Global variables with internal linkage are sometimes called internal variables.
However, it’s worth noting that internal objects (and functions) that are defined in different files are considered to be independent entities (even if their names and types are identical), so there is no violation of the one-definition rule. Each internal object only has one definition.


```cpp
static int g_x; // non-const clobals have external linkage, but if you add static, they are given internal linkage

const int g_y {1}; // const globals have internal linkage by default.

constexpr int g_z {2}; // constexpr globals have internal linkage by default
```

Functions default to external linkage, but if used with `static` keyword, will have internal linkage
```cpp
//add.cpp
static int add(int x, int y) {
    return x + y;
}
```
```cpp
// main.cpp
#include iostream
int add(int x, int y); // forward declaration for fn
int main(){
    std::cout << add(3, 4) << std::endl;
    return 0;
}
```

### External linkage

An identifier with external linkage can be seen and used both from the file in which it is defined, and from other code files (via a `forward declaration`). In this sense, identifiers with external linkage are truly `global` in that they can be used anywhere in your program!

`constexpr` variables can’t be forward declared, even if they have external linkage

Functions have external linkage by default.
In order to call a function defined in another file, you must place a forward declaration for the function in any other files wishing to use the function. The forward declaration tells the compiler about the existence of the function, and the linker connects the function calls to the actual function definition. No need of `extern` keyword for function forward declarations.

```cpp
// a.cpp
#include <iostream>
void sayHi() {
    std::cout << "Hi";
}
```
```cpp
void sayHi(); // forward declaration makes sayHi accessible in this file
int main(){
    sayHi();
    return 0;
}
```

For variables to have external linkage, use `extern` keyword in front of a global variable in it's forward declaration.
`extern` with definition makes it eligible for export (but it is optional)
`extern` without definition is equivalent to import/forward declaration for usage.

```cpp
// a.cpp
int g_x {2}; // non-const globals have external linkage by default
extern const int g_y { 3}; // this gives const g_y an external linkage, required for const
```

```cpp
// main.cpp
#include <iostream>
extern int g_x;
extern const int g_y;

int main(){
    std::cout << g_x;
    return 0;
}
```

### Initialization order problem of global variables

Initialization of global variables happens as part of program startup,
before execution of main function.

It has two phases:
1. static initialization: Initialize global variables with constexpr initializers, global vars without initializers are zero initialized.

2. dynamic initialization: global vars with non-constexpr initializers are initialized.

Dynamic initialization of global variables causes a lot of problems in C++. Avoid it whenever possible.

### Common way to manage constants in app

```cpp
// constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H
 
// define your own namespace to hold constants
namespace constants
{
    // constants have internal linkage by default
    constexpr double pi { 3.14159 };
    constexpr double avogadro { 6.0221413e23 };
    constexpr double my_gravity { 9.2 }; // m/s^2 -- gravity is light on this planet
    // ... other related constants
}
#endif
```

### When to put code in `.h` vs `.cpp`

Code in `.h` is blindly copied into files. It will be exected any number of times, the copied destination executes. 
So `.h` usually only has forward declarations (promise of availability of implementation)

Code in `.cpp` is instantiated once (implementation available once)

the common way is and always has been to put code in `.cpp` files (or whatever extension you like) and declarations in `headers`


### Classes

https://stackoverflow.com/questions/4955159/is-is-a-good-practice-to-put-the-definition-of-c-classes-into-the-header-file

A function defined within a class definition is an inline function.


### References

Initializing references
```cpp
	int kbc = 11;
	int& kbref{ kbc };// kbref is alias to kbc
    int& kb2 = kbc; // also valid way to initialize ref
    std::cout << "Value of kbref = " << kbref << std::endl;//11
	kbref = 222;// writing via ref/alias
	std::cout << "value of kbc after changing kbref = " << kbc << std::endl;// 222
```

You can read reference `int& a` as `a is reference to int` or `a is an alias of int`
Reference cannot be uninitialized,
and reference cannot be made to refer/alias anything else once created.
https://isocpp.org/wiki/faq/references

An alias (an alternate name) for an object.
The address of a reference is the actual object's address
References are kind of like const pointers but not exactly.
References are frequently used for pass-by-reference:

```cpp
void swap(int& i, int& j)
{
  int tmp = i;
  i = j;
  j = tmp;
}
int main()
{
  int x, y;
  swap(x,y);
}
```
Here i and j are aliases for main’s x and y respectively. In other words, i is x — not a pointer to x, nor a copy of x, but x itself. Anything you do to i gets done to x, and vice versa. This includes taking the address of it. The values of &i and &x are identical.

**Note** you cannot take non-const references to const origin
```cpp
    const int age{33};
    int& age_ref{age};// compiler error
```

#### Call by value or call by reference?

That depends on what you are trying to achieve:

1. If you want to change the object passed, call by reference or use a pointer; e.g., `void f(X&);` or `void f(X*);`.
2. If you don’t want to change the object passed and it is big, call by const reference; e.g., `void f(const X&);`.
3. Otherwise, call by value; e.g. `void f(X);`.
What does “big” mean? Anything larger than a couple of words.

Why would you want to change an argument? Well, often we have to, but often we have an alternative: produce a new value. Consider:
```cpp
    void incr1(int& x); // increment
    int incr2(int x);   // increment
    int v = 2;
    incr1(v);   // v becomes 3
    v = incr2(v);   // v becomes 4
```
For a reader, `incr2()` is likely easier to understand. That is, `incr1()` is more likely to lead to mistakes and errors. So, we should prefer the style that returns a new value over the one that modifies a value as long as the creation and copy of a new value isn’t expensive.

What if you do want to change the argument, should you use a pointer or use a reference? If passing “not an object” (e.g., a null pointer) is acceptable, using a pointer makes sense. One style is to use a pointer when you want to modify an object because in some contexts that makes it easier to spot that a modification is possible.

Note also that a call of a member function is essentially a call-by-reference on the object, so we often use member functions when we want to modify the value/state of an object.

### references

* references must always be initialized, otherwise it is a compiler error.
* The address of a reference is the actual object's address
* the construction argument for a reference has to be l-value
e.g. 
```cpp
int& j { 2}; // compiler error because 2 is not a l-value
// but an r-value can be passed to const ref
const int& k { 3}; // ok
```

* references cannot be reassigned, once initialized they cannot serve as alias for other variables.

e.g. 
```cpp
    int v1 = 5;
    int v2 = 6;
    int& r1 {v1};
    r1 = v2;
    std::cout << r1 << v1 << v2 << std::endl;

//Instead of changing ref to reference variable v2, it assigns the value of v2 to v1
```

#### const references

A reference to a const value is often called a const reference for short.

YOu can declare const ref to a non-const value but not the other way:
```cpp
    int c1 = 2;
    const int& cr1 { c1};// ok, cr1 considered read only reference/alias of c1
    c1 = 11;// c1 is still writable. only the above ref is read only

    const int c2 = 3;
    int& cr2 {c2};// compiler error because cr2 can change things

    // also works with r-value in constructor
    const int& ref3{ 6 }; // okay, 6 is an r-value
```

#### references to const initialized via r-values, extend lifetime of r-values

```cpp
int somefcn()
{
    const int& ref{ 2 + 3 }; // normally the result of 2+3 has expression scope and is destroyed at the end of this statement
    // but because the result is now bound to a reference to a const value...
    std::cout << ref << '\n'; // we can use it here
} // and the lifetime of the r-value is extended to here, when the const reference dies
```

#### const refs as function parameters

THis is same as read only reference passed to function, where function will not be able to modify.

 A const reference parameter allows you to pass in a non-const l-value argument, a const l-value argument, a literal, or the result of an expression.

 ```cpp
 #include <iostream>

void printIt(const int& x)
{
    std::cout << x;
}

int main()
{
    int a{ 1 };
    printIt(a); // non-const l-value

    const int b{ 2 };
    printIt(b); // const l-value
    printIt(3); // literal r-value
    printIt(2+b); // expression r-value
    return 0;
}
 ```


### Constructors

`Constructors` are present to initialize members, no need to returna anything from constructors.


### Member initializer list
The member initializer list is inserted after the constructor parameters. It begins with a colon (:), and then lists each variable to initialize along with the value for that variable separated by a comma.

Use member initializer lists to initialize your class member variables instead of assignment.


```cpp
class Something
{
private:
    int m_value1 {};
    double m_value2 {};
    char m_value3 {};

public:
    Something() : m_value1{ 1 }, m_value2{ 2.2 }, m_value3{ 'c' } // Initialize our member variables
    {
    // No need for assignment here
    }

    void print()
    {
         std::cout << "Something(" << m_value1 << ", " << m_value2 << ", " << m_value3 << ")\n";
    }
};

int main()
{
    Something something{};
    something.print();
    return 0;
}
```

For arrays:
In cpp, array symbol `[]` always goes after variable name.
```cpp
class Something
{
private:
    const int m_array[5];

public:
    Something(): m_array { 1, 2, 3, 4, 5 } // use uniform initialization to initialize our member array
    {
    }
};
```

###  const initialization in classes

Assignment not allowed for initialization
```cpp
class Something{
    private:
        const int j;
    public:
        Something(int x) {
            j = x; // compiler error
        }
};
```

Correct way: **No assignment, only member initialization**
```cpp
class Something{
    private:
         const int j;
    public:
        Something(int x) : j { x }
        {}
};
```


### copy constructor

#### default copy constructor

just does a memberwise shallow copy

#### providing custom copy constructor

A constructor with `const ClassName& other` argument

```cpp
class Something{
    private:
         const int j;
    public:
        Something(int x) : j { x }
        {}

        Something(const Something& other): j { other.j }
        {}
};
```

### memory leak

Memory dynamically allocated by our app, but we have lost
access to memory (for e.g.  via reassigning pointer)
or double allocation for same pointer

* scoped mem leak:
```cpp
int main(){
    {
        int *pnum { new int{57}};
    }// mem with int{57} leaked since we did not delete pnum
    return 0;
}
```

### Exception handling

#### Using `try/catch`

Generic way to handle with try/catch

```cpp
try{
    doSOmething();
} catch(std::exception& ex) {
    std::cout << ex.what() <<endl;
}
```

### Array heap vs stack allocation

You can create an array of objects on the stack† via:
```cpp
myarray stackArray[100]; // 100 objects
And on the heap† (or "freestore"):
```

```cpp
myarray* heapArray = new myarray[100];// note: always need to use pointer to get new result
delete [] heapArray; // when you're done
```
But it's best not manage memory yourself. Instead, use a std::vector:
```cpp
#include <vector>
std::vector<myarray> bestArray(100);
```
A vector is a dynamic array, which (by default) allocates elements from the heap


### copy constructor vs assignment

`=` can invoke either `copy assignment operator` or `copy constructor` depending on whether the variable being declared is initialized already or not.

```cpp
class Widget {
public:
	Widget() = default;
	Widget(const Widget& other) { // read only alias to other is provided in copy constructor
		std::cout << "Copy constructor" << std::endl;
	}
	Widget& operator=(Widget& other) {
		std::cout << "Copy assignment operator" << std::endl;
		return other;
	}
};

Widget w1; // default constructor
Widget w2(w1); //copy constructor
Widget w3{ w1 }; // copy constructor

Widget w4 = w1; // copy constructor, because w4 is being initialized first time
Widget w5;// default constructor
w5 = w1; // copy assingment operator
```

### pass by value invokes copy constructors

Unless you pass a reference of an object, pass by value happens 
and copy constructor is invoked on the function param side.

Pass by reference to const, is an alternative choice, if function
needs it for read only purposes and/or copy might be expensive'

### Returning references to local variables from functions is a bad practice, similarly returning pointers to local vars is bad practice

function local vars are allocated on stack and typically do not live longer than the duration of the function call.

Can result in a crash:
```cpp
// warning:: Reference to stack memory associated with local variable 'result' returned
int& sum (int a, int b) {
    int result = a+b;
    return result;
}
```


### consteval vs constexpr

`consteval` creates a so-called immediate function. Each invocation of an immediate function creates a compile-time constant. To say it more directly. A consteval (immediate) function is executed at compile-time.
```cpp
#include <iostream>

consteval int sqr(int n) {
    return n * n;
}

int main() {
    std::cout << "sqr(5): " << sqr(5) << std::endl;     // (1)
    const int a = 5;                                    // (2)
    std::cout << "sqr(a): " << sqr(a) << std::endl;     
    int b = 5;                                          // (3)
    // std::cout << "sqr(b): " << sqr(b) << std::endl; ERROR
}
```

consteval cannot be applied to destructors or functions which allocate or deallocate
`consteval` - It declares immediate functions, that is, functions that must be evaluated at compile time to produce a constant. 
(It used to be spelled constexpr! in a previous revision of the paper.) 
In contrast, `constexpr` functions may be evaluated at compile time or run time, and need not produce a constant in all cases.

`constinit` - constinit can be applied to variables with static storage duration or thread storage duration.
