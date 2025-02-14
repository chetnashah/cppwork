
## Header files declare interface, Consumers import the header.

## Implmentations (.cpp) should import their own header files (.h) first

The implementation file needs to include its own header for **three crucial technical reasons**:

1. **Class Identity Verification**  
   When implementing `ClassName::method()`, the compiler must verify:
   - The class actually exists in the current scope
   - The method is properly scoped to the class
   - The method signature *exactly matches* the declaration

2. **Memory Layout Awareness**  
   The compiler needs the full class definition from the header to:
   - Calculate object sizes (`sizeof(ClassName)`)
   - Determine member variable offsets
   - Handle inheritance hierarchies correctly

3. **Type Safety Enforcement**  
   Header inclusion ensures:
   - Parameter/return types match exactly
   - `const` qualifiers are preserved
   - Exception specifications align (when used)
   - Template parameters are consistent

**Example of what breaks without it:**  
```cpp
// Logger.h
class Logger {
public:
    void log(const std::string& message);  // Declaration
};

// Logger.cpp
void log(const std::string& message) {  // Oops! Forgot Logger::
    std::cout << message << '\n';       // Compiler thinks this is free function
}
```

**Modern Compiler Perspective**  
While `#include` seems simple, it actually creates critical compiler checkpoints:
1. Header inclusion triggers name mangling for linker
2. Generates vtable layouts for polymorphic classes
3. Enforces One Definition Rule (ODR) boundaries
4. Provides context for template instantiation tracking

