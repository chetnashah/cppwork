
### string literal

Type for string literal is `const char *`

### char array representation

```cpp
char message[] {"hi world"};// note [] after variable name instead of after char like java
char msg2[] {'H','e', 'y','\0'};// same as above
```

### const char* representation

```cpp
const char* mm = "hey world";// no need to delete this, since no new was used
cout << mm << endl;
```

### std::string
