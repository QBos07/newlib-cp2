! file.s with os functions, imported from hollyhock sdk
.macro DEFINE_OS_FUNC name, addr
.globl _\name
.set _\name, \addr
.endm

DEFINE_OS_FUNC cas_close 0x80058412
DEFINE_OS_FUNC cas_findClose 0x8005b3BA
DEFINE_OS_FUNC cas_findNext 0x8005B0F0
DEFINE_OS_FUNC cas_findFirst 0x8005ADAC
DEFINE_OS_FUNC cas_fstat 0x8005848E
DEFINE_OS_FUNC cas_getAddr 0x8005876A
DEFINE_OS_FUNC cas_lseek 0x80058596
DEFINE_OS_FUNC cas_mkdir 0x80058314
DEFINE_OS_FUNC cas_open 0x80058354
DEFINE_OS_FUNC cas_read 0x800583A2
DEFINE_OS_FUNC cas_remove 0x8005844E
DEFINE_OS_FUNC cas_rename 0x80058506
DEFINE_OS_FUNC cas_stat 0x800584c2
DEFINE_OS_FUNC cas_write 0x800583dA
