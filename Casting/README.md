
Casting usually comes into play when you as a programmer know better than the type system.

1. `dynamic_cast` (runtime checking) - dynamic_cast can be **used only with pointers and references to objects.** Its purpose is to ensure that the result of the type conversion is a valid complete object of the requested class. **Requires the use of polymorphic types, up and down the inheritance chain** and is checked at runtime. Returns `false` if the conversion is not possible.
2. `static_cast` (compile time) - static_cast can perform conversions between pointers to related classes, not only from the derived class to its base, but also from a base class to its derived. Use a combination of user defined and implicit conversions.
3. `const_cast` - This type of casting manipulates the constness of an object, either to be set or to be removed.
4. `reinterpret_cast` - reinterpret_cast converts **any pointer type to any other pointer type**, even of unrelated classes, considered unsafe and should be used with care.
