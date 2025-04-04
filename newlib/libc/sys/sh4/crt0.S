.section .bootstrap.start, "ax"
.global	_start
_start:
    ! init for hollyhock is only 16 bytes large
    ! therefore instantly jump to .text
    mov.l   start_address, r0
    jmp     @r0
    nop

! This should be long aligned, however the assembler would add a too large padding
.align 2 
start_address:
.long  actual_start
! load address for hollyhock launcher
load_address:
.long  _start

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
	mov.l	edata_k,r0
	mov.l	end_k,r1
	mov	    #0, r2
start_l:
	mov.l	r2, @r0
	add	    #4, r0
	cmp/ge	r0,r1
	bt	    start_l

    mov.l   cas_setup, r0
    mov     r4,r8
    mov     r5,r9
    jsr     @r0
    mov     r6,r10

    mov.l   ctors_k,r11
    mov.l   ctors_k,r12
    cmp/hs  r12,r11
    bt      init_nl
init_l:
    mov.l   @r11+,r1
    jsr     @r1
    nop
    cmp/hs  r12,r11
    bf      init_l

init_nl:
    mov     r8,r4
    mov     r9,r5
    mov     r10,r6

    ! backup stack pointer and set return address for exit
    mov.l   sp_bak, r0

	! call main
	mov.l	main_k, r1
	jsr	    @r1
    mov.l   r15, @r0

	! call exit
	mov.l   exit_k,r1
	jsr	    @r1
	mov	    r0,r4

.global __exit_address
__exit_address: ! r4: exit status
    mov     r4,r10

    mov.l   dtors_k,r8
    mov.l   dtors_end_k,r9
    cmp/hs  r9,r8
    bt      fini_nl
fini_l:
    mov.l   @r8+,r1
    jsr     @r1
    nop
    cmp/hs  r9,r8
    bf      fini_l
fini_nl:

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
	.long	_edata
end_k:
	.long	_end
main_k:
	.long	_main
exit_k:
	.long	_exit
ctors_k:
    .long   ___ctors
    .weak   ___ctors
ctors_end_k:
    .long   ___ctors_end
    .weak   ___ctors_end
dtors_k:
    .long   ___dtors
    .weak   ___dtors
dtors_end_k:
    .long   ___dtors_end
    .weak   ___dtors_end
sp_bak:
    .long   _sp_bak
cas_setup:
    .long   _cas_setup
cas_cleanup:
    .long   _cas_cleanup
_sp_bak:
    .long   1
