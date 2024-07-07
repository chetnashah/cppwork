

## TypeTraits and Type Functions allow us to query type parameters in templates

Type traits are a way to query type parameters in templates. They are used to check properties of types at compile time. Type traits are implemented as template classes with a public constant value member that is either true or false. The type traits are defined in the `<type_traits>` header.

### `std::is_pointer<T>::value` - Check if a type is a pointer


`Note`: `std::is_pointer<std::nullptr_t>::value` is false. This is because std::nullptr_t is not considered a pointer type, even though nullptr can be assigned to a pointer variable.