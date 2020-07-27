# assembly language

```assembly
main:
  subq  $8, %rsp  ; push the last frame rbp
  movq  %rbp, (%rsp) ; push the last frame rbp
  movq  %rsp, %rbp  ; update this frame from rsp to rbp
  ; %rbp -> new frame bottom and (%rbp) = last frame rbp
  subq  $16, %rsp ; add stack frame
  movl  $0, -4(%rbp) ;init a = 0 to M[-4 + R(rbp)]
  movl	-4(%rbp), %eax  ; save 0 to %eax or %rax
	movl	%eax, %edi  ; save a to No.1 argv
	call	_Z4testi
  ; push return address and jump to _Z4testi
	movl	%eax, -4(%rbp)
	movl	$0, %eax  ; set %eax to 0, used to return

_Z4testv:
	pushq	%rbp
  ; subq  $8, %rsp  ; push the last frame rbp
  ; movq  %rbp, (%rsp) ; push the last frame rbp
	movq	%rsp, %rbp ; update this frame
	popq	%rbp
  ; movq (%rsp), %rbp ; recover %rbp to the last frame
  ; addq $8, %rsp ; pop stack
	ret
```