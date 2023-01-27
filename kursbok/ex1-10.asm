

	ldr	r0,=i
	ldr	r0,[r0]
	lsl	r1,r0,#1
	add	r0,r1
	ldr	r1,=j
	ldr	r1,[r1]
	add	r0,r1
	ldr	r1,=m3
	ldrb	r0,[r0,r1]
	ldr	r1,=ch
	strb	r0,[r1]

	.align
i:	.word	2
j:	.word	1
m3:	.byte	1,2,3,4,5,6,7,8,9
ch:	.space	1
