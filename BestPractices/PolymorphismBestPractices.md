GIVEN `Google Search`

* This search fetches information on best practices related to C++ inheritance and polymorphism, focusing on specific keywords like virtual destructors, `override`, `final`, NVI, and protected constructors in abstract classes. This aligns directly with the user's refined request for practices specifically within this domain.

Okay, let's focus specifically on the best practices for C++ inheritance and polymorphism, with clear explanations and examples for each.

Here are the key practices:

1.  **Declare Base Class Destructors `virtual`**
    * **Concept:** If a class is intended to be a base class in a polymorphic hierarchy (i.e., has virtual functions or is expected to be deleted via a base pointer), its destructor **must** be `virtual`.
    * **Why:** This ensures that when you `delete` a derived object using a base class pointer, the correct chain of destructors (Derived first, then Base) is called. If the base destructor isn't virtual, deleting via a base pointer leads to undefined behavior (usually only the base destructor runs, leaking resources held by the derived part).
    * **Example:**
        ```c++
        #include <iostream>
        #include <memory>

        class Base {
        public:
            // BEST PRACTICE: Virtual destructor for polymorphic base class
            virtual ~Base() { std::cout << "Base Destructor\n"; }
            virtual void identify() const { std::cout << "I am Base\n"; }
        };

        class Derived : public Base {
            std::string* data; // Example resource managed by Derived
        public:
            Derived() : data(new std::string("Derived data")) { std::cout << "Derived Constructor\n"; }
            // Destructor (implicitly virtual because Base's is)
            ~Derived() override {
                std::cout << "Derived Destructor (deleting data)\n";
                delete data;
            }
            void identify() const override { std::cout << "I am Derived\n"; }
        };

        int main() {
            std::unique_ptr<Base> ptr = std::make_unique<Derived>();
            ptr->identify(); // Calls Derived::identify()
            // ptr goes out of scope, delete is called via Base*
            // CORRECT order: ~Derived(), then ~Base()
            std::cout << "Exiting main...\n";
        }
        ```
    * **Output:**
        ```
        Derived Constructor
        I am Derived
        Exiting main...
        Derived Destructor (deleting data)
        Base Destructor
        ```

2.  **Delete or Protect Copy/Move Operations in Base Classes to Prevent Slicing**
    * **Concept:** Copying or moving polymorphic objects by value slices off the derived part. To prevent this common error, explicitly delete the copy constructor, copy assignment, move constructor, and move assignment operators in the polymorphic base class.
    * **Why:** This forces users to work with the objects polymorphically via pointers or references, which is the correct way and avoids slicing.
    * **Example:**
        ```c++
        class Shape {
        public:
            virtual ~Shape() = default; // Rule 1: Virtual destructor

            // BEST PRACTICE: Delete copy/move to prevent slicing
            Shape(const Shape&) = delete;
            Shape& operator=(const Shape&) = delete;
            Shape(Shape&&) = delete;
            Shape& operator=(Shape&&) = delete;

            virtual void draw() const = 0; // Abstract class

        protected:
             Shape() = default; // Allow derived construction
        };

        class Circle : public Shape {
             // ... implementation ...
        public:
             Circle() = default;
             void draw() const override { /* draw circle */ }
        };

        int main() {
            Circle c;
            // Shape s = c; // COMPILE ERROR: Copy constructor is deleted
            // Shape s2;
            // s2 = c;    // COMPILE ERROR: Copy assignment is deleted
        }
        ```

3.  **Use `override` Explicitly When Overriding Virtual Functions**
    * **Concept:** When a derived class function is intended to override a base class `virtual` function, mark it with the `override` keyword.
    * **Why:** The compiler verifies that a matching `virtual` function exists in a base class. This catches typos in function names or signatures that would otherwise silently create a *new* unrelated function instead of overriding. It improves code clarity and safety.
    * **Example:**
        ```c++
        #include <iostream>

        class Base {
        public:
            virtual ~Base() = default;
            virtual void func(int x) { std::cout << "Base::func(int)\n"; }
        };

        class Derived : public Base {
        public:
            // BEST PRACTICE: Use override
            void func(int x) override { std::cout << "Derived::func(int)\n"; }

            // void fucn(int x) override; // COMPILE ERROR: 'fucn' does not override (typo)
            // void func(double x) override; // COMPILE ERROR: Signature mismatch
        };
        ```

