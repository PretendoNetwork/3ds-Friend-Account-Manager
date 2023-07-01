// mount sdmc
.org mount_archives
	b mount_hook

// unmount sdmc
.org unmount_archives
	b unmount_hook
	
// hooks for mounting stuff
.org mount_hooks_address
	mount_hook:
        stmfd      sp!, {lr}
        bl         mount_content_cfa
        ldr        r0, [pc, #0x18]
        ldmfd      sp!, {lr}
        b          mount_sd
    unmount_hook:
        stmfd      sp!, {lr}
        bl         unmount_romfs
        ldr        r0, [pc, #0x4]
        ldmfd      sp!, {lr}
        b          unmount_archive
		.pool
		.byte magic_bytes_two