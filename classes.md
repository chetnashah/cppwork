

There can be more than one definition of a class type, enumeration type, inline function with external linkage, class template, non-static function template, static data member of a class template, member function of a class template, or template specialization for which some template parameters are not specified in a program provided that each definition appears in a different translation unit, and provided the definitions satisfy the following requirements. Given such an entity named D defined in more than one translation unit....

If the definitions of D satisfy all these requirements, then the program shall behave as if there were a single definition of D. If the definitions of D do not satisfy these requirements, then the behavior is undefined.

The requirements specified by paragraph 6 essentially state that that two definitions must be identical (not simply equivalent). Consequently, a definition introduced in two separate translation units by an #include directive generally will not violate the ODR because the definitions are identical in both translation units.



### Classes vs structs

They are same but only difference is for struct default visibility is public,
and for classes default visibility is private. All other rules are same

This function inside a class is called a non-static member function.

If the `member functions` definition is included inside the declaration of the class, that function is by default made implicitly `inline`.

#### static member function

Member functions or variables declared static are shared between all instances of an object type. Meaning that only one copy of the member function or variable does exists for any object type.

### initialization

#### constructor types

Constructors have no names and cannot be called directly. They are invoked when initialization takes place, and they are selected according to the rules of initialization. 
1. The constructors without explicit specifier are converting constructors. 
2. The constructors with a constexpr specifier make their type a LiteralType. 
3. Constructors that may be called without any argument are default constructors.
4. Constructors that take another object of the same type as the argument are copy constructors and move constructors.




#### new & pointer (heap allocation)

```cpp
class Circle{
    double radius;
  public:
    Circle(double r) { radius = r;}
    double circum() { return 2 * radius * 3.14; }
}
int main(){
    // new always returns pointer to heap allocated object
    Circle* cp = new Circle(2);
}
```

### Constructor (direct) initialization

Constructors never return values, they simply initialize the object.
Also need to have same name as class.
```cpp
class Rectangle {
    int width, height;
  public:
    Rectangle (int,int);
    int area () {return (width*height);}
};

Rectangle::Rectangle (int a, int b) {
  width = a;
  height = b;
}

int main () {
    // notice no new keyword
  // normal constructor does not new, alos allocated on stack
  Rectangle rect (3,4);
  // also known as functional form or direct initialization
  Rectangle rectb (5,6);
  cout << "rect area: " << rect.area() << endl;
  cout << "rectb area: " << rectb.area() << endl;
  return 0;
}
```

Like any other function, a constructor can also be overloaded with different versions taking different parameters: with a different number of parameters and/or parameters of different types. The compiler will automatically call the one whose parameters match the arguments.

### default constructors call memberwise initializers

**note** - default constructor with no args
will conflict with a constructor with all args that have default values
```cpp
class Cylinder {
    private:
    int k {4};// memberwise initializer
    std::string j {"jay"};// memberwise initializers

    public:
    Cylinder() = default; // no params needed

    std::string getDescription(){
        return j;
    }
};
int main()
{
    Cylinder cc;
    std::cout << cc.getDescription() <<std::endl;
    return 0;
}
```




### direct initailizer list initialization

NOrmally other constructor initialization 
happens via doing memberwise initialization by assignment, but this approach avoids that.
```cpp
Cylinder::Cylinder(double radius_param, double height_param): base_radius(radius_param), height(height_param){
// nothing needed here
}
//usage
Cylinder c(1,2);
Cylinder c{1,6};
```

This is also valid:
```cpp
Cylinder::Cylinder(double radius_param, double height_param): base_radius{radius_param}, height{height_param}{
// nothing needed here
Cylinder c(1,2);
Cylinder c{1,6};
}
```

helps avoid copies in comparision to assignment initialization

Prefere initializer lists over memberwise
copy initialization(by assignment)


### Memberwise initialization by assigment does double copies

e.g.
```cpp
Cylinder::Cylinder(double rad_param, double height_param){
  // here init has already happened with default memberwise values populated in "this"
  // following assignments are copies of values that update "this" object
  base_radius = rad_param;
  height = height_param;
}
```

### Uniform initialization vs direct initialization

