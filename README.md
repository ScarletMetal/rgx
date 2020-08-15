# RGX - Regex Matching For C89

## Introduction

rgx is a small regex matching library, implemented from scratch in C89.


Supports a the regex syntax from python's `re` module

## Build & Install

This project is build using a simple `Makefile`. 

to compile a `rgx.so` file in the project root directory run

```shell
make
```

to compile a `rgx` binary from the test sources in the project directory run 

```shell
make test 
```

## API

### Testing for a string

using the functions provided by `rgx_test.h` you can check if a pattern appears in a given string.

```c
int main(int argc, char *argv) {
    struct rgx_node *pattern  = rgx_compile("\\d+");

    int i = rgx_test_str(pattern, "123");
    if (i) {
        printf("pattern found in provided string\n");
    } else {
        printf("pattern not found in provided string\n");
    }   
}
```

### Finding matches in a string

using the functions provided by `rgx_match.h` you can find all substrings matches by the pattern in a given string.

```c
int main(int argc, char *argv) {
    struct rgx_node *pattern = rgx_compile("\\d+");
    struct rgx_match *match = rgx_match_str(pattern, "123 123 123");
    while (match != NULL) {
        rgx_match_print(match)
        match = match->next;
    }
}
```

## Supported Operators

Currently, a number of basic regex operators are supported.

* `.` Dot, match any character

* `*` Asterisk, match zero or more (greedy) 

* `+` Plus, match one or more (greedy)

* `?` Question Mark, match zero or one

* `\s` Whitespace, match a whitesapce character

* `\w` Alpha, match an alpha-numeric character

* `\d` Digit, match a digit character

* `\S` Non-Whitespace, match a non-whitespace character

* `\W` Non-Alpha, match a non-alpha-numeric character

* `\D` Non-Digit, match a non digit character

* `[abc]` Class, match characters from a given list

* `[a-z]` Class Range, match characters in a given range

* `(...)` Group, match a sub-pattern 

* `(...|...)` Or, match one of the given sub-patterns