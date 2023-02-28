
start:	ldr	r0,=0x55555555
	ldr	r1,=0x40020c00
	str	r0,[r1]
	ldr	r5,=0x40020c14
	ldr	r6,=0x40021010

main:	ldrb	r0,[r6]
	ldrb	r1,[r6,#1]
	CMP	r0,r1
	BHI	main_1
	MOV	r0,#0x0
	B	main_2
main_1:	MOV	r0,#0xff
main_2:	STRB	r0,[r5]
	B	main
