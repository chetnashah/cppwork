# “Default construction” in the C++ Standard Library  
*(what it is, when it happens, and why it matters)*

---

## 1 . What exactly is “default construction”?

• **Default-initialisation** (`T obj;`)  
&nbsp;&nbsp;*If* `T` has a user-provided or implicitly-declared default constructor, that constructor is called.  
&nbsp;&nbsp;*If* `T` is an aggregate or a trivial type with no user ctor, its contents are left **indeterminate** (uninitialised).

• **Value-initialisation** (`T obj{}` or `new T{}`)  
&nbsp;&nbsp;The object is zero-initialised first, then the default ctor (if any) runs.  
&nbsp;&nbsp;All Standard Library containers use **value-initialisation** when they need an object “from thin air”; you get fully initialised objects, never indeterminate bits.

So, in library wording, “default-construct” really means “value-initialise”.

---

## 2 . Places where containers create **brand-new** elements

| Container / Function              | Condition that triggers a new element | Requirement on `T`            |
|-----------------------------------|---------------------------------------|--------------------------------|
| `std::vector<T>(n)`               | Constructor with *count*              | `T` must be **DefaultInsertable** (i.e. value-initialisable) |
| `std::vector<T>::resize(n)`       | New size larger than current          | same                           |
| `std::deque`, `std::list`, …      | same as vector                        | same                           |
| `std::array<T,N>{}`               | Static storage                        | `T` value-initialised **N** times |
| `std::map<Key,T>::operator[]`     | Key missing                           | `T` default-constructed (empty string, empty vector, zero, …) |
| `std::unordered_map::operator[]`  | Key missing                           | same                           |
| `std::optional<T>` default ctor   | `optional<T>{}` stores **no** `T`     | No requirement; nothing is built |
| `std::optional<T>::emplace()`     | Constructs *in-place*                 | Same as calling `T`’s ctor     |
| `std::stack`, `std::queue`, …     | Underlying `std::deque` does the work | See deque                      |

---

### Example 1 – vector growth

```cpp
std::vector<std::string> v(2);  // two default-constructed std::strings: ""
v.resize(5);                    // three more empty strings are value-initialised
```

### Example 2 – map `operator[]`

```cpp
std::map<int, std::vector<double>> m;
m[42].push_back(3.14);    // if 42 not present:
                          //   1) create new node
                          //   2) value-initialise its vector => empty
                          //   3) push_back(3.14)
```

No explicit `new` is visible because the container uses its allocator internally and invokes `::new (p) T{}` (placement-new) on raw storage.

---

## 3 . APIs that *do not* need default construction

* `push_back`, `emplace_back`, `insert`, `try_emplace`, `emplace` – you provide a fully built object or the ctor args.
* `std::vector<T>::reserve`, `shrink_to_fit` – only manipulate capacity.
* Ranges algorithms that copy/move existing objects (`copy`, `move`, …).

Therefore you can perfectly store a **non-default-constructible** type in a vector *as long as you never ask the vector to create one itself*:

```cpp
struct ND {
    ND(int x) : x(x) {}
    ND() = delete;          // no default ctor
    int x;
};

std::vector<ND> v;
v.emplace_back(1);          // OK
v.emplace_back(2);          // OK
// v.resize(10);            // ERROR: needs default ctor
```

---

## 4 . Why should you care?

1. **Compile-time constraints**  
   Code that calls `resize`, `operator[]`, etc. will **fail to compile** if `T` lacks an accessible default constructor.  

2. **Performance**  
   Creating thousands of heavy objects (e.g. `std::string` or large `std::array`) just to overwrite them later wastes time and allocations. Prefer `reserve` + `push_back` if the data will come immediately.

3. **Exception Safety & `noexcept`**  
   Operations are `noexcept` only when the element’s default constructor is `noexcept`. For example, `vector<int>`’s `resize` is `noexcept`, but `vector<std::string>`’s is not.

4. **Zero vs indeterminate memory**  
   Because containers *value-initialise*, the memory is predictable: integers are zero, pointers are `nullptr`, strings are empty, etc. This saves you from the UB pitfalls of uninitialised data.

---

## 5 . Little glossary

* **DefaultConstructible** – the type has a default constructor that can be called with no arguments (possibly deleted).  
* **DefaultInsertable** (*containers TS, now part of the standard*) – a stricter concept that also requires not throwing on construction when the allocator is used; containers rely on it.
* **TriviallyDefaultConstructible** – compiler can memset(0) the object. `int`, POD structs, etc.

---

## 6 . Summary cheat-sheet

* Library containers and some algorithms may **value-initialise** elements behind your back; make sure your element type supports it if you call those APIs.
* `operator[]` on associative containers = “create-on-first-use” (default construction).
* If you store a type without a default constructor, avoid functions that need to conjure up extra elements (`resize`, vector(count) ctor, etc.).
* For efficiency with large types, prefer `reserve` + `push_back` or `emplace_back`.
* You rarely need to worry about indeterminate memory: the Standard Library does the safe thing and gives you fully initialised objects.

That’s the essence of default construction inside the STL—silent, pervasive, and occasionally a compilation hurdle if your types forbid it.