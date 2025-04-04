if MAY_SUPPLY_SYSCALLS
libc_a_SOURCES += %D%/malloc.c %D%/syscalls.c %D%/impl.c %D%/crt0.S 
endif