4.  **Use `final` to Prevent Further Overriding or Inheritance**
    * **Concept:** Mark a `virtual` function as `final` in a derived class to prevent it from being overridden in further subclasses. Mark an entire class `final` to prevent it from being used as a base class.
    * **Why:** Signals design intent ("this behavior/class is complete") and can allow the compiler to perform optimizations (like devirtualizing calls) because it knows the function/class won't change further down.
    * **Example:**
        ```c++
        class Base {
        public:
            virtual void mandatoryStep() {}
            virtual void customizableStep() {}
            virtual ~Base() = default;
        };

        class Derived : public Base {
        public:
            // BEST PRACTICE: Prevent further override of mandatoryStep
            void mandatoryStep() final override { /* specific implementation */ }
            void customizableStep() override { /* allow further customization */ }
        };

        class MoreDerived : public Derived {
        public:
            // void mandatoryStep() override; // COMPILE ERROR: Base::mandatoryStep is final
            void customizableStep() override { /* ok */ }
        };

        // BEST PRACTICE: Prevent any further inheritance
        class Uninheritable final : public Base {
            // ...
        };

        // class Problem : public Uninheritable {}; // COMPILE ERROR: Uninheritable is final
        ```

5.  **Use Pure Virtual Functions (`= 0`) for Abstract Classes/Interfaces**
    * **Concept:** Declare a virtual function with `= 0` to make it *pure virtual*. A class with one or more pure virtual functions becomes an *abstract class* that cannot be instantiated directly. Derived classes *must* override all pure virtual functions to become concrete (instantiable).
    * **Why:** This is the standard C++ way to define an interface – a contract specifying what derived classes must provide without necessarily providing a base implementation.
    * **Example:**
        ```c++
        #include <string>

        class Serializable { // Interface class
        public:
            // BEST PRACTICE: Pure virtual functions define an interface
            virtual std::string serialize() const = 0;
            virtual void deserialize(const std::string& data) = 0;

            // BEST PRACTICE: Virtual destructor (even if interface has no data)
            virtual ~Serializable() = default;

        protected:
             // BEST PRACTICE: Protected constructor for abstract class
             Serializable() = default;
             // Prevent slicing
             Serializable(const Serializable&) = delete;
             Serializable& operator=(const Serializable&) = delete;
        };

        class UserData : public Serializable {
            std::string name;
            int age;
        public:
            // Must implement the interface
            std::string serialize() const override { /* ... */ return ""; }
            void deserialize(const std::string& data) override { /* ... */ }
        };

        int main() {
            // Serializable s; // COMPILE ERROR: Cannot instantiate abstract class
            UserData u; // OK: UserData provides implementations
            Serializable* ptr = &u; // OK: Can use base pointer to derived object
        }
        ```

