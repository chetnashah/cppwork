

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

