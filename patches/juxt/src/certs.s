// patch type to 1 (sdmc) instead of 5 (content:)
.org load_cave_pem
	mov r2, #0x1
	
// set certificate location
.org browser_cave_pem_string
	.ascii "3ds/juxt.pem", 0
	
// include the fiddler rootca
.org der_cert_address
	der_cert_start:
		.incbin    "rootca.der"
	der_cert_end:
	
//sizeof max 26 instructions
//r0, r1, r4, r8
// adds root certificate
.org add_default_cert_cave
	add_root_cert:
		ldr     r0, [pc, #0x3C]             // httpC:AddRootCA
		mrc     p15, 0x0, r4, c13, c0, 0x3  // TLS
		ldr     r1, [r5, #0xC]              // load HTTPC handle
		ldr     r8, [r5, #0x14]             // load httpC handle
		str     r0, [r4, #0x80]!            // store cmdhdr in cmdbuf[0]
		str     r1, [r4, #0x4]              // store HTTPC handle in cmdbuf[1]
		mov     r0, r8                      // move httpC handle to r0 for SVC SendSyncRequest
		ldr     r8, [data_one]
		ldr     r1, [data_two]
		str     r8, [r4, #0x8]              // store cert size in cmdbuf[2]
		str     r1, [r4, #0x10]             // store cert bufptr in cmdbuf[4]
		mov     r8, r8, lsl #0x4            // size <<= 4
		orr     r8, r8, #0xA                // size |= 0xA
		str     r8, [r4, #0xC]              // store translate header in cmdbuf[3]
		swi     #0x32                       // finally do the request
		mov     r0, r0                      // do whatever
		b       add_default_cert_cave_end   // jump past the pool
		.byte magic_bytes                   // magical bytes (idk what they do), also, most comments are from an older source code version
		.pool
		mov     r0, r0
		mov     r0, r0
		mov     r0, r0
		// so much mov r0, r0