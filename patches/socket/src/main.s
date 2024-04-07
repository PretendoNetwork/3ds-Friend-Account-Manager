.3ds

.open "code.bin", "build/patched_code.bin", 0x100000

;                  (r0)        (r1)
; int strcmp(char* str1, char* str2)
strcmp equ 0x1011E8

; Relevant registers:
; - r1: input_hostname (we are hooking the copy from r9 to r1)
; - r2: host_size (we are hooking before a memcpy)
; - r5: host_size
; - r9: input_hostname
gethostbyname_hook_addr equ 0x12C660
gethostbyname_hook_end equ 0x12C664

; Relevant registers:
; - r2: node (we are hooking the copy from r2 to r0 with gt condition)
; - r3: node_size
getaddrinfo_hook_addr equ 0x12C3E8
getaddrinfo_hook_end equ 0x12C3EC

; This is where the code cave starts, we have space up to 0x132000
start_replacements_addr equ 0x131088

.org gethostbyname_hook_addr
	b gethostbyname_hook

.org getaddrinfo_hook_addr
	bgt getaddrinfo_hook ; we replicate the original condition in this instruction

.org start_replacements_addr
	gethostbyname_hook: ; 0x131088
		bl    handle_gethostbyname_replacements
		b     gethostbyname_hook_end            ; go back to the original function

	getaddrinfo_hook: ; 0x131090
		bl    handle_getaddrinfo_replacements
		b     getaddrinfo_hook_end            ; go back to the original function

	handle_gethostbyname_replacements:
		stmdb sp!, {r0, r3, r4, r6, r7, r8, r9, r10, r11, r12, lr} ; save the current state just in case
		mov   r0, r9                                               ; move original hostname to r0 for replacements
		bl    handle_replacements
		cmp   r1, #0                                               ; check if the pointer has been modified (a size is returned)
		movgt r2, r1                                               ; if we did, modify the hostname size on r2 (the memcpy used later)
		movgt r5, r1                                               ; and on the original register r5
		mov   r1, r0                                               ; move the returned hostname to r1 (original behavior)
		ldmia sp!, {r0, r3, r4, r6, r7, r8, r9, r10, r11, r12, pc} ; load the original state back and return

	; Note: We do not need to modify the size here because getaddrinfo handles it using strlen
	handle_getaddrinfo_replacements:
		stmdb sp!, {r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, lr} ; save the current state just in case
		mov   r0, r2                                                       ; move original hostname to r0 (original behavior)
		bl    handle_replacements
		ldmia sp!, {r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, pc} ; load the original state back and return

	; Checks if the domain is an NNCS domain. Returns the pointer to a Pretendo domain if it matches, or the original domain
	; (r0)                            (r0)
	; char* handle_replacements(char* original)
	handle_replacements:
		stmdb sp!, {r10, r12, lr}      ; save the current state
		mov   r10, r0                  ; move original hostname to r10 to save it for later
		ldr   r1, =nncs1_orig_name
		bl    strcmp                   ; compare hostname with nncs1
		cmp   r0, #0
		ldreq r0, =nncs1_pretendo_name ; if it matches, return the pretendo domain and its size
		moveq r1, #21                  ; size of pretendo domain
		beq   handle_replacements_end
		mov   r0, r10                  ; move original hostname to r0
		ldr   r1, =nncs2_orig_name
		bl    strcmp                   ; compare hostname with nncs2
		cmp   r0, #0
		ldreq r0, =nncs2_pretendo_name ; if it matches, return the pretendo domain and its size
		moveq r1, #21                  ; size of pretendo domain
		movne r0, r10                  ; if none of the nncs domains match, use the original hostname
		movne r1, #0                   ; size of 0 to represent the domain hasn't been modified

	handle_replacements_end:
		ldmia sp!, {r10, r12, pc}      ; load the original state back and return

; strings
	.pool

	nncs1_orig_name:
		.asciiz "nncs1.app.nintendowifi.net"

	nncs2_orig_name:
		.asciiz "nncs2.app.nintendowifi.net"

	nncs1_pretendo_name:
		.asciiz "nncs1.app.pretendo.cc"

	nncs2_pretendo_name:
		.asciiz "nncs2.app.pretendo.cc"

.close
