Okay, we covered this just before. To reiterate the best practices for special member functions in a **polymorphic base class** (one designed for inheritance with `virtual` functions):

1.  **Declare a `virtual` Destructor:**
    * **Why:** This is **absolutely essential**. It ensures that when you `delete` a derived object through a base class pointer, the correct derived class destructor runs first, followed by the base class destructor. Without `virtual`, only the base destructor runs, leading to resource leaks and undefined behavior.
    * **How:** `virtual ~BaseClassName() = default;` (or with an implementation `{}` if needed).

2.  **Delete Copy Operations (Copy Constructor, Copy Assignment Operator):**
    * **Why:** Copying polymorphic objects by value causes **object slicing** (the derived part is lost). Deleting these operations prevents this common error and makes it clear the class should be handled via pointers or references.
    * **How:**
        ```c++
        BaseClassName(const BaseClassName&) = delete;
        BaseClassName& operator=(const BaseClassName&) = delete;
        ```

3.  **Delete Move Operations (Move Constructor, Move Assignment Operator):**
    * **Why:** Similar to copying, moving a derived object *as* a base object can lead to slicing or inconsistent states. Deleting them is often the safest and simplest approach for polymorphic base classes.
    * **How:**
        ```c++
        BaseClassName(BaseClassName&&) = delete;
        BaseClassName& operator=(BaseClassName&&) = delete;
        ```

**In Summary (The Rule of 5 for Polymorphic Bases):**

For a class designed as a polymorphic base:

* **MUST** have a `virtual` destructor.
* **SHOULD** typically delete the copy constructor, copy assignment, move constructor, and move assignment operators to prevent slicing and enforce polymorphic usage via pointers/references.

**Example (Same as before, illustrating the points):**

```c++
#include <iostream>
#include <string>
#include <memory> // For std::unique_ptr

// Polymorphic Base Class
class Shape {
public:
    // 1. Virtual Destructor (Essential!)
    virtual ~Shape() {
        std::cout << "Shape destructor called.\n";
    }

    // 2. Delete Copy Operations (Prevents Slicing)
    Shape(const Shape&) = delete;
    Shape& operator=(const Shape&) = delete;

    // 3. Delete Move Operations (Prevents Slicing / State Issues)
    Shape(Shape&&) = delete;
    Shape& operator=(Shape&&) = delete;

    // Default constructor
    Shape() = default;

    // Example virtual function
    virtual void draw() const = 0; // Pure virtual makes Shape abstract

protected:
    std::string color = "transparent";
};

// Derived Class
class Circle : public Shape {
public:
    Circle(double r, const std::string& c) : radius(r) {
        color = c;
        std::cout << "Circle constructed.\n";
    }

    ~Circle() override { // Implicitly virtual
        std::cout << "Circle destructor called.\n";
    }

    void draw() const override {
        std::cout << "Drawing a " << color << " circle with radius " << radius << ".\n";
    }

private:
    double radius;
};


int main() {
    std::unique_ptr<Shape> shape_ptr = std::make_unique<Circle>(5.0, "Red");
    shape_ptr->draw();

    // shape_ptr goes out of scope here.
    // ~Circle() is called, then ~Shape() because the base destructor is virtual.
    std::cout << "Exiting main...\n";
    return 0;
}
```

This setup ensures correct cleanup and prevents accidental slicing when working with `Shape` objects polymorphically.