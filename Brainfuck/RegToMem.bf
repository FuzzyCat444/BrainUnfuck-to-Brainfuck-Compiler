(address 14) (neqz flag 1) (neqz test flag 1) (trail 1) (copy space 2) (value 1) = 20

move a byte from address zero to the specified 4 byte address

assume copy byte and address are already set along with neqz flag at 1 and neqz test flag at 1 and trail at 0
start and end at address 0


>>>>>>>>>>>>>> go to neqz flag
[ while neqz
	<<<<<<<<<<<<<< go to address
	>>+>>>+>>>+>>>+<<<<<<<<<<<[>]>[>>>>>>>>>>+<<<<<<<<<<<]>[-]>[>]>[>>>>>>>+<<<<<<<<]>[-]>[>]>[>>>>+<<<<<]>[-]>[>]>[>+<<]>>[-<[-]>]< check is address zero
	
	[ if x4 is 1 aka address is zero
		- set x4 to 0
		>>>- set neqz flag to 0
		<<< go back to x4
	]
	
	>>> go to neqz
	
	[ if neqz
		<<<<<<<<<<<<<< go back to address
		
		begin move
		
		[->>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<<] move a
		>>>
		[->>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<<] move b
		>>>
		[->>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<<] move c
		>>>
		[->>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<<] move d
		>>>>>
		- set neqz to 0
		>>>
		[->>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<<] move copy space
		>>> go to new address position
		
		end move
		>>>>>>>>>>>+<<[>]>>[->>+<]<<<->>[-]>>[-<<+<<<<<[>]>>>>>[->>+<]<[-]<<<<<->>>>>>>[-<<+<<<<<<<<[>]>>>>>>>>[->>+<]<[-]<<<<<<<<->>>>>>>>>>[-<<<<<<<<<<<<<->>>>>>>>>>>>>]]]<<<<<<<<<<<<< sub 1 from address
		
		>>>>>>>>>>>>>>>+ set neqz test flag to 1
		
		>+ set trail flag
		
		<< go to neqz
	]
	
	> go to neqz test flag
	[ if neqz test flag
		- set test flag to 0
		<+> set neqz flag to 1
	]
	
	< go to neqz flag
]

>>>>> go to value byte
[-] set value byte zero
<< go to copy byte
[->>+<<] move copy byte

< go to trail
[ while trail byte
	- set trail to 0
	<<<<<<<<<<<<<<<<<<<< go to previous memory slot
]

<<<<<<<<<<<<<<<< back to beginning of slot (at address 0!)


Minified:
>>>>>>>>>>>>>>[<<<<<<<<<<<<<<>>+>>>+>>>+>>>+<<<<<<<<<<<[>]>[>>>>>>>>>>+<<<<<<<<<<<]>[-]>[>]>[>>>>>>>+<<<<<<<<]>[-]>[>]>[>>>>+<<<<<]>[-]>[>]>[>+<<]>>[-<[-]>]<[->>>-<<<]>>>[<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<<]>>>[->>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<<]>>>[->>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<<]>>>[->>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<<]>>>>>->>>[->>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<<]>>>>>>>>>>>>>>+<<[>]>>[->>+<]<<<->>[-]>>[-<<+<<<<<[>]>>>>>[->>+<]<[-]<<<<<->>>>>>>[-<<+<<<<<<<<[>]>>>>>>>>[->>+<]<[-]<<<<<<<<->>>>>>>>>>[-<<<<<<<<<<<<<->>>>>>>>>>>>>]]]<<<<<<<<<<<<<>>>>>>>>>>>>>>>+>+<<]>[-<+>]<]>>>>>[-]<<[->>+<<]<[-<<<<<<<<<<<<<<<<<<<<]<<<<<<<<<<<<<<<<