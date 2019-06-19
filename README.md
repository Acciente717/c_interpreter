# c_interpreter
A C language interpreter.

## Setup

Build the interpreter, run

```
cd c_interpreter
make
```

Now we have `cint` at `c_interpreter/`

## Run

Run the command `./cint [input_code]`. The program reads the C code from the file, and output at `stdout`.

## Testing

Run `./auto_test.py` to invoke automatic testing for interpreting execution.

Run `./jit_test.py` to invoke automatic testing for JIT execution.


### Unit Test Cases

#### Variable

| Test Point Description                         | Test Case     |
| ---------------------------------------------- | ------------- |
| Define `long` variable                          | `long_basic`   |
| Initialize `long` variable by `long` literal     | `long_basic`   |
| Initialize `long` variable by `long` variable    | `long_basic`   |
| Assign `long` variable by `long` literal         | `long_basic`   |
| Assign `long` variable by `long` variable        | `long_basic`   |
| Assign `long` variable to itself                | `long_basic`   |
| Define `double` variable                        | `float_basic` |
| Initialize `double` variable by `double` literal | `float_basic` |
| Initialize `double` variable by double` variable | `float_basic` |
| Assign `double` variable by `double` literal     | `float_basic` |
| Assign `double` variable by `double` variable    | `float_basic` |
| Assign double` variable to itself               | `float_basic` |
| Initialize `long` variable by `double` literal   | `float2long`   |
| Initialize `long` variable by `double` variable  | `float2long`   |
| Assign `long` variable by `double` literal       | `float2long`   |
| Assign `long` variable by `double` variable      | `float2long`   |
| Initialize `double` variable by `long` literal   | `long2float`   |
| Initialize `double` variable by `long` variable  | `long2float`   |
| Assign `double` variable by `long` literal       | `long2float`   |
| Assign `double` variable by `long` variable      | `long2float`   |

#### Arithmetic

| Test Point Description                                       | Test Case                |
| ------------------------------------------------------------ | ------------------------ |
| Apply `negate` operator on `long` literal                    | `binary_basic`           |
| Apply `negate` operator on `long` variable                   | `binary_basic`           |
| Apply `not` operator on `long` literal                       | `binary_basic`           |
| Apply `not` operator on `long` variable                      | `binary_basic`           |
| Apply `negate` operator on `double` literal                  | `binary_basic`           |
| Apply `negate` operator on `double` variable                 | `binary_basic`           |
| Apply `not` operator on `double` literal                     | `binary_basic`           |
| Apply `not` operator on `double` variable                    | `binary_basic`           |
| Cascading assign operation                                   | `binary_basic`           |
| Apply `add` operator between `long` literals and variables   | `ternary_arithmic_basic` |
| Apply `sub` operator between `long` literals and variables   | `ternary_arithmic_basic` |
| Apply `mul` operator between `long` literals and variables   | `ternary_arithmic_basic` |
| Apply `div` operator between `long` literals and variables   | `ternary_arithmic_basic` |
| Apply `mod` operator between `long` literals and variables   | `ternary_arithmic_basic` |
| Apply `add` operator between `double` literals and variables | `ternary_arithmic_basic` |
| Apply `sub` operator between `double` literals and variables | `ternary_arithmic_basic` |
| Apply `mul` operator between `double` literals and variables | `ternary_arithmic_basic` |
| Apply `div` operator between `double` literals and variables | `ternary_arithmic_basic` |

#### Control

| Test Point Description                                | Test Case     |
| ----------------------------------------------------- | ------------- |
| Single `if` statement                                 | `if_basic`    |
| `If` and `else` statement                             | `if_basic`    |
| Two consecutive `if` statements                       | `if_basic`    |
| Two consecutive `if` and `else` statements            | `if_basic`    |
| Nested `if` statements                                | `if_basic`    |
| Nested `if` and `else` statements                     | `if_basic`    |
| Chained `if` `else if` `else` statements              | `if_basic`    |
| `Not` operation                                       | `if_basic`    |
| `And` operation                                       | `if_basic`    |
| `Or` operation                                        | `if_basic`    |
| `Not` and `and` operation                             | `if_basic`    |
| `Not` and `or` operation                              | `if_basic`    |
| `Not` and `and` and `or` operation                    | `if_basic`    |
| Single `while` loop                                   | `while_basic` |
| Nested `while` loop                                   | `while_basic` |
| `While` loop with `break`                             | `while_basic` |
| `While` loop with `continue`                          | `while_basic` |
| Nested `while` loop with `break` in the inner loop    | `while_basic` |
| Nested `while` loop with `continue` in the inner loop | `while_basic` |

#### Array

| Test Point Description                                       | Test Case     |
| ------------------------------------------------------------ | ------------- |
| One dimensional array                                        | `array_basic` |
| Multi-dimensional array                                      | `array_basic` |
| Passing one-dimensional array without specific length as arguments | `array_basic` |
| Passing one-dimensional array with specific length as arguments | `array_basic` |
| Passing multi-dimensional array with specific length as arguments | `array_basic` |
| Modify array element by functions                            | `array_basic` |

#### Struct

| Test Point Description                     | Test Case      |
| ------------------------------------------ | -------------- |
| Struct containing a single builtin type    | `struct_basic` |
| Struct containing an array                 | `struct_basic` |
| Struct forming an array                    | `struct_basic` |
| An array of structs which contain an array | `struct_basic` |
| Nested struct                              | `struct_basic` |
| Assigning a struct                         | `struct_basic` |
| Returning a struct from a function         | `struct_basic` |
| Passing a struct as an argument            | `struct_basic` |

#### Function

| Test Point Description                                       | Test Case        |
| ------------------------------------------------------------ | ---------------- |
| Function of returning type `void`                            | `function_basic` |
| Function of returning type other than `void`                 | `function_basic` |
| Function with no parameter                                   | `function_basic` |
| Function modifying non-array argument will not affect the original variable | `function_basic` |
| Function modifying array argument will affect the original array | `function_basic` |
| Function with `return` argument in the middle of the body    | `function_basic` |
| Passing function return value as function arguments          | `function_basic` |
| Return a function return value                               | `function_basic` |
| Recursive function                                           | `function_basic` |



### Additional Test Cases

#### print_long

- Invoke function.
- Pass literal as function argument.
- Initialize `long` variable by literal.

#### primes

- Output the first 1000 prime numbers beginning with 11.