	.text
	.globl main
	.type main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$96, %rsp
	movl	$1, %eax
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %eax
	movl	%eax, -4(%rbp)
	movl	$0, %eax
	movl	%eax, -16(%rbp)
	movl	-16(%rbp), %eax
	movl	%eax, -8(%rbp)
L0:
	movl	-4(%rbp), %eax
	movl	%eax, -20(%rbp)
	movl	$10, %eax
	movl	%eax, -24(%rbp)
	movl	-20(%rbp), %eax
	movl	-24(%rbp), %ecx
	cmpl	%ecx, %eax
	setle	%al
	movzbl	%al, %eax
	movl	%eax, -28(%rbp)
	movl	-28(%rbp), %eax
	cmpl	$0, %eax
	jne	L1
	jmp	L2
L1:
	movl	-4(%rbp), %eax
	movl	%eax, -32(%rbp)
	movl	$2, %eax
	movl	%eax, -36(%rbp)
	movl	-32(%rbp), %eax
	movl	-36(%rbp), %ecx
	cltd
	idivl	%ecx
	movl	%eax, -40(%rbp)
	movl	$2, %eax
	movl	%eax, -44(%rbp)
	movl	-40(%rbp), %eax
	movl	-44(%rbp), %ecx
	imull	%ecx, %eax
	movl	%eax, -48(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, -52(%rbp)
	movl	-48(%rbp), %eax
	movl	-52(%rbp), %ecx
	cmpl	%ecx, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -56(%rbp)
	movl	-56(%rbp), %eax
	cmpl	$0, %eax
	jne	L3
	jmp	L4
L3:
	movl	-4(%rbp), %eax
	movl	%eax, -60(%rbp)
	movl	$3, %eax
	movl	%eax, -64(%rbp)
	movl	-60(%rbp), %eax
	movl	-64(%rbp), %ecx
	cmpl	%ecx, %eax
	setg	%al
	movzbl	%al, %eax
	movl	%eax, -68(%rbp)
	movl	-68(%rbp), %eax
	cmpl	$0, %eax
	jne	L5
	jmp	L6
L5:
	movl	-8(%rbp), %eax
	movl	%eax, -72(%rbp)
	movl	$1, %eax
	movl	%eax, -76(%rbp)
	movl	-72(%rbp), %eax
	movl	-76(%rbp), %ecx
	addl	%ecx, %eax
	movl	%eax, -80(%rbp)
	movl	-80(%rbp), %eax
	movl	%eax, -8(%rbp)
L6:
L7:
L4:
L8:
	movl	-4(%rbp), %eax
	movl	%eax, -84(%rbp)
	movl	$1, %eax
	movl	%eax, -88(%rbp)
	movl	-84(%rbp), %eax
	movl	-88(%rbp), %ecx
	addl	%ecx, %eax
	movl	%eax, -92(%rbp)
	movl	-92(%rbp), %eax
	movl	%eax, -4(%rbp)
	jmp	L0
L2:
	movl	-8(%rbp), %eax
	movl	%eax, -96(%rbp)
	movl	-96(%rbp), %eax
	leave
	ret
