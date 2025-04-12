.section .bootstrap.start, "ax"
.global start
.type start, @function
start:
    ! init for hollyhock is only 16 bytes large
    ! therefore instantly jump to .text
    mov.l   start_address, r0
    jmp     @r0
    nop

! This should be long aligned, however the assembler would add a too large padding
.align 2 
start_address:
    .long   actual_start
! load address for hollyhock launcher
load_address:
    .long   start

.global _start
.set _start, start
.global __start
.set __start, start

.section .bootstrap.text, "ax"
actual_start: ! r4: argc, r5: argv: r6: envp
    ! backup registers
    mov.l   r13, @-r15
    mov.l   r12, @-r15
    mov.l   r11, @-r15
    mov.l   r10, @-r15
    mov.l   r9, @-r15
    mov.l   r8, @-r15
    sts.l   pr, @-r15

    ! zero out bss
    mov.l   edata_k, r0
    mov.l   end_k, r1
    mov	    #0, r2
start_l:
    mov.l   r2, @r0
    add	    #4, r0
    cmp/ge  r0, r1
    bt	    start_l

    ! set environ
    mov.l   environ_k, r0
    tst     r0, r0
    bt      no_environ
    mov.l   r6, @r0

no_environ:
    mov.l   cas_setup, r0
    mov     r4, r8
    mov     r5, r9
    jsr     @r0
    mov     r6, r10
    mov.l   init_k, r0
    jsr     @r0

    ! backup stack pointer and set return address for exit
    mov.l   sp_bak, r11

    mov     r8, r4
    mov     r9, r5
    mov     r10, r6

    ! call main
    mov.l   main_k, r1
    jsr	    @r1
    mov.l   r15, @r11

    ! call exit
    mov.l   exit_k, r1
    jsr	    @r1
    mov	    r0, r4

.global __exit_address
.type __exit_address, @function
__exit_address: ! r4: exit status
    mov     r4, r10

    mov.l   fini_k, r0
    jsr     @r0

    ! restore stack pointer
    mov.l   sp_bak, r8

    mov.l   cas_cleanup, r0
    jsr     @r0
    mov.l   @r8, r15

    ! restore registers
    mov     r10,r0 ! return status
    lds.l   @r15+, pr
    mov.l   @r15+, r8
    mov.l   @r15+, r9
    mov.l   @r15+, r10
    mov.l   @r15+, r11
    mov.l   @r15+, r12

    rts
    mov.l   @r15+, r13

.align 4
edata_k:
    .long   _edata
end_k:
    .long   _end
main_k:
    .long   _main
exit_k:
    .long   _exit
init_k:
    .long   ___libc_init_array
fini_k:
    .long   ___libc_fini_array
environ_k:
    .long   _environ
    .weak   _environ
sp_bak:
    .long   _sp_bak
cas_setup:
    .long   _cas_setup
cas_cleanup:
    .long   _cas_cleanup
_sp_bak:
    .long   1
