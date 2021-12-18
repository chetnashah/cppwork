
### Enums

An enumeration is a distinct type whose value is restricted to a range of values (see below for details), 
which may include several explicitly named constants ("enumerators"). 
The values of the constants are values of an integral type known as the underlying type of the enumeration

Three types of declarations:
1. enum (unscoped enum) (Legacy)
2. enum class (scoped enum)
3. enum struct (scoped enum)

Enums can't be printed directly, need to be cast to an underlying type first - One of the design motivations behind enum classes is to prevent confusing implicit conversions to integral types. And this is one of the side effects of that design choice. Enum classes can't implicitly convert to something directly printable through std::cout

### Changing the underlying type

By default the underlying type is `int`.
can be changed to for e.g. to `unsigned char`

```cpp
enum class Directions : unsigned char {
    left, right, top, bottom
}
```

### Enum classes don't implicitly convert from/to anything, including other enum classes

```cpp
enum class Gender{
    Male, Female
};
enum class DayOfWeek{
    Monday = 1,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};
void print_day(DayOfWeek day){
    // does something
}
int main(){ 
    print_day(3);// Compiler Error! No implicit conversion allowed from int to DayOfWeek
    print_day(Gender::Female);// compiler error, no implicit conversion allowed from Gender::Female enum
    return 0;
}
```

### Bad practice - initializing with underlying type (Defeats the purpose of enum)

```cpp
enum class DayOfWeek{
    Monday = 1,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

DayOfWeek day{9}; // Correct, but bad
```


### enum class (scoped enum)

Declaring an enum class

```cpp
// left = 0, right = 1, top = 2, bottom = 3
enum class Directions {
    left, right, top, bottom
};


int main(int argc, const char * argv[]) {
    Directions startDirection  {Directions::top};
    // direction cannot directly be printed, must be cast
    std::cout << "start direction is " << static_cast<int>(startDirection) << std::endl; // 2
    return 0;
}
```

#### we can have multiple enumerators for the same value

```cpp
enum class Month {
    jan = 1, January = 1, feb, march, april, may, june, july, august, september, october, november,
    dec = 12, December = 12
};
int main(int argc, const char * argv[]) {
    Month firstMonth = Month::jan;    
    std::cout << (firstMonth == Month::January) << std::endl;// 1
    return 0;
}
```

#### Even better way is to declare in terms of other enums
e.g.
```cpp
enum class Directions {
   left = 0, L = left, right, R = right, top, T = top, bottom, B = bottom 
}
int main(int argc, const char * argv[]) {
    Directions startDirection { Directions::R }; // 1
    return 0;
}
```


### legacy (unscoped) enums

```cpp
enum Tools {
    pen, marker, eraser
}
```

They implicitly transform to int, e.g. in std::cout, but this implicit conversion, opens a can of problems and bugs that we would like to avoid.

### Type alias

#### type alias via `using` keyword

Dont forget the `=` sign

```cpp
using hugeint = unsigned long long int;

hugeint h{111};
```

#### type alias via `typedef`

alias name comes last, no equal sign

```cpp
typedef unsigned long long int hugeint;
```