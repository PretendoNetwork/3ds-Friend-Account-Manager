.3ds
.open "code.bin", "build/patched_code.bin", 0x100000

load_cave_pem equ 0x1E3210
browser_cave_pem_string equ 0x1E3468

mount_content_cfa equ 0x1D7CF4
unmount_romfs equ 0x1D8058

mount_archives equ 0x1D7FF8
unmount_archives equ 0x1D80EC

mount_sd equ 0x1B2B20
unmount_archive equ 0x232748

sdmc_string equ 0x1D819C
discovery_string equ 0x15748C

add_default_cert_cave equ 0x176F28
add_default_cert_cave_end equ 0x176F90

mount_hooks_address equ 0x38DC30
der_cert_address equ 0x38DCB0

// set url for miiverse/juxt
.org discovery_string
	.ascii "https://discovery.olv.pretendo.cc/v1/endpoint", 0

.include "src/certs.s"
.include "src/mounting.s"

.close