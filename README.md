# The Compiler
This project is a "compiler" or transpiler that can convert human-readable code into functioning Brainfuck code. It does **not** produce the most efficient or compact Brainfuck code, but it should produce a program that runs. The language - called Brain**Un**fuck - is a very simple procedural and command based language with syntax for input, output, loops, procedures and basic arithmetic operations. This project is written in C++.

Compiler execution example (Windows):
```
BrainUnfuckCompiler.exe Program.bu Program.bf
```
This will take a BrainUnfuck program named Program.bu and produce a Brainfuck program called Program.bf.

# BrainUnfuck
BrainUnfuck features 2<sup>31</sup> consecutive memory cells, each storing an unsigned 8-bit integer similar to Brainfuck. Input, output, and arithmetic operations on these cells will always be done on a per byte basis, so operations on n-byte integers and other compound data types must be implemented manually.

## List of Commands

**;** - Semicolon starts a comment on the line it appears

**DATA** - Declare variables

**STORE** - Store byte or string in variable(s)

**IN**, **OUT** - Input/output bytes as ASCII chars

**ADD**, **SUB**, **MUL**, **DIV** - Do arithmetic operations

**LT**, **GT**, **LTE**, **GTE**, **EQ**, **NEQ** - Do comparison operations

**NOT**, **OR**, **AND** - Do boolean operations

**ADDRESS** - Store 4-byte address of variable in 4 variables

**COPYVV**, **COPYMV**, **COPYVM** - Copy between static and dynamic memory addresses

**PROC**, **ENDPROC** - Define procedures (functions)

**DO**, **DO_** - Execute procedures

**WHILE**, **ENDWHILE** - Loop on condition

## DATA Command
This command is used to "declare variables," or more accurately, give names to BrainUnfuck 4-byte memory addresses. This allows a memory address to be referenced statically by name later on in the program. Multiple variables can be declared in one command, or they can be separated into multiple uses of the command. The `DATA` command may appear on any line in the program and will have the same effect. There is no such thing as variable scope and placing `DATA` declarations inside of `PROC` definitions or within `WHILE` loops is not necessary. The name and memory address of a variable declared inside a procedure or loop are still global.

The following code to declare three variables `a`, `b`, and `c`:
```
DATA a b c
```
is equivalent to:
```
DATA a
DATA b
DATA c
```
In the above example, `a` has address 0, `b` has address 1, and `c` has address 2. To offset data memory addresses, the following syntax may be used:
```
DATA a[5] b c
```
Here, variable `a` occupies addresses 0-4, `b` occupies address 5, and `c` occupies address 6. 

Any variable may be referenced in another part of the program using its name or using subscript notation. In this example, both `a` and `a[0]` represent address 0, and only `a[4]` represents address 4. `a[5]` is not out of bounds and represents a valid address 5, which is equivalent to `b` or `b[0]`. A subscript like `c[10]` is also valid and represents the unnamed address 16. Subscripts cannot be negative.

## STORE Command
This command is used to store numeric or string literals. A numeric literal will be stored in one memory address, whereas a string literal will be stored in as many memory addresses as the ASCII characters require. For string literals, a backslash followed by three decimal digits inserts the ASCII character for the associated code. For example, `"\010"` is a string with one newline character, and `"\034"` is a string with one double quote character; these characters must be escaped, however a single quote may be placed inside of a string literal. Numeric literals must be between 0-255.

Example:
```
DATA message[6]
DATA myNumber1
DATA myNumber2

STORE "Hello!" message       ; message[0-5] now contains 'H', 'e', 'l', 'l', 'o', '!'
STORE 7 myNumber1            ; Places 7 at address of myNumber1
STORE "\007" myNumber1       ; Equivalent to previous statement
STORE "\003\008" myNumber1   ; myNumber1 contains 3 and myNumber2 contains 8
STORE 24 myNumber1[1]        ; myNumber2 now contains 24
```

## OUT Command
`OUT` is used to print a specified number of ASCII characters starting at the specified memory address. Printing out an arbitrary number of characters with one usage of `OUT` is not possible and doing this requires placing the command inside a `WHILE` loop.

Example:
```
DATA message[7]
DATA number

STORE "Goodbye" message[0]
STORE 4 number

OUT 7 message         ; Prints "Goodbye"
OUT 4 message[2]      ; Prints "odby"
OUT number message    ; Error, first argument must be a numeric literal!

STORE 10 number
OUT 1 number          ; Prints a newline
```

## IN Command
`IN` is used in a manner similar to `OUT` in that it takes a specified number of ASCII characters as input and writes them to the specified memory address. Inputting an arbitrary number of characters with one usage of `IN` is not possible and doing this requires placing the command inside a `WHILE` loop.

Example:
```
DATA name[10] message[19] greetings[6]

STORE "What is your name?\010" message
OUT 19 message

IN 10 name       ; User can input their name (must be exactly 10 characters for program to continue)

STORE "Hello " greetings

; Prints Hello <name>
OUT 6 greetings
OUT 10 name
```

## ADD Command
Adds two numbers (with overflow) and stores them in a memory address. The first two arguments must be variables or numeric literals and the third argument must be a variable.

Example:
```
DATA a b c

STORE 5 a
STORE 10 b
ADD a b c     ; Stores 15 in c
ADD 2 a c     ; Stores 7 in c
ADD b 7 c     ; Stores 17 in c
ADD 8 3 c     ; Stores 11 in c
ADD a a a     ; Stores 10 in a
```

## SUB Command
Similar to `ADD` but does subtraction. The second argument is subtracted from the first argument and stored in the third argument.

## MUL Command
Similar to `ADD` but does multiplication.

## DIV Command
Does integer division and returns the quotient and remainder.

Example:
```
DATA a b q r

STORE 10 a
STORE 3 b

DIV a b q r     ; Stores 3 in q and 1 in r
```

## LT/LTE/GT/GTE/EQ/NEQ Commands
These commands mean less than, less than or equall to, greater than, greater than or equal to, equal to, and not equal to, respectively. They all take two arguments, perform the comparison, and store the result in the third argument as either a 0 or a 1.

**Note:** Some comparison commands are implemented with an efficient algorithm that comes with an unfortunate side effect where one argument cannot be 0 or else the program will fail. These commands are:

`LT` - Second argument must not be 0

`LTE` - First argument must not be 0

`GT` - First argument must not be 0

`GTE` - Second argument must not be 0

Example:
```
DATA flag
DATA a b

LT 2 3 flag      ; Stores 1 in flag
GTE 7 12 flag    ; Stores 0 in flag

STORE 10 a
STORE 10 b
EQ a b flag      ; Stores 1 in flag

LTE 0 3 flag     ; Runtme error! 0 cannot appear as the first argument of LTE
```