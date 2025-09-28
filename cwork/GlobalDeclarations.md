Exactly. At **file scope (global level)** in C:

* You can only write **declarations/definitions** (variables, functions, typedefs, structs, enums, etc.).
* You **cannot place executable statements** there — so no assignments, no function calls, no loops, nothing that represents "do this now."

That’s why something like:

```c
int k = 5;       // valid: declaration + definition (goes into data segment)
foo();           // ❌ invalid: statement not allowed at file scope
```

fails to compile.

---

### Why?

Because:

* Global variables (`int k = 5;`) are part of the program’s **data segment**. Their values are set up before `main()` starts running (typically by static initialization done by the runtime/loader).
* Function bodies are the only places where **statements** (runtime instructions) can appear. The compiler needs a clear separation: global scope is for "what exists," function scope is for "what happens."

---

### Special case: static initialization

When you write `int k = 5;` at global scope, the compiler encodes `5` into the binary’s **.data section**. No runtime assignment instruction executes in `main()` — it’s as if the variable comes pre-loaded with `5` when the program image is loaded into memory.
If you don’t initialize, e.g. `int k;`, it goes into **.bss section** (zero-initialized by the runtime).

So yes: **no executable code at global level, only declarations/definitions**.

