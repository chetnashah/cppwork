

This repo contains all the cpp work independent of the underlying platform.
That is no windows/unix api's will be used.

## Array decay

Array decay refers to the automatic conversion of an array to a pointer when the array is used in certain contexts in C or C++.

when an array is used in most expressions, it is automatically converted to a pointer to its first element.

```cpp
#include <stdio.h>

int main() {
    int myArray[5] = {1, 2, 3, 4, 5};

    // Using the array in a context that triggers decay
    int* pointerToFirstElement = myArray;

    // Here, myArray is decayed to a pointer
    // Equivalent to: int* pointerToFirstElement = &myArray[0];

    printf("Value at the first element: %d\n", *pointerToFirstElement);

    return 0;
}
```

### Loss of size information

When an array decays to a pointer, the size information is lost. 

This can lead to potential issues if the function that receives the pointer needs to know the size of the array for correct processing.

