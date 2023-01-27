
start:
	ldr	r6,=0x55555555
	ldr	r5,=0x40020c00
	str	r6,[r5]

	ldr	r5,=0x40020c14
	ldr	r6,=0x40021010
main:
	ldrb	r0,[r6]
	ldrb	r1,[r6,#1]
	neg	r0,r0
	strb	r0,[r5]
	b	main
