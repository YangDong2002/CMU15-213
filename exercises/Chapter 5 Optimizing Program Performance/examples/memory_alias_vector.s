	.file	"memory_alias_vector.cpp"
	.text
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"cannot create std::vector larger than max_size()"
	.text
	.p2align 4
	.globl	_Z3sumRKSt6vectorIS_IdSaIdEESaIS1_EE
	.type	_Z3sumRKSt6vectorIS_IdSaIdEESaIS1_EE, @function
_Z3sumRKSt6vectorIS_IdSaIdEESaIS1_EE:
.LFB832:
	.cfi_startproc
	endbr64
	movabsq	$1152921504606846975, %rdx
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	movq	8(%rsi), %rax
	subq	(%rsi), %rax
	sarq	$3, %rax
	imull	$-1431655765, %eax, %eax
	cltq
	cmpq	%rdx, %rax
	ja	.L14
	pxor	%xmm0, %xmm0
	movq	$0, 16(%rdi)
	movq	%rdi, %r12
	leaq	0(,%rax,8), %rbx
	movups	%xmm0, (%rdi)
	testq	%rax, %rax
	je	.L1
	movq	%rbx, %rdi
	movq	%rsi, %r13
	call	_Znwm@PLT
	movq	%rbx, %rdx
	xorl	%esi, %esi
	leaq	(%rax,%rbx), %rbp
	movq	%rax, (%r12)
	movq	%rax, %rdi
	movq	%rbp, 16(%r12)
	call	memset@PLT
	movq	%rbp, 8(%r12)
	movq	0(%r13), %rsi
	movq	%rax, %rdx
	.p2align 4,,10
	.p2align 3
.L5:
	movq	(%rsi), %rax
	movsd	(%rdx), %xmm0
	leaq	(%rax,%rbx), %rcx
	.p2align 4,,10
	.p2align 3
.L4:
	addsd	(%rax), %xmm0
	addq	$8, %rax
	movsd	%xmm0, (%rdx)
	cmpq	%rax, %rcx
	jne	.L4
	addq	$8, %rdx
	addq	$24, %rsi
	cmpq	%rdx, %rbp
	jne	.L5
.L1:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	movq	%r12, %rax
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
.L14:
	.cfi_restore_state
	leaq	.LC0(%rip), %rdi
	call	_ZSt20__throw_length_errorPKc@PLT
	.cfi_endproc
.LFE832:
	.size	_Z3sumRKSt6vectorIS_IdSaIdEESaIS1_EE, .-_Z3sumRKSt6vectorIS_IdSaIdEESaIS1_EE
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
