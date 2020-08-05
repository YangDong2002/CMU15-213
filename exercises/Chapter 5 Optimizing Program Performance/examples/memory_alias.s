	.file	"memory_alias.c"
	.text
	.p2align 4
	.globl	sum_rows2
	.type	sum_rows2, @function
sum_rows2:
.LFB0:
	.cfi_startproc
	endbr64
	testq	%rdx, %rdx
	jle	.L1
	leaq	0(,%rdx,8), %r8
	negq	%rdx
	leaq	(%rdi,%r8), %rcx
	salq	$3, %rdx
	leaq	(%rsi,%r8), %rdi
	.p2align 4,,10
	.p2align 3
.L3:
	movsd	(%rsi), %xmm0
	leaq	(%rcx,%rdx), %rax
	.p2align 4,,10
	.p2align 3
.L4:
	addsd	(%rax), %xmm0
	addq	$8, %rax
	movsd	%xmm0, (%rsi)
	cmpq	%rax, %rcx
	jne	.L4
	addq	$8, %rsi
	addq	%r8, %rcx
	cmpq	%rdi, %rsi
	jne	.L3
.L1:
	ret
	.cfi_endproc
.LFE0:
	.size	sum_rows2, .-sum_rows2
	.p2align 4
	.globl	sum_rows
	.type	sum_rows, @function
sum_rows:
.LFB1:
	.cfi_startproc
	endbr64
	testq	%rdx, %rdx
	jle	.L7
	leaq	0(,%rdx,8), %r8
	negq	%rdx
	pxor	%xmm1, %xmm1
	leaq	(%rdi,%r8), %rcx
	salq	$3, %rdx
	leaq	(%rsi,%r8), %rdi
	.p2align 4,,10
	.p2align 3
.L9:
	leaq	(%rcx,%rdx), %rax
	movapd	%xmm1, %xmm0
	.p2align 4,,10
	.p2align 3
.L10:
	addsd	(%rax), %xmm0
	addq	$8, %rax
	cmpq	%rcx, %rax
	jne	.L10
	movsd	%xmm0, (%rsi)
	addq	$8, %rsi
	addq	%r8, %rcx
	cmpq	%rdi, %rsi
	jne	.L9
.L7:
	ret
	.cfi_endproc
.LFE1:
	.size	sum_rows, .-sum_rows
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
