
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
2. if no constructors are specified, compiler adds a default constructor
3. default constructor is invalid, when only user defined non-default constructors are specified.

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

### Copy constructor

A copy constructor of class T is a non-template constructor whose first parameter is `T&‍`, `const T&‍`, `volatile T&‍`, or `const volatile T&‍`, and either there are no other parameters, or the rest of the parameters all have default values.

A copy constructor is a special type of constructor used to create a new object as a copy of an existing object.

The copy constructor is called whenever an object is initialized (by direct-initialization or copy-initialization) from another object of the same type (unless overload resolution selects a better match or the call is elided), which includes

1. `initialization`: `T a = b`; or `T a(b)`;, where `b` is of type `T`;
2. `function argument passing`: `f(a)`;, where `a` is of type `T` and `f` is `void f(T t)`;
3. `function return`: `return a;` inside a function such as `T f()`, where `a` is of type `T`, which has `no move constructor`.


**Note**- If you do not provide a copy constructor for your classes, C++ will create a public copy constructor for you, which does a shallow mem cpy of members

#### What does copy constructor signature look like
```cpp
Fraction(const Fraction& fraction){

}
```

### Memberwise Initialization

`Memberwise initialization` simply means that each member of the copy is initialized directly from the member of the class being copied.

### Uniform initialization

Using braces `{}` for initialization

### Direct initialization

Using round braces `()` for initialization


### copy initialization

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

