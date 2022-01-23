# The Compiler
This project is a "compiler" or transpiler that can convert human-readable code into functioning Brainfuck code. It does **not** produce the most efficient or compact Brainfuck code, but it should produce a program that runs. The language - called Brain**Un**fuck - is a very simple procedural and command based language with syntax for input, output, loops, procedures and basic arithmetic operations. This project is written in C++.

Compiler execution example (Windows):
```
BrainUnfuckCompiler.exe Program.bu Program.bf
```
This will take a BrainUnfuck program named Program.bu and produce a Brainfuck program called Program.bf.

# BrainUnfuck
BrainUnfuck features 2<sup>31</sup> consecutive memory cells, each storing an unsigned 8-bit integer similar to Brainfuck. Input, output, and arithmetic operations on these cells will always be done on a per byte basis, so operations on n-byte integers and other compound data types must be implemented manually.

## Commands

**DATA** - Declare variables

**PROC**, **ENDPROC** - Define procedures (functions)

**DO**, **DO_** - Execute procedures

**WHILE**, **ENDWHILE** - Loop on condition

**ADD**, **SUB**, **MUL**, **DIV** - Do arithmetic operations

**LT**, **GT**, **LTE**, **GTE**, **EQ**, **NEQ** - Do comparison operations

**NOT**, **OR**, **AND** - Do boolean operations

**STORE** - Store byte or string (multiple bytes) in variable(s)

**IN**, **OUT** - Input/output bytes as ASCII chars

**ADDRESS** - Store 4 byte address of variable in 4 variables

**COPYVV**, **COPYMV**, **COPYVM** - Copy between static and dynamic memory addresses