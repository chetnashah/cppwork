

There can be more than one definition of a class type, enumeration type, inline function with external linkage, class template, non-static function template, static data member of a class template, member function of a class template, or template specialization for which some template parameters are not specified in a program provided that each definition appears in a different translation unit, and provided the definitions satisfy the following requirements. Given such an entity named D defined in more than one translation unit....

If the definitions of D satisfy all these requirements, then the program shall behave as if there were a single definition of D. If the definitions of D do not satisfy these requirements, then the behavior is undefined.

The requirements specified by paragraph 6 essentially state that that two definitions must be identical (not simply equivalent). Consequently, a definition introduced in two separate translation units by an #include directive generally will not violate the ODR because the definitions are identical in both translation units.



### Classes vs structs

They are same but only difference is for struct default visibility is public,
and for classes default visibility is private. All other rules are same

This function inside a class is called a non-static member function.

If the `member functions` definition is included inside the declaration of the class, that function is by default made implicitly `inline`.

#### static member function

Member functions or variables declared static are shared between all instances of an object type. Meaning that only one copy of the member function or variable does exists for any object type.

