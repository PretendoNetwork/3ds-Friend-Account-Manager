strcmp: ; 0x1aa78
	str     r11, [sp, #-4]!
	add     r11, sp, #0
	sub     sp, sp, #0xc
	str     r0, [r11, #-8]
	str     r1, [r11, #-0xc]
	b       strcmp_lab_3

strcmp_lab_1: ; 0x1aa90
	ldr     r3, [r11, #-8]
	ldrb    r2, [r3]
	ldr     r3, [r11, #-0xc]
	ldrb    r3, [r3]
	cmp     r2, r3
	beq     strcmp_lab_2
	mov     r3, #0
	b       strcmp_lab_5

strcmp_lab_2: ; 0x1aab0
	ldr     r3, [r11, #-8]
	add     r3, r3, #1
	str     r3, [r11, #-8]
	ldr     r3, [r11, #-0xc]
	add     r3, r3, #1
	str     r3, [r11, #-0xc]

strcmp_lab_3: ; 0x1aac8
	ldr     r3, [r11, #-8]
	ldrb    r3, [r3]
	cmp     r3, #0
	beq     strcmp_lab_4
	ldr     r3, [r11, #-0xc]
	ldrb    r3, [r3]
	cmp     r3, #0
	bne     strcmp_lab_1

strcmp_lab_4: ; 0x1aae8
	ldr     r3, [r11, #-0xc]
	ldrb    r3, [r3]
	cmp     r3, #0
	moveq   r3, #1
	movne   r3, #0
	and     r3, r3, #0xff

strcmp_lab_5: ; 0x1ab00
	mov     r0, r3
	sub     sp, r11, #0
	ldr     r11, [sp], #0x4
	bx      lr
	
strstr: ; 0x1ab10
	push    {r11, lr}
	add     r11, sp, #4
	sub     sp, sp, #8
	str     r0, [r11, #-8]
	str     r1, [r11, #-0xc]
	b       strstr_lab_3

strstr_lab_1: ; 0x1ab28
	ldr     r3, [r11, #-8]
	ldrb    r2, [r3]
	ldr     r3, [r11, #-0xc]
	ldrb    r3, [r3]
	cmp     r2, r3
	bne     strstr_lab_2
	ldr     r1, [r11, #-0xc]
	ldr     r0, [r11, #-8]
	bl      strcmp
	mov     r3, r0
	cmp     r3, #0
	beq     strstr_lab_2
	ldr     r3, [r11, #-8]
	b       strstr_lab_4

strstr_lab_2: ; 0x1ab60
	ldr     r3, [r11, #-8]
	add     r3, r3, #1
	str     r3, [r11, #-8]

strstr_lab_3: ; 0x1ab6c
	ldr     r3, [r11, #-8]
	ldrb    r3, [r3]
	cmp     r3, #0
	bne     strstr_lab_1
	mov     r3, #0

strstr_lab_4: ; 0x1ab80
	mov     r0, r3
	sub     sp, r11, #4
	pop     {r11, lr}
	bx      lr
