

start:	LDR	R1,=i
	LDR	R2,=s
	LDR	R0,[R1]
	STRH	R0,[R2]
	B	start

	.ALIGN
i:	.SPACE 	4
s:	.SPACE	2