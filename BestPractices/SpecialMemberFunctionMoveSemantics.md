
## There are following special member functions

1. default constructor
2. copy constructor
3. copy assignment operator
4. move constructor
5. move assignment operator
6. destructor

## What is user declared?

Anythin written by the user is user declared. even `=default` is user declared.

## By default both copy and move operations are enabled

Unless there are members that are not copyable or movable, or base class that has members
which are not copyable/movable, both move/copy ops are generated.

## User declared copying makes move operations muted (i.e fallbacks with copying)

If you want to disable both copying and moving, you can mark copying operations as deleted.

## User declared moving makes copying operations deleted

Trying to use copy constructor or assignment operator will result in compilation error.

## User declared destructor mutes move operations (i.e. fallbacks with copying)

## Do not "=delete" move operations

Declaring the special move member functions as deleted is usually not the right way to do it
because it disables the fallback mechanism (copy). Just doing copy ops `=default` is enough.

