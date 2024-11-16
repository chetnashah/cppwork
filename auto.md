

## `auto` drops const and reference qualifiers (unless explicitly specified)

**Best practice** - If you want a const reference, reapply the const qualifier and reference even when it’s not strictly necessary, as it makes your intent clear and helps prevent mistakes.

## Hack to find out deduced type

```cpp
template <typename T>
class TD; // Type displayer

int main()
{
    const int k = 1;
    auto a = k;
    TD<decltype(a)> td; // TD<int> - deduced type is int

    const int &kr = k;
    auto& b = kr;
    TD<decltype(b)> td2; // TD<const int&> - deduced type is const int&
}
```

### `auto` dropping const
```cpp
void autodroppingconst()
{
    const int k = 1;
    auto a = k; // a is int (const dropped), copied from k
    a = 11; // a is modifiable as it is plain int
    std::cout << "a = " << a << std::endl;// 11
    std::cout << "k = " << k << std::endl;// 1
}
```

### `auto` dropping refernce

```cpp
void autodroppingreference()
{
    int x = 1;
    int &y = x;
    auto a = y; // a is int (reference dropped), copied from y

    if (std::is_reference_v<decltype(a)>)
        std::cout << "a is a reference" << std::endl;
    else
        std::cout << "a is not a reference" << std::endl;
}
```

### auto dropping both const and ref

```cpp
void autodroppingconstandref()
{
    const int k = 1;
    const int &kr = k;
    auto a = kr;                                                          // a is int (const and reference dropped), copied from kr
    auto isconst = std::is_const_v<std::remove_reference_t<decltype(a)>>; // false
    std::cout << "a is const? = " << std::boolalpha << isconst << std::endl;

    auto isref = std::is_reference_v<std::remove_const<decltype(a)>>; // false
    std::cout << "a is a reference? = " << std::boolalpha << isref << std::endl;
}
```

## Solution: reapply const and reference qualifiers as needed

### auto with const re-applied

```cpp
void autodwithconst()
{
    const int k = 1;
    // auto dropped const, but we re-applied const qualifier at the same site
    auto const a = k;
    // a = 11;
    std::cout << "a = " << a << std::endl;
    std::cout << "k = " << k << std::endl;
}
```

### auto with reference re-applied

```cpp
void autowithreference()
{
    int x = 1;
    int &y = x;
    // auto dropped reference qualifier, but we reapplied here.
    auto &a = y;

    if (std::is_reference_v<decltype(a)>)
        std::cout << "a is a reference" << std::endl; // prints
    else
        std::cout << "a is not a reference" << std::endl;
}
```

### auto with const and ref re-applied

```cpp
```

## Note: `auto` does not drop pointers

Unlike references, **auto type deduction does not drop pointers**:
```cpp
void autoDoesNotDropPointers()
{
    const int *p = nullptr;
    auto a = p; // a is const int*
}
```

## Note: `auto` preserves low level const, but strips top level const

Great question! Understanding how `const` interacts with `auto` type deduction in C++ can be a bit nuanced. Let's dive into why the `const` qualifier isn't dropped in your specific example and clarify the general rules governing this behavior.

## **Your Code Example**

```cpp
const int val = 5;
const int *ptr = &val;

auto p1 = ptr;  // 'const int*'
```

### **Why `const` Isn't Dropped for `p1`**

In this scenario:

- `ptr` is of type `const int*`, meaning it's a **pointer to a `const int`**.
- The `const` qualifier here is a **low-level `const`** because it applies to the data being pointed to (`int`), not to the pointer itself.

When you use `auto` to deduce the type of `p1`:

```cpp
auto p1 = ptr;  // Deduces to 'const int*'
```

- **Low-Level `const` Preservation:** `auto` preserves **low-level `const`** qualifiers. Since `const` is associated with the `int` (the pointee) and not the pointer, `auto` retains it.
- **Top-Level `const` Stripping:** `auto` only strips **top-level `const`** qualifiers, which are `const` qualifiers that apply directly to the variable itself (e.g., the pointer) rather than to the data it points to.

In your case, since the `const` is low-level, `auto` retains it, resulting in `p1` being of type `const int*`.

## **Distinguishing Top-Level vs. Low-Level `const`**

To understand why `const` is or isn't dropped, it's crucial to differentiate between **top-level** and **low-level** `const`:

1. **Top-Level `const`:**
   - **Definition:** Applies directly to the object/variable itself.
   - **Example:** `int* const ptr;` Here, `const` applies to the pointer `ptr` itself.
   - **Behavior with `auto`:** Stripped during type deduction.

2. **Low-Level `const`:**
   - **Definition:** Applies to the data being pointed to or referenced.
   - **Example:** `const int* ptr;` Here, `const` applies to the `int` data that `ptr` points to.
   - **Behavior with `auto`:** Preserved during type deduction.

