# The Compiler
This project is a "compiler" or transpiler that can convert human-readable code into functioning Brainfuck code. It does **not** produce the most efficient or compact Brainfuck code, but it should produce a program that runs. The human-readable programming language -- called Brain**Un**fuck -- is a very simple procedural and command based language with syntax for input, output, loops, procedures and basic arithmetic operations. This project is written in C++.

Compiler execution example (Windows):
```
BrainUnfuckCompiler.exe Program.bu Program.bf
```
This will take a BrainUnfuck program named Program.bu and produce a Brainfuck program called Program.bf.

# BrainUnfuck
BrainUnfuck features 2<sup>31</sup> consecutive memory cells, each storing an unsigned 8-bit integer like Brainfuck. Each cell has a unique 4-byte memory address. Input, output, and arithmetic operations on these cells will always be done on a per byte basis, so operations on N-byte integers and other compound data types must be implemented manually.

## List of Commands

**;** - Semicolon starts a comment on the line it appears

**DATA** - Declare variables

**STORE** - Store byte or string in variable(s)

**IN**, **OUT** - Input/output bytes as ASCII chars

**ADD**, **SUB**, **MUL**, **DIV** - Do arithmetic operations

**LT**, **GT**, **LTE**, **GTE**, **EQ**, **NEQ** - Do comparison operations

**NOT**, **OR**, **AND** - Do boolean operations

**ADDRESS** - Store 4-byte address of variable in 4 variables

**COPYVV**, **COPYVM**, **COPYMV** - Copy between static and dynamic memory addresses

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

The larger the memory address of a variable, the more Brainfuck text will be generated each time the variable is used.

## STORE Command
This command is used to store numeric or string literals in variable(s). A numeric literal will be stored in one memory address, whereas a string literal will be stored in as many memory addresses as the ASCII characters require. For string literals, a backslash followed by three decimal digits inserts the ASCII character for the associated code. For example, `"\010"` is a string with one newline character, and `"\034"` is a string with one double quote character; these characters must be escaped, however a single quote may be placed inside of a string literal. Numeric literals must be between 0 and 255.

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
`IN` is used in a manner similar to `OUT` in that it takes a specified number of ASCII characters as input and writes them to the specified memory address(es). Inputting an arbitrary number of characters with one usage of `IN` is not possible and doing this requires placing the command inside a `WHILE` loop.

Example:
```
DATA name[10] message[19] greetings[6]

STORE "What is your name?\010" message
OUT 19 message

; User can input their name (must be exactly 10 characters for program to continue)
IN 10 name

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
DATA q r

DIV 25 11 q r     ; Stores 2 in q and 3 in r
```

## LT / LTE / GT / GTE / EQ / NEQ Commands
These commands mean less than, less than or equal to, greater than, greater than or equal to, equal to, and not equal to, respectively. They all take two arguments, perform the comparison, and store the result in the third argument as either a 0 or a 1.

**Note:** Some comparison commands are implemented with an efficient algorithm that comes with the unfortunate side effect where one argument cannot be 0 or else the program will fail. These commands are:

`LT` - Second argument cannot be 0

`LTE` - First argument cannot be 0

`GT` - First argument cannot be 0

`GTE` - Second argument cannot be 0

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

## NOT / OR / AND Commands
These commands do boolean operations on their arguments and store the result in the last argument. Every boolean operation will interpret an input value >= 1 as being true and 0 as being false.

Example:
```
DATA flag

NOT 0 flag         ; flag = 1 true
NOT 1 flag         ; flag = 0 false
NOT 7 flag         ; flag = 0 false

OR 0 0 flag        ; flag = 0 false
OR 1 0 flag        ; flag = 1 true
OR 0 1 flag        ; flag = 1 true
OR 1 1 flag        ; flag = 2 true

AND 0 0 flag       ; flag = 0 false
AND 1 0 flag       ; flag = 0 false
AND 0 1 flag       ; flag = 0 false
AND 1 1 flag       ; flag = 1 true
AND 99 99 flag     ; flag = 1 true
```

## ADDRESS Command
The `ADDRESS` command stores the 4-byte memory address of a variable into 4 other variables. The first argument is the variable whose address is being stored, and the second, third, fourth, and fifth arguments are the variables that the address is being stored in.

