// patch type to 1 (sdmc) instead of 5 (content:)
.org load_cave_pem + 0x14
	mov r2, #0x1
	
// set certificate location
.org browser_cave_pem_string
	.ascii "3ds/juxt-prod.pem", 0
	
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
		ldr     r0, =0x00240082             // httpC:AddRootCA
		mrc     p15, 0x0, r4, c13, c0, 0x3  // TLS
		ldr     r1, [r5, #0xC]              // load HTTPC handle
		ldr     r8, [r5, #0x14]             // load httpC handle
		str     r0, [r4, #0x80]!            // store cmdhdr in cmdbuf[0]
		str     r1, [r4, #4]                // store HTTPC handle in cmdbuf[1]
		mov     r0, r8                      // move httpC handle to r0 for SVC SendSyncRequest
		ldr     r8, =der_cert_end-der_cert_start
		ldr     r1, =der_cert_start
		str     r8, [r4, #8]                // store cert size in cmdbuf[2]
		str     r1, [r4, #16]               // store cert bufptr in cmdbuf[4]
		mov     r8, r8, lsl #4              // size <<= 4
		orr     r8, r8, #0xA                // size |= 0xA
		str     r8, [r4, #12]               // store translate header in cmdbuf[3]
		swi     0x32                        // finally do the request
		nop                                 // do whatever
		b       add_default_cert_cave_end   // jump past the pool
		.pool
		nop
		nop
		nop
		// so much nop