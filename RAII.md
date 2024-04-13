
## Exceptions

RAII provides strong exception safety guarantees. When an exception is thrown, the stack is unwound, and destructors of objects in the scope are called automatically. 

This ensures that resources are released properly, even in the presence of exceptions. Therefore, using RAII significantly simplifies error handling and resource cleanup, reducing the risk of resource leaks and ensuring that the program remains in a consistent state

## Use

RAII (Resource Acquisition Is Initialization) is a powerful and widely used idiom in C++ for managing resources. To make the most out of RAII, consider the following best practices:

1. **Use Smart Pointers**: Prefer using smart pointers like `std::unique_ptr` and `std::shared_ptr` over raw pointers for managing dynamically allocated resources. Smart pointers automatically clean up the allocated memory when they go out of scope, eliminating the need for manual memory management and reducing the risk of memory leaks.

2. **Avoid Naked `new` and `delete`**: Avoid using `new` and `delete` directly whenever possible. Instead, use smart pointers or container classes like `std::vector` and `std::string` that manage memory automatically. If you must use raw pointers, ensure proper ownership semantics and use RAII wrappers such as `std::unique_ptr` or `std::shared_ptr` to manage their lifetime.

3. **Implement the Rule of Three/Five**: If your class manages resources, adhere to the Rule of Three (or Rule of Five in modern C++) by providing appropriate constructors, copy constructors, copy assignment operators, move constructors, and move assignment operators. This ensures proper resource management and prevents issues like resource leaks and double deletions.

4. **Define Custom Resource Managers**: For custom resources such as file handles, network connections, or locks, encapsulate the resource management logic within dedicated RAII classes. Implement constructors to acquire resources and destructors to release them, ensuring that resources are properly managed throughout their lifetime.

5. **Use Scope Guards**: In situations where RAII wrappers are not suitable, consider using scope guards like `std::lock_guard`, `std::unique_lock`, or `std::scoped_lock` for managing locks and other resources. Scope guards acquire resources upon construction and release them upon destruction, ensuring proper resource management even in the presence of exceptions.

6. **Follow Single Responsibility Principle (SRP)**: Ensure that RAII classes adhere to the Single Responsibility Principle by encapsulating a single responsibility or resource. This improves code maintainability and makes it easier to reason about resource management.

7. **Document Ownership Semantics**: Clearly document the ownership semantics of your classes and functions to make it clear who is responsible for managing resources. Follow consistent naming conventions for methods and parameters related to resource ownership (e.g., `create`, `release`, `acquire`, `dispose`, `owner`, `shared_ptr` vs. `unique_ptr`, etc.).

8. **Test Resource Management**: Write unit tests to verify the correctness of your resource management code, including proper acquisition, release, and error handling. Test edge cases, error conditions, and exception safety to ensure robustness.

By following these best practices, you can effectively leverage RAII to manage resources safely and efficiently in your C++ codebase, leading to fewer resource leaks, improved code reliability, and easier maintenance.