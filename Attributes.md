
Attributes are a mechanism to add optional and/or vendor-specific information into source code. Before attributes were standardized in C++, vendors decided how to specify such information. Examples are `__attribute__`, `__declspec`, and so on. Since C++11, there is standardized support for attributes by using the double square brackets syntax `[[attribute]]`. The `[[attribute]]` syntax is used to specify attributes in C++11 and later versions.

## Interesting attributes

### `[[nodiscard]]` - caller should not ignore the return value

The `[[nodiscard]]` attribute can be used on a function that returns a value. The compiler will then issue a warning if the return value from that function is not used by the calling function. 
**This feature can, for example, be used for functions that return error codes or status values, where ignoring the return value could lead to bugs.**

Here is an example:
```cpp
[[nodiscard]] int func() { return 42; }
 
int main()
{
    func();
}
```
The compiler issues a warning similar to the following:
```
warning C4834: discarding return value of function with 'nodiscard' attribute
```

### `[[noreturn]]`

Adding a `[[noreturn]]` attribute to a function means that it never returns control to the caller. 
**Typically, the function either causes some kind of termination (process termination or thread termination) or throws an exception.**

```cpp
[[noreturn]] void forceProgramTermination()
{
    exit(1);  // Defined in <cstdlib>
}
```

### `[[deprecated]]`
