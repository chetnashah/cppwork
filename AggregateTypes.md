https://en.cppreference.com/w/cpp/language/aggregate_initialization

##

### Aggregate Types in C++

In C++, an **aggregate** is a type that is an array or a class (struct, class, or union) with no user-declared constructors, no private or protected non-static data members, no base classes, and no virtual functions. Essentially, aggregates are simple data structures that can be initialized directly with brace-enclosed initializer lists, providing a straightforward, data-centric form of initialization.

### Characteristics of Aggregate Types

An aggregate can have:
- Public data members.
- Member arrays.
- Nested aggregates.
- Methods
- Inherited members if they come from public base aggregates (since C++17).

Aggregates cannot have:
- User-declared constructors.
- Private or protected non-static data members (before C++17).
- Virtual functions.
- Base classes (before C++17).

### Examples of Aggregate Types

Let's go through some examples to see how aggregates can be defined and used in C++.

#### Example 1: Basic Struct

Here's a simple struct with only public data members:

```cpp
struct Point {
    int x;
    int y;
};

int main() {
    Point p = {3, 4};  // Aggregate initialization
    return 0;
}
```

In this example, `Point` is an aggregate type because it only contains public data members and does not have any constructors or other disqualifying features. It can be initialized directly using a brace-enclosed list.

#### Example 2: Nested Aggregates

Aggregates can also contain other aggregates:

```cpp
struct Point {
    int x;
    int y;
};

struct Rectangle {
    Point topLeft;
    Point bottomRight;
};

int main() {
    Rectangle rect = {{0, 1}, {5, 6}};  // Nested aggregate initialization
    return 0;
}
```

Here, `Rectangle` contains two `Point` structures. Both `Rectangle` and `Point` are aggregates, and we can initialize `Rectangle` using a nested brace-enclosed list.

#### Example 3: Array as an Aggregate

Arrays are inherently aggregates in C++:

```cpp
int main() {
    int numbers[5] = {1, 2, 3, 4, 5};  // Array aggregate initialization
    return 0;
}
```

This example initializes an array of integers, which is an aggregate.

#### Example 4: C++17 Aggregate with Inheritance

From C++17, aggregates can have base classes if the base classes themselves are aggregates:

```cpp
struct Base {
    int b;
};

struct Derived : Base {
    int d;
};

int main() {
    Derived obj = {1, 2};  // Initializes Base::b to 1 and Derived::d to 2
    return 0;
}
```

In this example, `Derived` inherits from `Base`, and both are aggregates. Thus, `Derived` can be initialized in a way that includes values for both its own and its base's members.

### Conclusion

Aggregate types in C++ provide a way to initialize data structures in a simple and straightforward manner without the complexities of constructors and other class-specific features. They are especially useful when you need to work with plain data structures that encapsulate multiple types of data or arrays but do not require the overhead of encapsulation and data hiding typical of more complex class designs.