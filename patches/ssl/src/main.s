.arm.little

.open "code.bin", "build/patched_code.bin",0x100000

.thumb

jmp_addr equ 0x106C72
 
.org 0x106C56
main:
    add r7, r4, #7
    add r7, #0xf9
    sub r2, r7, r4
    add r0, r2, r2
    add r0, #0x1b
    orr r5, r0
    str r5, [r7,#0x28]
    cmp r6, #0
    beq jmp_addr
    mov r0, r4
    add r0, #0x20
    mov r1, r6

.close
