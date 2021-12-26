

### Function blue prints

`T` is referred as type parameter.

```cpp
template <typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}
int main(){
    int a{1};
    int b{1};
    int c = maximum(a, b); // type deduced for T is int
}
```

### specify explicit type for template


```cpp
int main(){
    maximum<double>(c,d);// explicitly specify type param as double
}
```

### Template specialization

```cpp
template <typename T> T maximum(T a, T b);

//Template specialization - provide better fits 
// done because we don't want to do pointer comparision but data comparision
const char* maximum<const char*> (const char* a, const char* b){
    return (std::cmp(a,b) > 0) ? a : b;
}
```

### Function template with overloading


direct matches are preferred over template matches.


### Auto return type deduction

```cpp
template <typename T, typename P>
auto maximum(T a, P b){
    return (a>b) ? a : b;
}
```