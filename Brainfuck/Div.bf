t1 b a t2 t3 t4 b1 b2 q r
divide a by b and store quotient in q and remainder in r
start and end on a

< Go to b
[->>>>>+>+>>+<<<<<<<<] Set b1 b2 r to b
>>>>>[-<<<<<+>>>>>]<<<<< Set b to b1
> Go to a
[ While dividend is not zero
	>>+<< Set t3 cmp flag to 1
	[-<-[<]>>] Compare dividend and divisor
	>
	[- b lteq a so restore divisor
		>>>[-<+<<<<<+>>>>>>] restore divisor
		>+ Increase quotient
		<<[->+<]<< Move b1 back to b2 and go to t3
	]
	>
	[ a lt b do nothing
		->
	]
	<<< Go back to a dividend
]
Sub b from divisor to get remainder
<[->>>>>>>>-<<<<<<<<]
> Go to a

Minified:
<[->>>>>+>+>>+<<<<<<<<]>>>>>[-<<<<<+>>>>>]<<<<<>[>>+<<[-<-[<]>>]>[->>>[-<+<<<<<+>>>>>>]>+<<[->+<]<<]>[->]<<<]<[->>>>>>>>-<<<<<<<<]>