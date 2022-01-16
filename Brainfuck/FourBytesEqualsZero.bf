a z1 x1 b z2 x2 c z3 x3 d z4 x4 ct _
set x4 flag to one if all four bytes are zero
start on a and end on x4

>>+>>>+>>>+>>>+<<<<<<<<<<< set all zero test flags and go back to a
[>]> move to x1 if a is greater than zero else z1
[>>>>>>>>>>+<<<<<<<<<< <] add to count if a greater than zero and end at z1
>[-]> go to b resetting x1
[>]> move to x2 if b is greater than zero else z2
[>>>>>>>+<<<<<<< <] add to count if b greater than zero and end at z2
>[-]> go to c resetting x2
[>]> move to x3 if c is greater than zero else z3
[>>>>+<<<< <] add to count if c greater than zero and end at z3
>[-]> go to d resetting x3
[>]> move to x4 if d is greater than zero else z4
[>+< <] add to count if d greater than zero and end at z4
>>[-<[-]>] move to ct and unset x4 flag if ct is greater than zero ie not all four bytes zero
< end at x4 which is one if the multibyte integer is zero else zero

Minified:
>>+>>>+>>>+>>>+<<<<<<<<<<<[>]>[>>>>>>>>>>+<<<<<<<<<<<]>[-]>[>]>[>>>>>>>+<<<<<<<<]>[-]>[>]>[>>>>+<<<<<]>[-]>[>]>[>+<<]>>[-<[-]>]<
