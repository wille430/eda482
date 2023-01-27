
main:
	ldr	r0,=i
	ldr	r0,[r0]
	lsl	r0,r0,#2
	ldr	r1,=ivec
	ldr	r0,[r1,r0]
	ldr	r2,=svec
	strh	r0,[r2,#24]
	b	main
	.align
ivec:	.space	256
svec:	.space	224
i:	.space	4
