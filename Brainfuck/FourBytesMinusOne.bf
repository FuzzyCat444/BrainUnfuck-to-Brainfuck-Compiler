a z1 x1 b z2 x2 c z3 x3 d t1 t2 t3 cr
subtract one from four digit int abcd
start and end on a

>>>>>>>>>>>+<< Set t2 zero flag to one and go to d
[>]>> Move to t2 if d is zero else move to t3
[->>+<] If at t2 set cr carry flag and end up at t3
<<<- Go to d and subtract one
>>[-] Set zero flag to zero
>> Go to cr
[ If carry needed for c
- Set cr to zero
<<+<<<<< Go to c while setting zero flag
[>]>>>>> Move to t2 if c is zero else t3
[->>+<] Set cr if carry needed for b ending at t3
<[-]<<<<<- Sub one from c while resetting zero flag
>>>>>>> Go back to carry flag
[ If carry needed for b
- Set cr to zero
<<+<<<<<<<< Go to b while setting zero flag
[>]>>>>>>>> Move to t2 if b is zero else t3
[->>+<] Set cr if carry needed for b ending at t3
<[-]<<<<<<<<- Sub one from b while resetting zero flag
>>>>>>>>>> Go back to carry flag
[ If carry needed for a
- Set cr to zero
<<<<<<<<<<<<< Go to a
- Sub one from a
>>>>>>>>>>>>> Go back to empty cr since no carry can be done
]
]
]
<<<<<<<<<<<<< Go back to a

Minified:
>>>>>>>>>>>+<<[>]>>[->>+<]<<<->>[-]>>[-<<+<<<<<[>]>>>>>[->>+<]<[-]<<<<<->>>>>>>[-<<+<<<<<<<<[>]>>>>>>>>[->>+<]<[-]<<<<<<<<->>>>>>>>>>[-<<<<<<<<<<<<<->>>>>>>>>>>>>]]]<<<<<<<<<<<<<