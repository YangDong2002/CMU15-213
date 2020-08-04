	.file	"nested_array.c"
	.text
	.p2align 4
	.globl	get_pgh_zip
	.type	get_pgh_zip, @function
get_pgh_zip:
.LFB0:
	.cfi_startproc
	endbr64
	leaq	(%rdi,%rdi,4), %rdx
	leaq	pgh(%rip), %rax
	leaq	(%rax,%rdx,4), %rax
	ret
	.cfi_endproc
.LFE0:
	.size	get_pgh_zip, .-get_pgh_zip
	.p2align 4
	.globl	get_pgh_digit
	.type	get_pgh_digit, @function
get_pgh_digit:
.LFB1:
	.cfi_startproc
	endbr64
	movq	%rsi, %r8
	leaq	(%rdi,%rdi,4), %rsi
	leaq	pgh(%rip), %rax
	addq	%r8, %rsi
	movl	(%rax,%rsi,4), %eax
	ret
	.cfi_endproc
.LFE1:
	.size	get_pgh_digit, .-get_pgh_digit
	.p2align 4
	.globl	get_univ_digit
	.type	get_univ_digit, @function
get_univ_digit:
.LFB2:
	.cfi_startproc
	endbr64
	leaq	univ(%rip), %rax
	movq	(%rax,%rdi,8), %rax
	movl	(%rax,%rsi,4), %eax
	ret
	.cfi_endproc
.LFE2:
	.size	get_univ_digit, .-get_univ_digit
	.p2align 4
	.globl	nxn_matrix_access
	.type	nxn_matrix_access, @function
nxn_matrix_access:
.LFB3:
	.cfi_startproc
	endbr64
	movslq	%edx, %rdx
	movslq	%edi, %rdi
	movslq	%ecx, %rcx
	imulq	%rdx, %rdi
	leaq	(%rdi,%rcx), %rax
	movl	(%rsi,%rax,4), %eax
	ret
	.cfi_endproc
.LFE3:
	.size	nxn_matrix_access, .-nxn_matrix_access
	.comm	univ,40,32
	.comm	pgh,4660,32
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
