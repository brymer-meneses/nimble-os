
# Style Guide

This document contains the C++ coding style used in this repository.

## Guidelines

#### Prefer using trailing return type.

```cpp
// don't do this
i64 function(...);

// do this instead
auto function(...) -> i64;
```

#### Use left-aligned pointers and references

```cpp
// don't do this
u8 *pointer = nullptr;

// do this
u8* pointer = nullptr;
```
This makes it clear that pointers and references are a part of the type of a
variable or a function argument.

