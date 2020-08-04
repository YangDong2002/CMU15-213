	.file	"int128.c"
	.text
	.globl	store_uprod
	.type	store_uprod, @function
store_uprod:
.LFB4:
	.cfi_startproc
	endbr64
	movq	%rsi, %rax
	mulq	%rdx
	movq	%rax, (%rdi)
	movq	%rdx, 8(%rdi)
	ret
	.cfi_endproc
.LFE4:
	.size	store_uprod, .-store_uprod
	.globl	add
	.type	add, @function
add:
.LFB5:
	.cfi_startproc
	endbr64
	movq	%rdi, %r8
	movq	%rsi, %rax
	movq	%rax, %rdi
	movq	%rdx, %rax
	movq	%rcx, %rdx
	addq	%r8, %rax
	adcq	%rdi, %rdx
	ret
	.cfi_endproc
.LFE5:
	.size	add, .-add
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
