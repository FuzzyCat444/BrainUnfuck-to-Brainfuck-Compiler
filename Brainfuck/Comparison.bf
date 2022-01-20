LTE:
0 a b 0 1 0 f
sets flag to 1 if a less than or equal to b
start and end on a

>[-<-[<]>>]
>[ a lteq b
	-
	>>+<<
]
>[ b lt a
	-> do nothing
]
<<<<

Minified:
>[-<-[<]>>]>[->>+<<]>[->]<<<<

LT:
0 b a 0 1 0 f
sets flag to 1 if a less than b
start and end on a

[-<-[<]>>]
>[ b lteq a
	- do nothing
]
>[ a lt b
	->
	>>+<<
]
<<<

Minified:
[-<-[<]>>]>[-]>[->>>+<<]<<<

GTE:
0 b a 0 1 0 f
sets flag to 1 if a greater than or equal to b
start and end on a

[-<-[<]>>]
>[ b lteq a
	- 
	>>+<<
]
>[ a lt b
	-> do nothing
]
<<<

Minified:
[-<-[<]>>]>[->>+<<]>[->]<<<

GT:
0 a b 0 1 0 f
sets flag to 1 if a greater than b
start and end on a

>[-<-[<]>>]
>[ a lteq b
	- do nothing
]
>[ b lt a
	-> 
	>>+<<
]
<<<<

Minified:
>[-<-[<]>>]>[-]>[->>>+<<]<<<<