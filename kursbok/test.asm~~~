start:

mov r0, #239

mov r1, #127

mov r2, #7

add r2, r2, r2

adc r1, r1, r2

orr r2, r2, r0

orr r2, r2, r0

mul r0, r0, r2

mul r2, r2, r1

eor r1, r1, r2

adc r0, r0, r1

eor r0, r0, r1

sub r2, r2, r0

eor r0, r0

eor r0, r1

eor r0, r2

ldr r1, =0x000000FF

and r0, r1

end: b end