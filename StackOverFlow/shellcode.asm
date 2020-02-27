.section .text
.global _start
_start:
	jmp str
entry_point:
	pop %rcx
	
	mov %rcx,%rbx
	add $7,%rbx
	xor %rax,%rax
	mov %rax,(%rbx)

	xor %edx,%edx
	xor %rsi,%rsi
	mov %rcx,%rdi
	add $59,%rax
	syscall

str:
	call entry_point
	.ascii "/bin/sh"