#### copy constructor on assignment
Assignment calls single argument constructor,
sometimes also referred to as copy constructor
```cpp
class_name object_name = initialization_value;
// this calls copy constructor with initialization_value as single argument
```
From c++11, 
C++ introduced the possibility of constructors to be called using uniform initialization, which essentially is the same as the functional form, but using braces `({})` instead of parentheses `(())`:
```cpp
class_name object_name { value, value, value, ... }
```

```cpp
// classes and uniform initialization
#include <iostream>
using namespace std;

class Circle {
    double radius;
  public:
    Circle(double r) { radius = r; }
    double circum() {return 2*radius*3.14159265;}
};

int main () {
  Circle foo (10.0);   // functional form or direct initialization
  Circle bar = 20.0;   // assignment init.
  Circle baz {30.0};   // uniform init or brace initialization
  Circle qux = {40.0}; // POD-like

  cout << "foo's circumference: " << foo.circum() << '\n';
  return 0;
}
```

#### Member initialization in constructors

The body of a function definition of any constructor, before the opening brace of the compound statement, may include the `member initializer list`, whose syntax is the colon character `:`, followed by the comma-separated list of one or more member-initializers.

When a constructor is used to initialize other members, these other members can be initialized directly, without resorting to statements in its body. This is done by inserting, before the constructor's body, a colon (:) and a list of initializations for class members.

e.g.
```cpp
class Circle {
    double radius;
  public:
    Circle(double r) : radius(r) { }
    double area() {return radius*radius*3.14159265;}
};

class Cylinder {
    Circle base;
    double height;
  public:
    Cylinder(double r, double h) : base (r), height(h) {}
    double volume() {return base.area() * height;}
};

// with initializer syntax
Cylinder::Cylinder (double r, double h) : base{r}, height{h} { }
```

If a non-static data member has a default member initializer and also appears in a member initializer list, then the member initializer is used and the default member initializer is ignored:
```cpp
struct S {
    int n = 42;   // default member initializer
    S() : n(7) {} // will set n to 7, not 42
};
```

### compiler can do implicit conversions for function calls

opt out of it by following:
```cpp
explicit Square(double side_param)
```
Unlike explicit constructors, which are only considered during direct initialization (which includes explicit conversions such as static_cast), converting constructors are also considered during copy initialization
```cpp
struct A
{
    A() { }         // converting constructor (since C++11)  
    A(int) { }      // converting constructor
    A(int, int) { } // converting constructor (since C++11)
};
 
struct B
{
    explicit B() { }
    explicit B(int) { }
    explicit B(int, int) { }
};
 
int main()
{
    A a1 = 1;      // OK: copy-initialization selects A::A(int)
    A a2(2);       // OK: direct-initialization selects A::A(int)
    A a3{4, 5};    // OK: direct-list-initialization selects A::A(int, int)
    A a4 = {4, 5}; // OK: copy-list-initialization selects A::A(int, int)
    A a5 = (A)1;   // OK: explicit cast performs static_cast, direct-initialization
 
//  B b1 = 1;      // error: copy-initialization does not consider B::B(int)
    B b2(2);       // OK: direct-initialization selects B::B(int)
    B b3{4, 5};    // OK: direct-list-initialization selects B::B(int, int)
//  B b4 = {4, 5}; // error: copy-list-initialization selected an explicit constructor
                   //        B::B(int, int)
    B b5 = (B)1;   // OK: explicit cast performs static_cast, direct-initialization
    B b6;          // OK, default-initialization
    B b7{};        // OK, direct-list-initialization
//  B b8 = {};     // error: copy-list-initialization selected an explicit constructor
                   //        B::B()
}
```

### Friend classes

Only class implementer can declare who its friends are.
A function or class cannot declare itself as a friend of any class.

In class definition use the `friend` keyword and name of a non-member function or other class to grant it
access to the private and protected members of your class.

Friendship is not transitive (a friend of your friend is not your friend).
Friendship is not inherited (your friend's children are not your friends).



```cpp
class M {
  friend class F;
}
```

### Member functions defined within classes are implicitly declared inline

A function defined within a class definition is an inline function.

An inline function or variable shall be defined in every translation unit in which it is odr-used and shall have exactly the same definition in every case.

A member function may be defined  in its class definition, in which case it is an inline member function.

This means they do not break `one-definition-rule`.
and itss ok when they get copied in case of header files containing implementations
of classes along with implementations of method definitions.