6.  **Use `protected` Constructors for Abstract Base Classes**
    * **Concept:** As shown above, if a base class is abstract (due to pure virtual functions), make its constructors `protected`.
    * **Why:** Prevents direct instantiation of the abstract base class (which isn't allowed anyway) while still allowing derived classes to call the base constructor during their own initialization.

7.  **Consider the Non-Virtual Interface (NVI) Idiom**
    * **Concept:** Make public functions non-virtual. These functions provide the stable public interface and call private or protected virtual "hook" functions for the customizable parts.
    * **Why:** Allows the base class to enforce invariants, add logging, locking, or common setup/cleanup code around the customizable derived class behavior. Separates interface from implementation detail.
    * **Example:**
        ```c++
        #include <iostream>

        class Task {
        public:
            // BEST PRACTICE: Public Non-Virtual Interface
            void execute() {
                std::cout << "Base: Preparing task...\n";
                if(preExecuteCheck()){
                    doExecute(); // Call the customizable virtual part
                }
                std::cout << "Base: Finalizing task...\n";
            }
            virtual ~Task() = default;

        protected:
            // Protected virtual function for customization
            virtual void doExecute() = 0;
            virtual bool preExecuteCheck() { return true; } // Optional hook
        };

        class MyTask : public Task {
        protected:
            void doExecute() override {
                std::cout << "  MyTask: Performing specific action...\n";
            }
        };

        int main() {
            MyTask t;
            t.execute(); // Calls public NVI method
        }
        ```

8.  **Use Smart Pointers to Manage Polymorphic Objects**
    * **Concept:** When dynamically allocating objects that will be used polymorphically, use `std::unique_ptr<Base>` or `std::shared_ptr<Base>`.
    * **Why:** Ensures proper memory management (automatic deletion when the pointer goes out of scope) and correctly calls the virtual destructor thanks to the smart pointer's customized deleter. Avoids manual `new`/`delete` and potential memory leaks.
    * **Example:** (See examples 1 and 5 above using `std::unique_ptr`)

9.  **Prefer Composition Over Inheritance (Where Appropriate)**
    * **Concept:** Before using inheritance, consider if composition (having a member variable of another class type) would be a better fit. Inheritance models "is-a" relationships suitable for polymorphism. Composition models "has-a" or "uses-a" relationships.
    * **Why:** Inheritance creates a strong coupling between base and derived classes. Overuse can lead to rigid designs. Composition is often more flexible. Use public inheritance primarily when you intend to substitute derived objects where base objects are expected (Liskov Substitution Principle) and need polymorphic behavior.

These practices help create robust, maintainable, and less error-prone object-oriented C++ code that effectively utilizes inheritance and polymorphism.
Okay, let's focus specifically on the best practices for C++ **inheritance and polymorphism**. Here's a clear list with explanations and examples using the `Shape` theme:

**1. Use a `virtual` Destructor in the Base Class**

* **Concept:** If a class is designed to be a base class for polymorphism (i.e., it has any virtual functions, or you intend to delete derived objects via base pointers), its destructor **must** be `virtual`.
* **Why:** Ensures that when you `delete` a derived object using a base class pointer, the derived class's destructor runs *first* to clean up derived-specific resources, followed by the base class's destructor. Without `virtual`, only the base destructor runs, leading to resource leaks and undefined behavior.
* **Example:**
    ```c++
    #include <iostream>
    #include <memory>

    class Shape {
    public:
        // ESSENTIAL: Virtual destructor for proper cleanup via base pointer
        virtual ~Shape() { std::cout << "Shape destroyed\n"; }
        virtual void draw() const = 0; // Makes Shape abstract
    };

    class Circle : public Shape {
        double radius;
    public:
        Circle(double r) : radius(r) {}
        // Destructor automatically called before base destructor because base is virtual
        ~Circle() override { std::cout << "Circle destroyed (r=" << radius << ")\n"; }
        void draw() const override { /* ... */ }
    };

    int main() {
        Shape* shapePtr = new Circle(5.0);
        delete shapePtr; // CORRECTLY calls ~Circle() then ~Shape()
        return 0;
    }
    // Output:
    // Circle destroyed (r=5.0)
    // Shape destroyed
    ```

**2. Prevent Slicing: Delete Base Class Copy/Move Operations**

* **Concept:** Object slicing occurs when a derived object is copied or assigned *by value* to a base class object, losing derived data and behavior. Since polymorphism relies on pointers/references, you usually want to prevent value-based copying/moving of polymorphic objects.
* **Why:** Avoids data loss and ensures objects maintain their true polymorphic type. Makes the intended usage (via pointers/references) clear.
* **How:** Delete the copy constructor, copy assignment, move constructor, and move assignment operators in the base class.
* **Example:**
    ```c++
    class Shape {
    public:
        virtual ~Shape() = default;

        // Prevent slicing by deleting copy operations
        Shape(const Shape&) = delete;
        Shape& operator=(const Shape&) = delete;

        // Usually best to delete move operations too for polymorphic bases
        Shape(Shape&&) = delete;
        Shape& operator=(Shape&&) = delete;

        virtual void draw() const = 0;
    };

    class Circle : public Shape { /* ... */ };

    int main() {
        Circle c1(1.0);
        // Shape s1 = c1; // COMPILE ERROR: Copy constructor is deleted
        // Shape s2;
        // s2 = c1;      // COMPILE ERROR: Copy assignment is deleted
        return 0;
    }
    ```

**3. Use `override` for Derived Virtual Functions**

* **Concept:** When a derived class intends to override a base class `virtual` function, explicitly mark it with the `override` keyword.
* **Why:** The compiler verifies that a virtual function with the exact same signature exists in a base class. This catches typos or signature mismatches at compile time that would otherwise silently create a new, non-overriding function.
* **Example:**
    ```c++
    class Shape {
    public:
        virtual ~Shape() = default;
        virtual void draw() const { /* base draw */ }
        virtual void rotate(int degrees) { /* base rotate */ }
    };

    class Circle : public Shape {
    public:
        // Correctly overrides base virtual function
        void draw() const override { /* circle draw */ }

        // void rotate(float rads) override; // COMPILE ERROR: Signature mismatch (float vs int)
                                          // 'override' catches this!
        void rotate(int degrees) override { /* circle rotate */ } // OK
    };
    ```

**4. Use `final` to Control Inheritance/Overriding**

* **Concept:**
    * Mark a `virtual` function `final` in a derived class to prevent classes inheriting from *it* from overriding that function further.
    * Mark an entire `class` `final` to prevent any other class from inheriting from it.
* **Why:** Clearly expresses design intent ("this behavior/class is complete") and can sometimes allow compiler optimizations (devirtualization).
* **Example:**
    ```c++
    class Shape {
    public:
        virtual ~Shape() = default;
        virtual void draw() const = 0;
    };

    // SealedCircle cannot be inherited from
    class SealedCircle final : public Shape {
    public:
        // draw() implicitly final because SealedCircle is final
        void draw() const override { /* draw circle */ }
    };

    // class SpecialCircle : public SealedCircle {}; // COMPILE ERROR: SealedCircle is final

    class Polygon : public Shape {
    public:
        // Make area calculation final for all Polygons inheriting from this
        virtual double area() const final { /* specific polygon area calc */ return 0.0;}
        void draw() const override { /* draw polygon */ }
    };

    class Square : public Polygon {
    public:
        // void area() const override; // COMPILE ERROR: Polygon::area is final
        // draw() can still be overridden as it wasn't final in Polygon
        void draw() const override { /* draw square */ }
    };
    ```

**5. Define Interfaces with Abstract Classes (Pure Virtual Functions)**

* **Concept:** If a base class exists purely to define an interface (a contract for what derived classes *must* do) without providing a meaningful implementation itself, make it abstract using pure virtual functions (`= 0`).
* **Why:** Enforces that derived classes *must* provide implementations for the core interface methods. Prevents instantiation of the incomplete base class.
* **How:** Declare at least one virtual function with `= 0;`. Often combined with a `protected` constructor.
* **Example:**
    ```c++
    class Serializable { // Interface class
    public:
        virtual ~Serializable() = default;
        // Pure virtual functions define the interface contract
        virtual void serialize(std::ostream& os) const = 0;
        virtual void deserialize(std::istream& is) = 0;
    protected:
        // Protected constructor prevents direct instantiation of Serializable
        Serializable() = default;
        // Deleted copy/move also typical for interfaces
        Serializable(const Serializable&) = delete;
        // ... etc ...
    };

    class MyData : public Serializable { // Concrete class implementing the interface
    public:
        void serialize(std::ostream& os) const override { /* write data */ }
        void deserialize(std::istream& is) override { /* read data */ }
    private:
        int data_;
    };

    int main() {
        // Serializable s; // COMPILE ERROR: Cannot instantiate abstract class
        MyData d;         // OK
        Serializable& ref = d; // OK, polymorphic reference
        ref.serialize(std::cout);
        return 0;
    }
    ```

**6. Use the Non-Virtual Interface (NVI) Idiom**

* **Concept:** Make public interface methods non-virtual. These methods handle setup, teardown, logging, or enforcing invariants, and then call private/protected `virtual` methods ("hooks") that derived classes override to provide the specific behavior.
* **Why:** Allows the base class to control the overall algorithm template while letting derived classes customize specific steps. Separates the public contract from the internal customization mechanism.
* **Example:**
    ```c++
    #include <iostream>

    class Task {
    public:
        // Public Non-Virtual Interface method
        void execute() {
            std::cout << "Base: Preparing task...\n";
            if (preExecuteCheck()) {
                doExecute(); // Call the virtual hook
            }
            std::cout << "Base: Finalizing task...\n";
        }
        virtual ~Task() = default;

    protected:
        // Protected virtual hook for customization
        virtual void doExecute() = 0;
        // Optional virtual hook
        virtual bool preExecuteCheck() { return true; }

        Task() = default; // Protected constructor if abstract
    };

    class SpecificTask : public Task {
    protected:
        void doExecute() override {
            std::cout << "  Derived: Performing specific action!\n";
        }
         bool preExecuteCheck() override {
            std::cout << "  Derived: Running pre-check...\n";
            return true;
        }
    };

    int main() {
        SpecificTask st;
        Task& baseRef = st;
        baseRef.execute(); // Calls the NVI method
        return 0;
    }
    // Output:
    // Base: Preparing task...
    //   Derived: Running pre-check...
    //   Derived: Performing specific action!
    // Base: Finalizing task...
    ```

**7. Use Smart Pointers for Polymorphic Object Management**

* **Concept:** When dealing with dynamically allocated polymorphic objects, use smart pointers (`std::unique_ptr` for unique ownership, `std::shared_ptr` for shared ownership) instead of raw pointers.
* **Why:** Automates memory management (calls `delete` automatically, invoking the correct virtual destructor), prevents memory leaks, and clearly defines ownership semantics.
* **Example:** (See examples 1 and 5 which use `std::unique_ptr`)

**8. Prefer Composition Over Inheritance (Where Appropriate)**

* **Concept:** Inheritance creates a strong "is-a" relationship. If the relationship is more "has-a" or "uses-a", prefer composition (including another class as a member variable) or aggregation (storing a pointer/reference to another class).
* **Why:** Inheritance creates tight coupling. Composition is often more flexible, easier to change, and avoids complex multiple inheritance issues. Use public inheritance primarily when you need runtime polymorphism based on a common interface or substitutability ("is-a").

These practices help ensure your inheritance hierarchies are robust, maintainable, safe from common errors like slicing and resource leaks, and correctly implement polymorphic behavior.