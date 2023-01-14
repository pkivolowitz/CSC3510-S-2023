		.text
		.global		main

/*	InitRegs()

	Sets all registers to their register number except X30 
	which is being used as the link register.
*/

InitRegs:
		str		x30, [sp, -16]!
		mov		x0, xzr
		mov		x1, 1
		mov		x2, 2
		mov		x3, 3
		mov		x4, 4
		mov		x5, 5
		mov		x6, 6
		mov		x7, 7
		mov		x8, 8
		mov		x9, 9
		mov		x10, 10
		mov		x11, 11
		mov		x12, 12
		mov		x13, 13
		mov		x14, 14
		mov		x15, 15
		mov		x16, 16
		mov		x17, 17
		mov		x18, 18
		mov		x19, 19
		mov		x20, 20
		mov		x21, 21
		mov		x22, 22
		mov		x23, 23
		mov		x24, 24
		mov		x25, 25
		mov		x26, 26
		mov		x27, 27
		mov		x28, 28
		mov		x29, 29
		ldr		x30, [sp], 16
		ret
	
/*	main()

	argc and v are not being used.
*/

main:	
		str		x30, [sp, -16]!

		bl		InitRegs

		mov		x0, 16			// suggest br & inf reg
		bl		malloc

		str		x0, [sp, -16]!	// suggest inf reg
		bl		InitRegs
		ldr		x0, [sp], 16

		mov		x1, x0
		ldr		x0, =s
		bl		printf
		ldr		x30, [sp], 16
		ret

		.data

s:		.asciz		"This is an address 0x%lx\n"

/*	NOTES

printf preserved x18 through x29

*/
		.end



