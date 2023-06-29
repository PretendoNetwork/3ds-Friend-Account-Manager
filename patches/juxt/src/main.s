.3ds

.open "code.bin", "build/patched_code.bin", 0x0

load_cave_pem equ 0xE3224
browser_cave_pem_string equ 0xE3468

mount_content_cfa equ 0xD7CF4
unmount_romfs equ 0xD8058

mount_archives equ 0xD7FF8
unmount_archives equ 0xD80EC

mount_sd equ 0xB2B20
unmount_archive equ 0x132748

sdmc_string equ 0xD819C
discovery_string equ 0x5748C

add_default_cert_cave equ 0x76F28
add_default_cert_cave_end equ 0x76F90

mount_hooks_address equ 0x28DC30
der_cert_address equ 0x28DCB0

magic_bytes equ 0x82, 0x00, 0x24, 0x00, 0x79, 0x03, 0x00, 0x00, 0xB0, 0xDC, 0x38, 0x00
magic_bytes_two equ 0x9C, 0x81, 0x1D, 0x00

data_one equ 0x76F70
data_two equ 0x76F74

// set url for miiverse/juxt
.org discovery_string
	.ascii "https://discovery.olv.pretendo.cc/v1/endpoint", 0

.include "src/certs.s"
.include "src/mounting.s"

.close