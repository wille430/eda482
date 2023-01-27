
main:
@ R0=j, R1=cvec2[j]
	LDR	R0,#j
	LDR	R0,[R0]
	LDRB	R1,#cvec2
	LDR	R0,[R1,R0]
	LDR	R1,#i
	LDR	R1,[R1]
	LDR	R2,#cvec1
	STRB	R0,[R2,R1]
	.ALIGN
i:	.SPACE	4
j:	.SPACE	4
cvec1:	.SPACE	512
cvec2:	.SPACE	1024
