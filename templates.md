Templates are lot more powerful than regular generics.

```cpp
// this fn only gets created if it is used somewhere in the code
template<typename T> // note typename is an extra keyword, class also works
void Print(T value) {
    std::cout << value << std::endl;
}

int main(){
    Print(5);// implicitly inferred typename
    Print<int>(5);// same but explicit typename provided
    return 0;
}
```

One can think of templates that it will generate (Rendered) code at compile time
based on its usage within the code.

Templates can also be used on classes like following:
```cpp
template<typename K>
class SomeClass {
    // Use K in someway..
}
```

## class vs typename

In summary, both class and typename can be used to declare template parameters in C++ templates, but typename is often preferred for clarity, especially when dealing with nested types or dependent names.

