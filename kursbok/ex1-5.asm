

	LDR	R0,=cvec
	LDRB	R0,[R0,#14]
	SXTB	R0,R0
	LDR	R1,=i
	STR	R0,[R1]

	.ALIGN
cvec:	.SPACE	128
i:	.SPACE	4
