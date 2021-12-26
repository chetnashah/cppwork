

when implicit conversion happens, temporaries are involved.

### implicit conversion from literal to non-const reference is forbidden

```cpp
int testme(int& k){ // COMPILER Error!!
    return k;
}
int main(){
    testme(3);
    return 0;
}
```

### implicit conversion from literal to const reference is fine

```cpp
int testme(const int& k){
    return k;
}
int main(){
    testme(3);
    return 0;
}
```