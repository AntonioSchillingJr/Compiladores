	.data
	.align 4
rbss_data:
	.zero 4

	.text
	.globl main
	.type main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	leaq	rbss_data(%rip), %rbx
	subq	$32, %rsp
	movl	$0, %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, 0(%rbx)
L0:
	movl	0(%rbx), %eax
	movl	%eax, -8(%rbp)
	movl	$5, %eax
	movl	%eax, -12(%rbp)
	movl	-8(%rbp), %eax
	movl	-12(%rbp), %ecx
	cmpl	%ecx, %eax
	setl	%al
	movzbl	%al, %eax
	movl	%eax, -16(%rbp)
	movl	-16(%rbp), %eax
	cmpl	$0, %eax
	jne	L1
	jmp	L2
L1:
	movl	0(%rbx), %eax
	movl	%eax, -20(%rbp)
	movl	$1, %eax
	movl	%eax, -24(%rbp)
	movl	-20(%rbp), %eax
	movl	-24(%rbp), %ecx
	addl	%ecx, %eax
	movl	%eax, -28(%rbp)
	movl	-28(%rbp), %eax
	movl	%eax, 0(%rbx)
	jmp	L0
L2:
	movl	0(%rbx), %eax
	movl	%eax, -32(%rbp)
	movl	-32(%rbp), %eax
	leave
	ret