### **Illustrative Examples**

Let's explore how `auto` behaves with both top-level and low-level `const`:

#### **1. Low-Level `const` (Your Example)**

```cpp
const int val = 5;
const int *ptr = &val;

auto p1 = ptr;  // 'p1' is deduced as 'const int*'
```

- **Explanation:** Since `const` is low-level (`const int*`), `auto` preserves it. Thus, `p1` remains a `const int*`.

#### **2. Top-Level `const`**

```cpp
int val = 5;
int* const ptr = &val;  // 'ptr' is a const pointer to 'int'

auto p2 = ptr;  // 'p2' is deduced as 'int*'
```

- **Explanation:** Here, `const` is top-level (`int* const`). `auto` strips the top-level `const`, so `p2` is deduced as `int*`, **losing the `const` qualifier on the pointer itself**.

#### **3. Both Top-Level and Low-Level `const`**

```cpp
const int val = 5;
const int* const ptr = &val;  // 'ptr' is a const pointer to a const int

auto p3 = ptr;  // 'p3' is deduced as 'const int*'
```

- **Explanation:** `const` exists at both top and low levels. `auto` strips the top-level `const` (applicable to `ptr`), but since `const int*` remains, `p3` is deduced as `const int*`.

## **Key Takeaways**

1. **`auto` Strips Only Top-Level `const`:**
   - If `const` applies directly to the variable you're deducing (`top-level`), `auto` will remove it.
   - If `const` applies to the data the variable points to or references (`low-level`), `auto` preserves it.

2. **Determine `const` Level:**
   - Analyze whether `const` modifies the pointer/reference itself or the data it points to.
   - Use the **right-left** rule to parse complex declarations.

3. **Preserving `const` in `auto` Deductions:**
   - To retain **top-level `const`**, explicitly declare it:
     ```cpp
     auto const p4 = ptr; // 'const int*' remains the same as low-level const is preserved
     ```
   - To ensure both levels are preserved when both exist:
     ```cpp
     const auto* p5 = ptr;  // 'const int*'
     ```

## **Additional Examples for Clarity**

### **Example 1: Stripping Top-Level `const`**

```cpp
int value = 10;
int* const ptr = &value;  // 'ptr' is a const pointer to 'int'

auto p = ptr;  // p is 'int*', top-level 'const' is stripped
```

- **Result:** `p` is of type `int*`. The `const` on the pointer itself is removed, but the `int` remains non-const.

### **Example 2: Preserving Low-Level `const`**

```cpp
const int value = 10;
const int* ptr = &value;  // 'ptr' is a pointer to a const int

auto p = ptr;  // p is 'const int*', low-level 'const' is preserved
```

- **Result:** `p` is of type `const int*`. The `const` on the pointed-to data is preserved.

### **Example 3: Both Top-Level and Low-Level `const`**

```cpp
const int value = 10;
const int* const ptr = &value;  // 'ptr' is a const pointer to a const int

auto p = ptr;  // p is 'const int*', top-level 'const' is stripped
```

- **Result:** `p` is of type `const int*`. The `const` on the pointer is stripped, but the `const` on the pointed-to data remains.

## **Best Practices to Manage `const` with `auto`**

1. **Be Explicit When Necessary:**
   - If preserving `const` is crucial, consider being explicit in your type declarations.
   - For instance, use `const auto*` or `const auto&` to clearly indicate intent.

2. **Use References to Preserve Qualifiers:**
   - When using `auto` with references, the qualifiers are preserved naturally.
     ```cpp
     const int val = 10;
     auto& ref = val;  // 'ref' is 'const int&'
     ```

3. **Understand the Context:**
   - Always analyze whether `const` is top-level or low-level in your declarations.
   - This awareness helps predict how `auto` will behave during type deduction.

4. **Leverage `const` Correctness:**
   - Use `const` to express intent and prevent accidental modifications.
   - When using `auto`, be mindful of how `const` qualifiers are handled to maintain this correctness.

## **Conclusion**

In your specific example:

```cpp
const int *ptr = &val;

auto p1 = ptr;  // 'const int*'
```

- The `const` qualifier is **low-level** (applies to the `int`), and thus **`auto` preserves it**, resulting in `p1` being of type `const int*`.

Understanding the distinction between top-level and low-level `const` is essential for accurately predicting how `auto` will deduce types in various scenarios. This knowledge not only helps prevent unintended type stripping but also ensures that your code maintains the intended `const` correctness and safety.

## Key Insight: dropping reference can convert a low level const to a top level const

Dropping a reference may change a low-level const to a top-level const: `const std::string&` is a low-level const, but dropping the reference yields `const std::string`, which is a top-level const.


