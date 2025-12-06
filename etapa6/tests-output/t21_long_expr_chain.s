	.text
	.globl main
	.type main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$60, %rsp
	movl	$1, %eax
	movl	%eax, -8(%rbp)
	movl	$2, %eax
	movl	%eax, -12(%rbp)
	movl	-8(%rbp), %eax
	movl	-12(%rbp), %ecx
	addl	%ecx, %eax
	movl	%eax, -16(%rbp)
	movl	$3, %eax
	movl	%eax, -20(%rbp)
	movl	-16(%rbp), %eax
	movl	-20(%rbp), %ecx
	imull	%ecx, %eax
	movl	%eax, -24(%rbp)
	movl	$4, %eax
	movl	%eax, -28(%rbp)
	movl	$2, %eax
	movl	%eax, -32(%rbp)
	movl	-28(%rbp), %eax
	movl	-32(%rbp), %ecx
	cltd
	idivl	%ecx
	movl	%eax, -36(%rbp)
	movl	-24(%rbp), %eax
	movl	-36(%rbp), %ecx
	subl	%ecx, %eax
	movl	%eax, -40(%rbp)
	movl	$5, %eax
	movl	%eax, -44(%rbp)
	movl	$2, %eax
	movl	%eax, -48(%rbp)
	movl	-44(%rbp), %eax
	movl	-48(%rbp), %ecx
	imull	%ecx, %eax
	movl	%eax, -52(%rbp)
	movl	-40(%rbp), %eax
	movl	-52(%rbp), %ecx
	addl	%ecx, %eax
	movl	%eax, -56(%rbp)
	movl	-56(%rbp), %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, -60(%rbp)
	movl	-60(%rbp), %eax
	leave
	ret
