
## Const with types

value of the variable cannot be changed after initialization.

## Const with pointers (one level of indirection)

- `const int* ptr` or `int const* ptr` - Pointer to constant integer. The integer value cannot be changed through the pointer, but the pointer itself can be changed.

- `int* const ptr` - Constant pointer to integer. The pointer itself cannot be changed, but the integer value can be changed through the pointer.

- `const int* const ptr` - Constant pointer to constant integer. Both the pointer and the integer value are constant.

## const to protect parameters

We can do automatic casting in functions to avoid changing the value of the parameter.

```cpp
void mysteryFunction(const string* someString)
{
    *someString = "Test";  // Will not compile
}
 
int main()
{
    string myString { "The string" };
    mysteryFunction(&myString);  // &myString is a string*
}
```


## const member functions

**mark class member functions as const, preventing them from modifying data members of the class**, if class data member is `mutable`, const member function can modify it.

```cpp
export class AirlineTicket
{
    public:
        double calculatePriceInDollars() const;
 
        std::string getPassengerName() const;
        void setPassengerName(std::string name);
 
        int getNumberOfMiles() const;
        void setNumberOfMiles(int miles);
 
        bool hasEliteSuperRewardsStatus() const;
        void setHasEliteSuperRewardsStatus(bool status);
    private:
        std::string m_passengerName { "Unknown Passenger" };
        int m_numberOfMiles { 0 };
        bool m_hasEliteSuperRewardsStatus { false };
};
 
std::string AirlineTicket::getPassengerName() const
{
    return m_passengerName;
}
// Other member functions omitted…
```

## References to const

once a reference is initialized, it cant point to anything else.
But its value can be changed, if it is not a reference to const.

By having a reference-to-const, you get the best of both worlds: no copy is made, and the original variable cannot be changed.
**Note** `referencet-to-const` does not affect constness of original variable which is being referred.

```cpp
int value = 5;
int& ref2 = value;
const int& ref = value; // ref is a reference to const int
value = 6; // OK
// ref = 7; // Error, cant use a reference to const to change the value of the original variable
ref2 = 8; // OK
```

## const and `auto`

**auto** always strips away **reference** and **const** qualifiers and thus results in a copy!

If you don't want a copy, use `auto&` or `const auto&`.

### Use `decltype` instead of auto to preserve constness

```cpp
const int x = 5;
auto y = x; // y is an int, not a const int
decltype(x) z = x; // z is a const int
```



## Guideline: Avoid const member variables in class

The C++ Core Guidelines recommend avoiding `const` member variables for several reasons. Here are some key points:

1. **Immutability and Initialization**:
   - `const` member variables must be initialized at the point of declaration or in the constructor initializer list. This can complicate the construction process, especially if the initialization logic is complex or if it depends on runtime information.
   - Once set, a `const` member cannot be modified, which can limit the flexibility of the class. This immutability can sometimes make it difficult to implement certain algorithms or behaviors that require modification of all member variables.

2. **Assignment Operator and Move Semantics**:
   - `const` member variables can interfere with the automatically generated assignment operator. Since `const` members cannot be assigned after initialization, the compiler-generated assignment operator will not work if the class contains `const` members.
   - Similarly, move semantics (move constructors and move assignment operators) can be problematic. Moving typically involves transferring resources from one object to another, which often necessitates modifying the member variables.

3. **Consistency and State Management**:
   - `const` members can lead to inconsistencies in the internal state management of a class. Since they cannot be changed after initialization, developers might need to introduce additional mutable state or external management logic to handle scenarios where the `const` member needs to be logically modified.
   - Maintaining `const` correctness throughout a complex class hierarchy can be challenging and might introduce subtle bugs or design complications.

4. **Design Considerations**:
   - From a design perspective, it is often better to use other mechanisms to enforce immutability or const-correctness. For example, providing `const` member functions (which guarantee that the function does not modify the object) can be a more flexible and maintainable approach.
   - Using `const` member functions allows the interface to express immutability without imposing the constraints on the internal state management of the class.

### Example

Here’s an illustrative example to show some of the complications:

```cpp
class Example {
    const int value; // const member

public:
    Example(int v) : value(v) {} // Must be initialized in the constructor

    // Copy assignment operator would not be generated by the compiler
    Example& operator=(const Example& other) = delete;
};
```

In the example above, the `const` member `value` must be initialized in the constructor initializer list. Additionally, the compiler will not generate a copy assignment operator due to the presence of the `const` member, potentially complicating object management.

### Alternatives

Instead of `const` member variables, consider:

- Using `const` member functions to prevent modification of the object state.
- Using `private` member variables with controlled access through `getter` functions that return `const` references or values.
- Providing a clear and consistent interface that ensures immutability where necessary without imposing strict internal constraints.

### Conclusion

While `const` member variables can be useful in certain scenarios, their use can introduce complexities and limitations in class design and object management. The C++ Core Guidelines recommend avoiding them to promote more flexible, maintainable, and robust code.