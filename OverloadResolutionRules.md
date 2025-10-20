# 🧭 C++ Overload Resolution & Binding — Comprehensive Cheat Sheet

## 1️⃣ Overview
Overload resolution decides **which function or operator** is called for a given expression.

**Process:**
1. Find all visible overloads.
2. Keep only *viable* ones (arguments can be converted).
3. Rank each conversion sequence.
4. Pick the overload that requires **the least work** (fewest or simplest conversions).

---

## 2️⃣ Value Categories (Expression Side)

| Category | Meaning | Example | Can Bind To |
|-----------|----------|----------|-------------|
| **lvalue** | Has identity (address, name) | `x`, `*p` | `T&`, `const T&` |
| **xvalue** | Expiring object | `std::move(x)` | `T&&`, `const T&` |
| **prvalue** | Pure temporary | `42`, `T{}` | `T&&`, `const T&`, `T` |

Every **expression** has both a **type** and a **value category**.  
Category influences overload resolution and binding behavior.

---

## 3️⃣ Parameter Types (Function Side)

| Parameter Form | Binds To | Behavior |
|----------------|-----------|-----------|
| `T&` | modifiable lvalue | reference, no copy |
| `const T&` | any compatible value | read-only reference |
| `T&&` | rvalues (xvalue/prvalue) | move source |
| `T` | any | copy/move creates new object |

---

## 4️⃣ Reference Binding Rules

| Argument → | `T&` | `const T&` | `T&&` | `T` |
|-------------|------|-------------|-------|-----|
| **lvalue** | ✅ exact | ✅ via const | ❌ | ✅ copy |
| **const lvalue** | ❌ | ✅ exact | ❌ | ✅ copy |
| **xvalue** | ❌ | ✅ | ✅ exact | ✅ move |
| **prvalue** | ❌ | ✅ | ✅ exact | ✅ move |
| **const prvalue** | ❌ | ✅ | ❌ | ✅ copy |

**Priority rule:**  
Direct reference binding beats copy construction — unless copy/move can be elided (class prvalues).

---

## 5️⃣ Ranking of Conversion Sequences

1. **Exact match** (same type, ref bind, or add const)
2. **Promotion** (e.g. `char → int`)
3. **Standard conversion** (`int → double`)
4. **User-defined conversion** (constructor or conversion operator)
5. **Ellipsis match (`...`)** — worst

---

## 6️⃣ Tie-Breakers

| Rule | Preference |
|------|-------------|
| Non-template > Template | Non-template wins if equally ranked |
| `T&` > `const T&` | for same lvalue |
| `T&&` > `const T&` / `T` | for rvalues |
| `T` > `const T&` | for same-type class prvalues (elision) |
| Fewer cv-qualifiers added | wins |
| Equal conversions | → **ambiguous** (compile error) |

---

## 7️⃣ Pass-by-Value vs Reference

- Reference binding → no new object → cheaper → preferred.  
- Pass-by-value (`T`) → copy or move construction → costlier.  
- **Exception:** prvalue of same class type → `T` wins (copy elision).

---

## 8️⃣ Inside the Function

| Parameter Type | Inside Function |
|----------------|-----------------|
| `T&`, `const T&`, `T&&` | alias to caller’s object |
| `T` | copy/move of argument |
| Any named parameter | is **always an lvalue** |
| To forward ownership | use `std::move(param)` or `std::forward<T>(param)` |

---

## 9️⃣ Templates & Deduction

- `T&&` in a deduced context = **forwarding reference** (binds to any category).  
- `std::forward<T>(u)` restores original category.  
- Non-template overloads are preferred if equally good.

---

## 1️⃣0️⃣ Names and Binding

- A **name** used as an expression is always an **lvalue**, even if declared `T&&`.  
- In assignments and function calls, **names bind to expressions**; their category decides whether copy, move, or reference binding occurs.

Example:
```cpp
int&& r = 5;
r;            // lvalue expression
std::move(r); // xvalue expression
```

---

## 1️⃣1️⃣ Common Gotchas

| Situation | Explanation |
|------------|-------------|
| Named rvalue ref (`T&& x`) | acts as lvalue inside the function |
| `const T&` binds to temp | may hide copies (extends lifetime) |
| `const T&&` | almost never useful |
| `T&&` vs `const T&` | `const T&` wins for const rvalues |
| Class prvalue | prefers by-value (`T`) |
| Fundamental prvalue | prefers `T&&` |
| Member function ref-qualifiers | select overload based on lvalue/rvalue of `*this` |
| Template vs non-template | non-template wins when equally ranked |
| Multiple standard conversions | can lead to ambiguity |

---

## 1️⃣2️⃣ Quick Reference Matrix

| Argument | Preferred Overload | Why |
|-----------|--------------------|-----|
| lvalue | `T&` | exact bind |
| const lvalue | `const T&` | only legal |
| rvalue / xvalue | `T&&` | direct bind |
| const rvalue | `const T&` | can’t move |
| prvalue (class) | `T` | elision possible |
| prvalue (fundamental) | `T&&` | direct bind |
| fallback | `const T&` | universal receiver |
| tie between template & non-template | pick non-template | |

---

## 1️⃣3️⃣ Mental Model Mantra

> Every argument expression has a **value category**.  
> Every parameter declares how it wants to **bind**.  
> Overload resolution picks the path of **least resistance** —  
> fewest conversions, no extra objects, minimal cv-adjustments.  
>  
> Inside a function, every parameter is a **name**, hence an **lvalue**.

---

## 1️⃣4️⃣ Quick Diagnostic Checklist

When calls get confusing:
1. Identify argument **value category** (lvalue / xvalue / prvalue).  
2. List all candidate overloads.  
3. Eliminate non-viable ones.  
4. Rank remaining by conversion cost.  
5. Apply tie-breakers.  
6. Remember non-template preference.  
7. Inside the chosen overload, treat parameter names as lvalues.

---

## 🧠 Rule of Thumb Summary
| Rule | Mnemonic |
|------|-----------|
| Reference before copy | “Bind, don’t clone.” |
| Move before copy | “Move if you can.” |
| Value before const-ref (class prvalue) | “Elide when same type.” |
| Non-template before template | “Prefer concrete.” |
| Names are lvalues | “A name always has an address.” |

---

**Keep this handy:** reread it once a week, and it’ll permanently anchor how C++ decides what to call and when a move, copy, or bind happens.
