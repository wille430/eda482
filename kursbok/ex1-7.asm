
main:
	ldr	r0,=i
	ldr	r0,[r0]
	lsl	r1,r0
	ldr	r2,=usvec
	ldrh	r0,[r1,r2]
	ldr	r2,=svec
	strh	r0,[r2,#28]
	.align
usvec:	.space	128
svec:	.space	144
i:	.space	4
