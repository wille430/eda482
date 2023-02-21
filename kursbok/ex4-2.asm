
main:
	ldr 	r0,=func1
	blx	r0
	ldr	r0,=func2
	blx	r0
	.thumb_func
func1:
	bx	lr
func2:
	bx	lr
