Templates are lot more powerful than regular generics.

```cpp
// this fn only gets created if it is used somewhere in the code
template<typename T>
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

