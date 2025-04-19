
## Iteration best practice (use const auto& and auto&)

When iterating over a collection coll containing elements of type T:

If you only need read-only access to elements:

### Use for (const auto& item : coll).
This is the most common and generally preferred way. **It's efficient (no copies) and safe (prevents accidental modification).** This directly addresses the Clang-Tidy warning.
If you need to modify the elements within the collection:

### Use for (auto& item : coll).
**This gives you a mutable reference to each element**, allowing you to change the original items in the collection. Use this only when modification is intended.
If you need a copy of each element (less common):

### Use for (auto item : coll).
**This explicitly creates a copy.** Reasons might include:
You want to modify the loop variable item itself without affecting the original collection.
The elements are very cheap to copy (e.g., primitive types like int, float, pointers) and you specifically want a value rather than a reference. However, const auto& is often still good practice even for cheap types for consistency.
Be aware that this can have performance implications for non-trivial types and might trigger the Clang-Tidy warning if the copy isn't actually utilized in a way that requires it.
