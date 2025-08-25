# All the Common Ways to Iterate over `std::map` (C++11-C++23)

Below is a “cook-book” of idioms—from the very plain for-loop to modern
`std::ranges`—showing how to walk through a `std::map<Key,Value>`.

Assume we start with:

```cpp
std::map<std::string,int> m{
    {"alice", 3},
    {"bob",   5},
    {"carol", 8}
};
```

---

## 1. Classic iterator loop

```cpp
for (std::map<std::string,int>::iterator it = m.begin();
     it != m.end(); ++it)
{
    std::cout << it->first << " = " << it->second << '\n';
}
```

Verbose, but works in every C++ version.

---

## 2. Range-based `for`  (C++11)

```cpp
for (const auto &kv : m)                       // kv is a const std::pair<const Key,Val>&
    std::cout << kv.first << " -> " << kv.second << '\n';
```

Mutating the pair’s `second` is allowed:

```cpp
for (auto &kv : m)
    kv.second *= 2;                            // ok; key is const, value is not
```

---

## 3. Structured bindings  (C++17)

```cpp
for (auto &[key, value] : m)                   // reference binding
    std::cout << key << ':' << value << '\n';
```

Add `const` if you do not intend to modify `value`:

```cpp
for (const auto &[key, value] : m) { /* read-only */ }
```

---

## 4. `auto` iterators when you need `erase`, `lower_bound`, … (C++11)

```cpp
for (auto it = m.begin(); it != m.end(); ) {
    if (it->second < 5)
        it = m.erase(it);                      // erase returns next iterator
    else
        ++it;
}
```

---

## 5. Reverse order  (`rbegin` / `rend`)

```cpp
for (auto it = m.rbegin(); it != m.rend(); ++it)
    std::cout << it->first << '\n';
```

Or in C++14+:

```cpp
for (auto & [k,v] : std::views::reverse(m))    // needs <ranges> C++20
    std::cout << k << '\n';
```

---

## 6. Iterate **keys only** or **values only**

C++20 one-liner using `<ranges>` views:

```cpp
// keys
for (const auto &k : std::views::keys(m))
    std::cout << k << ' ';

// values
for (auto &v : std::views::values(m))          // v is int&
    v += 10;
```

Without ranges:

```cpp
for (auto it = m.begin(); it != m.end(); ++it)
    std::cout << it->first << ' ';
```

---

## 7. Enumerate with an index counter

```cpp
std::size_t idx = 0;
for (const auto &[k,v] : m)
    std::cout << "#" << idx++ << " " << k << '\n';
```

Or C++23 `views::enumerate`:

```cpp
for (auto [i, kv] : std::views::enumerate(m))
    std::cout << i << ": " << kv.first << '\n';
```

---

## 8. `std::for_each` / algorithms

```cpp
std::for_each(m.begin(), m.end(), [](const auto &kv){
    std::cout << kv.first << "=" << kv.second << '\n';
});
```

With ranges (C++20):

```cpp
std::ranges::for_each(m, [](const auto &kv){ /* … */ });
```

---

## 9. Parallel iteration of two maps (zip) – C++23

```cpp
for (auto && [a, b] : std::views::zip(m, otherMap))
    std::cout << a.first << " vs " << b.first << '\n';
```

---

## 10. Early exit (`break`) based on predicate

```cpp
for (const auto &[k,v] : m) {
    if (v == 5) { std::cout << "found\n"; break; }
}
```

---

## 11. Safe iteration while inserting elsewhere

`std::map` iterators are **not** invalidated by insertions/erases that occur at
different nodes, so you can do this safely:

```cpp
for (auto it = m.begin(); it != m.end(); ++it)
    m.emplace("x"+it->first, it->second);      // fine
```

---

### Quick cheat-sheet

• read-only: `for (const auto &[k,v] : m)`  
• read/write value: `for (auto &[k,v] : m)`  
• reverse: `for (auto it=m.rbegin(); it!=m.rend(); ++it)`  
• erase-while-iterating: `it = m.erase(it)` idiom  
• keys/values view: `<ranges>` → `std::views::keys/values(m)`

These idioms cover virtually every pattern you’ll need when traversing an
ordered `std::map`.