	.data
	.align 4
rbss_data:
	.zero 8

	.text
	.globl main
	.type main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	leaq	rbss_data(%rip), %rbx
	subq	$20, %rsp
	movl	$10, %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, 0(%rbx)
	movl	$20, %eax
	movl	%eax, -8(%rbp)
	movl	-8(%rbp), %eax
	movl	%eax, 4(%rbx)
	movl	0(%rbx), %eax
	movl	%eax, -12(%rbp)
	movl	4(%rbx), %eax
	movl	%eax, -16(%rbp)
	movl	-12(%rbp), %eax
	movl	-16(%rbp), %ecx
	imull	%ecx, %eax
	movl	%eax, -20(%rbp)
	movl	-20(%rbp), %eax
	leave
	ret
