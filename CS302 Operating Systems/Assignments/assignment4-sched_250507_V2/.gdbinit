set confirm off
source scripts/gef.py
set architecture riscv:rv64
file build/kernel
add-symbol-file build/kernel -o -0xffffffff00000000
#gef config context.show_registers_raw 1
#target extended-remote 127.0.0.1:3333
gef-remote --qemu-user --qemu-binary build/kernel 127.0.0.1 3333
set riscv use-compressed-breakpoints yes
b *0x80200000
#b *0x3ffffff000
#b kerneltrap
