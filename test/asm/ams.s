	.file	"main.cpp"
	.text
#APP
	    .text
       .p2align 4,,15
.globl change
change:
       movq 8(%rdi), %rax 
       movq %rax, (%rsp) 
       ret 

#NO_APP
	.section	.text._ZNKSt5ctypeIcE8do_widenEc,"axG",@progbits,_ZNKSt5ctypeIcE8do_widenEc,comdat
	.align 2
	.p2align 4
	.weak	_ZNKSt5ctypeIcE8do_widenEc
	.type	_ZNKSt5ctypeIcE8do_widenEc, @function
_ZNKSt5ctypeIcE8do_widenEc:
.LFB1428:
	.cfi_startproc
	movl	%esi, %eax
	ret
	.cfi_endproc
.LFE1428:
	.size	_ZNKSt5ctypeIcE8do_widenEc, .-_ZNKSt5ctypeIcE8do_widenEc
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"func1"
	.text
	.p2align 4
	.type	_ZL4fun1v, @function
_ZL4fun1v:
.LFB1673:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$5, %edx
	leaq	.LC0(%rip), %rsi
	leaq	_ZSt4cout(%rip), %rdi
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	movq	_ZSt4cout(%rip), %rax
	leaq	_ZSt4cout(%rip), %rdx
	movq	-24(%rax), %rax
	movq	240(%rdx,%rax), %rbp
	testq	%rbp, %rbp
	je	.L9
	cmpb	$0, 56(%rbp)
	je	.L5
	movsbl	67(%rbp), %esi
.L6:
	leaq	_ZSt4cout(%rip), %rdi
	call	_ZNSo3putEc@PLT
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movq	%rax, %rdi
	jmp	_ZNSo5flushEv@PLT
	.p2align 4,,10
	.p2align 3
.L5:
	.cfi_restore_state
	movq	%rbp, %rdi
	call	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
	movq	0(%rbp), %rax
	movl	$10, %esi
	leaq	_ZNKSt5ctypeIcE8do_widenEc(%rip), %rdx
	movq	48(%rax), %rax
	cmpq	%rdx, %rax
	je	.L6
	movq	%rbp, %rdi
	call	*%rax
	movsbl	%al, %esi
	jmp	.L6
.L9:
	call	_ZSt16__throw_bad_castv@PLT
	.cfi_endproc
.LFE1673:
	.size	_ZL4fun1v, .-_ZL4fun1v
	.section	.rodata.str1.1
.LC1:
	.string	"func2"
	.text
	.p2align 4
	.type	_ZL4fun2v, @function
_ZL4fun2v:
.LFB1674:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$5, %edx
	leaq	.LC1(%rip), %rsi
	leaq	_ZSt4cout(%rip), %rdi
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	movq	_ZSt4cout(%rip), %rax
	leaq	_ZSt4cout(%rip), %rdx
	movq	-24(%rax), %rax
	movq	240(%rdx,%rax), %rbp
	testq	%rbp, %rbp
	je	.L16
	cmpb	$0, 56(%rbp)
	je	.L12
	movsbl	67(%rbp), %esi
.L13:
	leaq	_ZSt4cout(%rip), %rdi
	call	_ZNSo3putEc@PLT
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movq	%rax, %rdi
	jmp	_ZNSo5flushEv@PLT
	.p2align 4,,10
	.p2align 3
.L12:
	.cfi_restore_state
	movq	%rbp, %rdi
	call	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
	movq	0(%rbp), %rax
	movl	$10, %esi
	leaq	_ZNKSt5ctypeIcE8do_widenEc(%rip), %rdx
	movq	48(%rax), %rax
	cmpq	%rdx, %rax
	je	.L13
	movq	%rbp, %rdi
	call	*%rax
	movsbl	%al, %esi
	jmp	.L13
.L16:
	call	_ZSt16__throw_bad_castv@PLT
	.cfi_endproc
.LFE1674:
	.size	_ZL4fun2v, .-_ZL4fun2v
	.p2align 4
	.globl	_Z12test_changedv
	.type	_Z12test_changedv, @function
_Z12test_changedv:
.LFB1675:
	.cfi_startproc
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	xorl	%esi, %esi
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	leaq	_ZL4fun1v(%rip), %rax
	movq	%rsp, %rdi
	movq	%rax, (%rsp)
	leaq	_ZL4fun2v(%rip), %rax
	movq	%rax, 8(%rsp)
	call	change@PLT
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L20
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L20:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE1675:
	.size	_Z12test_changedv, .-_Z12test_changedv
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function

main:
.LFB1676:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	_Z12test_changedv
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1676:
	.size	main, .-main
	.p2align 4
	.type	_GLOBAL__sub_I__Z12test_changedv, @function
_GLOBAL__sub_I__Z12test_changedv:
.LFB2177:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	leaq	_ZStL8__ioinit(%rip), %rdi
	call	_ZNSt8ios_base4InitC1Ev@PLT
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	leaq	__dso_handle(%rip), %rdx
	leaq	_ZStL8__ioinit(%rip), %rsi
	jmp	__cxa_atexit@PLT
	.cfi_endproc
.LFE2177:
	.size	_GLOBAL__sub_I__Z12test_changedv, .-_GLOBAL__sub_I__Z12test_changedv
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__Z12test_changedv
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 10.1.0-2ubuntu1~18.04) 10.1.0"
	.section	.note.GNU-stack,"",@progbits
