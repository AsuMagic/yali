# Virtual Machine documentation

## System functions

System functions are a set of standard functions which are implemented as native code within the implementation.

| Name     | Op     | Short description                  |
|----------|--------|------------------------------------|
| `print`  | `0x00` | Print `a`                          |
| `+`      | `0x10` | Pops `a`, `b`, pushes `a + b`      |
| `invsgn` | `0x11` | Pops `a`, pushes `-a`              |
| `*`      | `0x12` | Pops `a`, `b`, pushes `a * b`      |
| `/`      | `0x13` | Pops `a`, `b`, pushes `a / b`      |

## Stack

### Pushing variables

The Virtual Machine uses a stack internally which is used to temporarily store variables and pass parameters and retrieve return values when using functions.

Because e.g. system functions do not necessarily expect a specific type, the stack can hold type information, which is specified using the `typeinfo` instruction, which pushes a 8-bit type information value to the stack. It should be placed after the `pushXX` of the related value.

For example, an `s32` (32-bit signed integer) can be specified as such (`typeinfo 0` being the `typeinfo` for `s32`):

```
push32 424242
typeinfo 0
```

The `typeinfo` is useful for functions to know how large a variable on the stack is or to how deal with the internal representation of a numeric value (e.g. `(+ 5.2 3.5)` vs `(+ 5 3)`).

A list is represented similarily. All of its elements are pushed (from right to left) followed by a `push32` of its element count and a `typeinfo 3`, for example:

```
push32 424242
typeinfo 0
push64 3.141
typeinfo 1
push32 2
typeinfo 2
```

### Typeinfo types

| Name     | Typeinfo |
|----------|----------|
| `s32`    | `0x00`   |
| `f32`    | `0x01`   |
| `list`   | `0x02`   |

### Functions

A function call is performed through several steps:

1. Caller pushes parameters on the stack from right to left.
3. Caller uses `invoke_func`.
4. Callee effectively pops all parameters from the stack (i.e. by reading them).
5. Callee pushes one return parameter to the stack.
6. Caller uses `return`.

### Example

Recursive fibonacci in bytecode:

```
; Fibonacci function, function ID 0
cmp 3
return
```

## Instructions

### Notation

Instructions referring to **both** `a` and `b` as their parameters always refer to the 2nd topmost and the 1st topmost element on the stack, respectively.  
For example, `(* (- 5 3) 2)` could compile into:

```
push32 2
typeinfo 0
push32 -3
typeinfo 0
push32 5
typeinfo 0
invoke_system 0x10 ;add
invoke_system 0x12 ;mul
invoke_system 0x00 ;print
```

In the same way, function parameters are passed from right to left.

```
mark_parameter_scope
typeinfo 2 ;list
push32 2 ;param count
typeinfo 0 ;s32
push32 5
typeinfo 1 ;f64
push32 3.141
invoke_func <placeholder> ;print
```