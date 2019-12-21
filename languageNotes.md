

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

