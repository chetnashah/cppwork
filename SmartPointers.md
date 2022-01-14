


### raw pointer

raw pointers are copyable

### unique_ptr

`unique_ptr` is move only.
The move constructor, will null out the source pointer.
`unique_ptr` has its copy constructors deleted, so if you try to do copy initialization you will receive following error:
```cpp
void uptrtaker(std::unique_ptr<int> aarg) {
    std::cout << "hey" << std::endl;
}

int main(){
    std::unique_ptr<int> j = std::make_unique<int>(2);
    std::unique_ptr<int> k = j;// compiler Error! Call to implicitly-deleted copy constructor of 'std::unique_ptr<int>'
    std::unique_ptr<int> l(j);;// compiler Error! Call to implicitly-deleted copy constructor of 'std::unique_ptr<int>'

    std::unique_ptr<int> z = std::move(j); // Ok! move constructor of unique_ptr invoked due to cast by std::move

    uptrtaker(j); // compiler Error! Call to implicitly-deleted copy constructor of 'std::unique_ptr<int>'
    uptrtaker(std::move(j)); // Ok
}
```

#### custom deleters can do nice things!

```cpp
struct FileCloser {
    void operator()(FILE* fp) const {
        assert(fp != nullptr);
        fclose(fp);
    }
}

FILE* fp = fopen("input.txt", "r");
std::unique_ptr<FILE, FileCloser> uptr(fp);
```

### shared_ptr

Involves reference counting!