Example:
```
DATA a[20] b
DATA x y z w

ADDRESS   b   x y z w     ; Stores address of b in x, y, z, w variables. x, y, z = 0 and w = 20
```

## COPYVV / COPYVM / COPYMV Commands
`COPYVV` - Copies from a static variable to a static variable. Generally the fastest kind of copy. Static variables that have a larger address will generate more Brainfuck text when compiled.

Example:
```
DATA a b
STORE 5 a

COPYVV a b      ; b now contains 5
```

`COPYVM` - Copies from a static variable to a memory address specified at runtime. Good for accessing the n<sup>th</sup> element of a data structure at runtime. Static variables that have a larger address will generate more Brainfuck text when compiled. Copying to dynamic memory addresses will be considerably slower for larger addresses.

Example:
```
DATA a
DATA x y z w

STORE 7 a
STORE 0 x
STORE 0 y
STORE 1 z
STORE 25 w

; Address x y z w represents 0 * 256^3 + 0 * 256^2 + 1 * 256^1 + 25 * 256^0 = 281
; So 7 is now at memory address 281
COPYVM   a   x y z w

; Can specify address as numeric literals
; Address 5 0 0 0 is 5 * 256^3 = 83886080
; So 7 is now at memory address 83886080
COPYVM   a   5 0 0 0
```

`COPYMV` - Copies from a memory address specified at runtime to a static variable. Good for accessing the n<sup>th</sup> element of a data structure at runtime. Static variables that have a larger address will generate more Brainfuck text when compiled. Copying from dynamic memory addresses will be considerably slower for larger addresses.

Example:
```
DATA a
DATA x y z w

STORE 63 a[65536]

STORE 0 x
STORE 1 y
STORE 0 z
STORE 0 w

; Address 0 1 0 0 represents 0 * 256^3 + 1 * 256^2 + 0 * 256^1 + 0 * 256^0 = 65536
; So 63 is moved to a (address 0) from address 65536
COPYMV   x y z w   a
```

## PROC / ENDPROC / DO / DO_ Commands

`PROC` and `ENDPROC` begin and end a procedure definition. Procedures are a way to reuse the same code. Procedure definitions may **not** be nested. `DO` may be used to execute a procedure. `DO` must be supplied the procedure name and all of its arguments one after another. Alternatively, `DO_` allows a procedure to be called with only one argument which will be subscripted for each of the procedure's arguments. A procedure must never call itself directly or indirectly, as recursion is not allowed.

Example:
```
PROC Cube number result
    MUL number number result
    MUL number result result   ; result is now number^3
ENDPROC

DATA x y
STORE 3 x

DO Cube x y    ; Stores 27 in y

PROC Add4 number1 number2 number3 number4 result
    ADD number1 number2 result
    ADD number3 result result
    ADD number4 result result
ENDPROC

DATA z

STORE "\001\002\003\004" z           ; Store 1-4 in z[0-3]
DO Add4 z[0] z[1] z[2] z[3] z[4]     ; Add z[0-3] together and store in z[4]
DO_ Add4 z                           ; DO_ - Same as previous line
```

**Note:** Be careful when sending variables and literals to procedures using `DO` and `DO_`. Procedures might treat arguments as variables when they are actually numeric or string literals, and vice versa.

## WHILE / ENDWHILE Commands
The `WHILE` command allows looping on a condition and `ENDWHILE` concludes a loop. Loops may be nested indefinitely. `WHILE` takes exactly one argument, which is a variable that will either be 0 or >= 1, indicating whether the loop should continue or not. There is no syntax to break or continue a loop at an arbitrary position.

Example:
```
DATA bool message

STORE "hi\010" message

STORE 1 bool
WHILE bool
    OUT 3 message     ; Prints "hi" forever since bool is always 1
ENDWHILE
```

Example for equivalent Java loop:
```java
int i = 1;
while (i <= 10) {
    System.out.println("hi");
    i++;
}
```
```
DATA i bool message

STORE 1 i
STORE "hi\010" message

LTE i 10 bool
WHILE bool
    OUT 3 message     ; Prints "hi" 10 times
    ADD 1 i i
    LTE i 10 bool 
ENDWHILE
```

**Note:** BrainUnfuck does not provide syntax for if statements. `WHILE` may be used to simulate if statements as long as the condition variable is set to 0 afer the first iteration of the loop.