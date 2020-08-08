	.file	"syscall.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Hello world\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB11:
	.cfi_startproc
	endbr64
	pushq	%rax
	.cfi_def_cfa_offset 16
	popq	%rax
	.cfi_def_cfa_offset 8
	movl	$1, %edi
	movl	$13, %edx
	leaq	.LC0(%rip), %rsi
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	write@PLT
	xorl	%edi, %edi
	call	_exit@PLT
	.cfi_endproc
.LFE11:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-10ubuntu2) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
