
## Every name in the program is part of certain scope

- **Scope**: The region of the program where a name is valid and can be used to refer to an entity.

## Scope types

1. **Global scope** - A global name is one that is declared outside of any class, function, or namespace. However, in C++ even these names exist with an implicit global namespace. The scope of global names extends from the point of declaration to the end of the file in which they are declared. For global names, visibility is also governed by the rules of linkage which determine whether the name is visible in other files in the program.

2. **Namespace scope** -  A name that is declared within a namespace, outside of any class or enum definition or function block, is visible from its point of declaration to the end of the namespace. A namespace may be defined in multiple blocks across different files. All identifiers at namespace scope are visible to one another without qualification. Identifiers outside the namespace can access the members by using the fully qualified name for each identifier, for example `std::vector<std::string> vec;`.

3. **Local scope** - A name declared within a function or lambda, including the parameter names, have local scope. They are often referred to as "locals". They are only visible from their point of declaration to the end of the function or lambda body. Local scope is a kind of block scope, which is discussed later in this article.

4. **Class scope** - Names of class members have class scope, which extends throughout the class definition regardless of the point of declaration. Class member accessibility is further controlled by the public, private, and protected keywords. Public or protected members can be accessed only by using the member-selection operators (. or ->) or pointer-to-member operators (.* or ->*).

5. **Statement scope** - Names declared in a for, if, while, or switch statement are visible until the end of the statement block.

6. **Function scope** - A label has function scope, which means it is visible throughout a function body even before its point of declaration. Function scope makes it possible to write statements like goto cleanup before the cleanup label is declared.

## Relationship between scope and linkage

**Internal Linkage** - When a name has internal linkage, that name can be referred to from all scopes in the current translation unit. Static functions, variables, and their templated version, they all have internal linkage. Also, any names declared in an unnamed namespace have this level of linkage.

**External Linkage** - When a name has external linkage, it can be referred to from the scopes of another translation unit. This can go as far as using variables and functions from translation units that were written in another language. Enumerations, class names and their member functions and static data members, non-static templates and class templates, etc.

**Module Linkage** - Module linkage was introduced in C++20. When a name has module linkage, it can only be referred to from the same module unit. This might mean another translation unit.

