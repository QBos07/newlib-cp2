.macro DEFINE_OS_FUNC name, addr
.globl _\name
.set _\name, \addr
.endm

DEFINE_OS_FUNC free 0x800A86AC
DEFINE_OS_FUNC malloc 0x800D0AD8
