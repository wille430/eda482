
main:
	ldr	r0,=j
	ldr	r0,[r0]
	ldr	r1,=cvec
	ldrsb	r0,[r1,r0]
	ldr	r1,=i
	ldr	r1,[r1]
	lsl	r1,r1
	ldr	r2,=svec
	strh	r0,[r2,r1]
	b	main

	.align
cvec:	.space	64
svec:	.space	176
i:	.space	4
j:	.space	4