	.file	"library.c"
	.section .rdata,"dr"
LC0:
	.ascii " True \0"
LC1:
	.ascii "%s\12\0"
LC2:
	.ascii " False \0"
	.text
.globl _printbool
	.def	_printbool;	.scl	2;	.type	32;	.endef
_printbool:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	cmpl	$1, 8(%ebp)
	jne	L2
	movl	$LC0, 4(%esp)
	movl	$LC1, (%esp)
	call	_printf
	jmp	L1
L2:
	movl	$LC2, 4(%esp)
	movl	$LC1, (%esp)
	call	_printf
L1:
	leave
	ret
.globl _get_float
	.def	_get_float;	.scl	2;	.type	32;	.endef
_get_float:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$40, %esp
	leal	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_gets
	leal	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_atof
	pushl	%eax
	fildl	(%esp)
	leal	4(%esp), %esp
	leave
	ret
.globl _get_int
	.def	_get_int;	.scl	2;	.type	32;	.endef
_get_int:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$40, %esp
	leal	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_gets
	leal	-24(%ebp), %eax
	movl	%eax, (%esp)
	call	_atoi
	pushl	%eax
	fildl	(%esp)
	leal	4(%esp), %esp
	leave
	ret
.globl _addints
	.def	_addints;	.scl	2;	.type	32;	.endef
_addints:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$12, %esp
	leal	12(%ebp), %eax
	movl	%eax, -4(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, -8(%ebp)
	movl	$0, -12(%ebp)
L7:
	cmpl	$0, -8(%ebp)
	je	L8
	movl	-8(%ebp), %edx
	leal	-12(%ebp), %eax
	addl	%edx, (%eax)
	movl	-4(%ebp), %edx
	leal	-4(%ebp), %eax
	addl	$4, (%eax)
	movl	(%edx), %eax
	movl	%eax, -8(%ebp)
	jmp	L7
L8:
	movl	-12(%ebp), %eax
	leave
	ret
.globl _multiplyints
	.def	_multiplyints;	.scl	2;	.type	32;	.endef
_multiplyints:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$12, %esp
	leal	12(%ebp), %eax
	movl	%eax, -4(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, -8(%ebp)
	movl	$1, -12(%ebp)
L10:
	cmpl	$1, -8(%ebp)
	je	L11
	movl	-12(%ebp), %eax
	imull	-8(%ebp), %eax
	movl	%eax, -12(%ebp)
	movl	-4(%ebp), %edx
	leal	-4(%ebp), %eax
	addl	$4, (%eax)
	movl	(%edx), %eax
	movl	%eax, -8(%ebp)
	jmp	L10
L11:
	movl	-12(%ebp), %eax
	leave
	ret
.globl _dupintarray
	.def	_dupintarray;	.scl	2;	.type	32;	.endef
_dupintarray:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$4, %esp
	movl	$0, -8(%ebp)
L13:
	movl	-8(%ebp), %eax
	cmpl	16(%ebp), %eax
	jge	L12
	movl	-8(%ebp), %eax
	leal	0(,%eax,4), %ecx
	movl	12(%ebp), %ebx
	movl	-8(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	8(%ebp), %eax
	movl	(%edx,%eax), %eax
	movl	%eax, (%ecx,%ebx)
	leal	-8(%ebp), %eax
	incl	(%eax)
	jmp	L13
L12:
	addl	$4, %esp
	popl	%ebx
	popl	%ebp
	ret
.globl _dupfloatarray
	.def	_dupfloatarray;	.scl	2;	.type	32;	.endef
_dupfloatarray:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$4, %esp
	movl	$0, -8(%ebp)
L17:
	movl	-8(%ebp), %eax
	cmpl	16(%ebp), %eax
	jge	L16
	movl	-8(%ebp), %eax
	leal	0(,%eax,4), %ecx
	movl	12(%ebp), %ebx
	movl	-8(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	8(%ebp), %eax
	movl	(%edx,%eax), %eax
	movl	%eax, (%ecx,%ebx)
	leal	-8(%ebp), %eax
	incl	(%eax)
	jmp	L17
L16:
	addl	$4, %esp
	popl	%ebx
	popl	%ebp
	ret
.globl _search_index
	.def	_search_index;	.scl	2;	.type	32;	.endef
_search_index:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	$0, -4(%ebp)
L21:
	movl	-4(%ebp), %eax
	cmpl	12(%ebp), %eax
	jge	L22
	movl	-4(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	8(%ebp), %eax
	flds	(%edx,%eax)
	flds	16(%ebp)
	fxch	%st(1)
	fucompp
	fnstsw	%ax
	sahf
	jp	L25
	je	L24
L25:
	jmp	L23
L24:
	movl	-4(%ebp), %eax
	movl	%eax, -8(%ebp)
	jmp	L20
L23:
	leal	-4(%ebp), %eax
	incl	(%eax)
	jmp	L21
L22:
	movl	$-1, -8(%ebp)
L20:
	movl	-8(%ebp), %eax
	leave
	ret
.globl _sortintarray
	.def	_sortintarray;	.scl	2;	.type	32;	.endef
_sortintarray:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$12, %esp
	movl	$0, -12(%ebp)
L27:
	movl	-12(%ebp), %eax
	cmpl	12(%ebp), %eax
	jge	L26
	movl	-12(%ebp), %eax
	incl	%eax
	movl	%eax, -16(%ebp)
L30:
	movl	-16(%ebp), %eax
	cmpl	12(%ebp), %eax
	jge	L29
	movl	-12(%ebp), %eax
	leal	0(,%eax,4), %esi
	movl	8(%ebp), %ebx
	movl	-16(%ebp), %eax
	leal	0(,%eax,4), %ecx
	movl	8(%ebp), %edx
	movl	(%esi,%ebx), %eax
	cmpl	(%ecx,%edx), %eax
	jle	L32
	movl	-12(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	8(%ebp), %eax
	movl	(%edx,%eax), %eax
	movl	%eax, -20(%ebp)
	movl	-12(%ebp), %eax
	leal	0(,%eax,4), %ecx
	movl	8(%ebp), %ebx
	movl	-16(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	8(%ebp), %eax
	movl	(%edx,%eax), %eax
	movl	%eax, (%ecx,%ebx)
	movl	-16(%ebp), %eax
	leal	0(,%eax,4), %ecx
	movl	8(%ebp), %edx
	movl	-20(%ebp), %eax
	movl	%eax, (%ecx,%edx)
L32:
	leal	-16(%ebp), %eax
	incl	(%eax)
	jmp	L30
L29:
	leal	-12(%ebp), %eax
	incl	(%eax)
	jmp	L27
L26:
	addl	$12, %esp
	popl	%ebx
	popl	%esi
	popl	%ebp
	ret
.globl _sortfloatarray
	.def	_sortfloatarray;	.scl	2;	.type	32;	.endef
_sortfloatarray:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$12, %esp
	movl	$0, -8(%ebp)
L35:
	movl	-8(%ebp), %eax
	cmpl	12(%ebp), %eax
	jge	L34
	movl	-8(%ebp), %eax
	incl	%eax
	movl	%eax, -12(%ebp)
L38:
	movl	-12(%ebp), %eax
	cmpl	12(%ebp), %eax
	jge	L37
	movl	-8(%ebp), %eax
	leal	0(,%eax,4), %ebx
	movl	8(%ebp), %ecx
	movl	-12(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	8(%ebp), %eax
	flds	(%ebx,%ecx)
	flds	(%edx,%eax)
	fxch	%st(1)
	fucompp
	fnstsw	%ax
	sahf
	ja	L42
	jmp	L40
L42:
	movl	-8(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	8(%ebp), %eax
	movl	(%edx,%eax), %eax
	movl	%eax, -16(%ebp)
	movl	-8(%ebp), %eax
	leal	0(,%eax,4), %ecx
	movl	8(%ebp), %ebx
	movl	-12(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	8(%ebp), %eax
	movl	(%edx,%eax), %eax
	movl	%eax, (%ecx,%ebx)
	movl	-12(%ebp), %eax
	leal	0(,%eax,4), %ecx
	movl	8(%ebp), %edx
	movl	-16(%ebp), %eax
	movl	%eax, (%ecx,%edx)
L40:
	leal	-12(%ebp), %eax
	incl	(%eax)
	jmp	L38
L37:
	leal	-8(%ebp), %eax
	incl	(%eax)
	jmp	L35
L34:
	addl	$12, %esp
	popl	%ebx
	popl	%ebp
	ret
.globl _factorial
	.def	_factorial;	.scl	2;	.type	32;	.endef
_factorial:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	$0, -4(%ebp)
	cmpl	$0, 8(%ebp)
	jle	L44
	movl	$1, -4(%ebp)
	movl	$1, -8(%ebp)
L45:
	movl	-8(%ebp), %eax
	cmpl	8(%ebp), %eax
	jg	L44
	movl	-4(%ebp), %eax
	imull	-8(%ebp), %eax
	movl	%eax, -4(%ebp)
	leal	-8(%ebp), %eax
	incl	(%eax)
	jmp	L45
L44:
	movl	-4(%ebp), %eax
	leave
	ret
.globl _permutation
	.def	_permutation;	.scl	2;	.type	32;	.endef
_permutation:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$8, %esp
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_factorial
	movl	%eax, %ebx
	movl	12(%ebp), %edx
	movl	8(%ebp), %eax
	subl	%edx, %eax
	movl	%eax, (%esp)
	call	_factorial
	movl	%eax, -8(%ebp)
	movl	%ebx, %eax
	cltd
	idivl	-8(%ebp)
	movl	%eax, -8(%ebp)
	movl	-8(%ebp), %eax
	addl	$8, %esp
	popl	%ebx
	popl	%ebp
	ret
.globl _combination
	.def	_combination;	.scl	2;	.type	32;	.endef
_combination:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$8, %esp
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_factorial
	movl	%eax, %ebx
	movl	12(%ebp), %eax
	movl	%eax, (%esp)
	call	_factorial
	movl	%eax, %esi
	movl	12(%ebp), %edx
	movl	8(%ebp), %eax
	subl	%edx, %eax
	movl	%eax, (%esp)
	call	_factorial
	imull	%esi, %eax
	movl	%eax, -12(%ebp)
	movl	%ebx, %eax
	cltd
	idivl	-12(%ebp)
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	addl	$8, %esp
	popl	%ebx
	popl	%esi
	popl	%ebp
	ret
.globl _ap_nterm
	.def	_ap_nterm;	.scl	2;	.type	32;	.endef
_ap_nterm:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	flds	16(%ebp)
	fld1
	fsubrp	%st, %st(1)
	fmuls	12(%ebp)
	fadds	8(%ebp)
	fstps	-4(%ebp)
	movl	-4(%ebp), %eax
	movl	%eax, -8(%ebp)
	flds	-8(%ebp)
	leave
	ret
	.section .rdata,"dr"
	.align 4
LC9:
	.long	1073741824
	.text
.globl _ap_sumnterms
	.def	_ap_sumnterms;	.scl	2;	.type	32;	.endef
_ap_sumnterms:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	flds	16(%ebp)
	flds	LC9
	fdivrp	%st, %st(1)
	flds	8(%ebp)
	fld	%st(0)
	faddp	%st, %st(1)
	flds	16(%ebp)
	fld1
	fsubrp	%st, %st(1)
	fmuls	12(%ebp)
	faddp	%st, %st(1)
	fmulp	%st, %st(1)
	fstps	-4(%ebp)
	movl	-4(%ebp), %eax
	movl	%eax, -8(%ebp)
	flds	-8(%ebp)
	leave
	ret
.globl _gp_nterm
	.def	_gp_nterm;	.scl	2;	.type	32;	.endef
_gp_nterm:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$40, %esp
	flds	8(%ebp)
	fstpl	-16(%ebp)
	flds	16(%ebp)
	fld1
	fsubrp	%st, %st(1)
	fstpl	8(%esp)
	flds	12(%ebp)
	fstpl	(%esp)
	call	_pow
	fmull	-16(%ebp)
	fstps	-4(%ebp)
	movl	-4(%ebp), %eax
	movl	%eax, -20(%ebp)
	flds	-20(%ebp)
	leave
	ret
	.section .rdata,"dr"
	.align 4
LC15:
	.long	-1082130432
	.align 4
LC17:
	.long	2143289344
	.text
.globl _gp_suminfinity
	.def	_gp_suminfinity;	.scl	2;	.type	32;	.endef
_gp_suminfinity:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	flds	12(%ebp)
	fld1
	fxch	%st(1)
	fucompp
	fnstsw	%ax
	sahf
	ja	L55
	flds	12(%ebp)
	flds	LC15
	fucompp
	fnstsw	%ax
	sahf
	ja	L55
	jmp	L54
L55:
	fldz
	fstps	-4(%ebp)
	jmp	L53
L54:
	flds	12(%ebp)
	fldz
	fxch	%st(1)
	fucompp
	fnstsw	%ax
	sahf
	ja	L57
	jmp	L56
L57:
	flds	8(%ebp)
	fdivs	12(%ebp)
	fld1
	fsubrp	%st, %st(1)
	fstps	-4(%ebp)
	jmp	L53
L56:
	flds	LC17
	fstp	%st(0)
L53:
	flds	-4(%ebp)
	leave
	ret
.globl _init
	.def	_init;	.scl	2;	.type	32;	.endef
_init:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	movl	$0, -4(%ebp)
L59:
	movl	-4(%ebp), %eax
	cmpl	12(%ebp), %eax
	jge	L58
	movl	-4(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ecx
	movl	8(%ebp), %edx
	movl	$0x00000000, %eax
	movl	%eax, (%ecx,%edx)
	movl	-4(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	leal	0(,%eax,4), %edx
	movl	8(%ebp), %eax
	movl	$0, 4(%edx,%eax)
	movl	-4(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	leal	0(,%eax,4), %edx
	movl	8(%ebp), %eax
	movl	$0, 8(%edx,%eax)
	leal	-4(%ebp), %eax
	incl	(%eax)
	jmp	L59
L58:
	leave
	ret
.globl _init2
	.def	_init2;	.scl	2;	.type	32;	.endef
_init2:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	movl	$0, -4(%ebp)
L63:
	movl	-4(%ebp), %eax
	cmpl	12(%ebp), %eax
	jge	L62
	movl	-4(%ebp), %eax
	movl	%eax, %ecx
	sall	$4, %ecx
	movl	8(%ebp), %edx
	movl	$0x00000000, %eax
	movl	%eax, (%ecx,%edx)
	movl	-4(%ebp), %eax
	movl	%eax, %edx
	sall	$4, %edx
	movl	8(%ebp), %eax
	movl	$0, 4(%edx,%eax)
	movl	-4(%ebp), %eax
	movl	%eax, %edx
	sall	$4, %edx
	movl	8(%ebp), %eax
	movl	$0, 8(%edx,%eax)
	movl	-4(%ebp), %eax
	movl	%eax, %edx
	sall	$4, %edx
	movl	8(%ebp), %eax
	movl	$0, 12(%edx,%eax)
	leal	-4(%ebp), %eax
	incl	(%eax)
	jmp	L63
L62:
	leave
	ret
	.section .rdata,"dr"
	.align 4
LC21:
	.ascii "-------------------------------------------\12\0"
	.align 4
LC22:
	.ascii " x         |      f         |      fx     | \12\0"
LC23:
	.ascii "|\0"
	.align 4
LC24:
	.ascii "%5.2f     |   %5d       |    %5d     |\12\0"
	.text
.globl _ShowMeanTable
	.def	_ShowMeanTable;	.scl	2;	.type	32;	.endef
_ShowMeanTable:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$84, %esp
	movl	%esp, -44(%ebp)
	movl	12(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$2, %eax
	addl	$15, %eax
	addl	$15, %eax
	shrl	$4, %eax
	sall	$4, %eax
	movl	%eax, -56(%ebp)
	movl	-56(%ebp), %eax
	call	__alloca
	leal	20(%esp), %eax
	addl	$15, %eax
	shrl	$4, %eax
	sall	$4, %eax
	movl	%eax, -48(%ebp)
	movl	$0, -20(%ebp)
	movl	$0, -24(%ebp)
	movl	$0, -36(%ebp)
	movl	$0x00000000, %eax
	movl	%eax, -32(%ebp)
	movl	%eax, -28(%ebp)
	movl	12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-48(%ebp), %edx
	movl	%edx, (%esp)
	call	_init
	movl	$0, -12(%ebp)
L67:
	movl	-12(%ebp), %eax
	cmpl	12(%ebp), %eax
	jge	L68
	movl	-24(%ebp), %eax
	movl	%eax, -20(%ebp)
	movl	$0, -36(%ebp)
L70:
	cmpl	$0, -20(%ebp)
	js	L71
	movl	-12(%ebp), %eax
	leal	0(,%eax,4), %ecx
	movl	8(%ebp), %ebx
	movl	-20(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$2, %eax
	flds	(%ecx,%ebx)
	movl	-48(%ebp), %ebx
	flds	(%eax,%ebx)
	fxch	%st(1)
	fucompp
	fnstsw	%ax
	sahf
	jp	L87
	je	L73
L87:
	jmp	L72
L73:
	movl	$1, -36(%ebp)
	jmp	L71
L72:
	leal	-20(%ebp), %eax
	decl	(%eax)
	jmp	L70
L71:
	cmpl	$1, -36(%ebp)
	je	L69
	movl	-24(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ecx
	movl	-12(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	8(%ebp), %eax
	movl	(%edx,%eax), %eax
	movl	-48(%ebp), %edx
	movl	%eax, (%ecx,%edx)
	movl	-24(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$2, %eax
	addl	-48(%ebp), %eax
	addl	$4, %eax
	movl	$0, (%eax)
	movl	$0, -16(%ebp)
L75:
	movl	-16(%ebp), %eax
	cmpl	12(%ebp), %eax
	jge	L76
	movl	-24(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ecx
	movl	-16(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	8(%ebp), %eax
	movl	-48(%ebp), %ebx
	flds	(%ecx,%ebx)
	flds	(%edx,%eax)
	fxch	%st(1)
	fucompp
	fnstsw	%ax
	sahf
	jp	L86
	je	L79
L86:
	jmp	L77
L79:
	movl	-24(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$2, %eax
	addl	-48(%ebp), %eax
	addl	$4, %eax
	incl	(%eax)
L77:
	leal	-16(%ebp), %eax
	incl	(%eax)
	jmp	L75
L76:
	leal	-24(%ebp), %eax
	incl	(%eax)
L69:
	leal	-12(%ebp), %eax
	incl	(%eax)
	jmp	L67
L68:
	movl	$0, -12(%ebp)
L80:
	movl	-12(%ebp), %eax
	cmpl	-24(%ebp), %eax
	jge	L81
	movl	-12(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$2, %eax
	addl	-48(%ebp), %eax
	leal	8(%eax), %ebx
	movl	-12(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	leal	0(,%eax,4), %ecx
	movl	-12(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$2, %eax
	addl	-48(%ebp), %eax
	addl	$4, %eax
	fildl	(%eax)
	movl	-48(%ebp), %edx
	fmuls	(%ecx,%edx)
	fnstcw	-38(%ebp)
	movzwl	-38(%ebp), %eax
	orw	$3072, %ax
	movw	%ax, -40(%ebp)
	fldcw	-40(%ebp)
	fistpl	(%ebx)
	fldcw	-38(%ebp)
	movl	-12(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$2, %eax
	addl	-48(%ebp), %eax
	addl	$8, %eax
	fildl	(%eax)
	flds	-28(%ebp)
	faddp	%st, %st(1)
	fstps	-28(%ebp)
	movl	-12(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$2, %eax
	addl	-48(%ebp), %eax
	addl	$4, %eax
	fildl	(%eax)
	flds	-32(%ebp)
	faddp	%st, %st(1)
	fstps	-32(%ebp)
	leal	-12(%ebp), %eax
	incl	(%eax)
	jmp	L80
L81:
	movl	$LC21, (%esp)
	call	_printf
	movl	$LC22, (%esp)
	call	_printf
	movl	$LC21, (%esp)
	call	_printf
	movl	$0, -12(%ebp)
L83:
	movl	-12(%ebp), %eax
	cmpl	-24(%ebp), %eax
	jge	L84
	movl	$LC23, (%esp)
	call	_printf
	movl	-12(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$2, %eax
	addl	-48(%ebp), %eax
	addl	$8, %eax
	movl	(%eax), %eax
	movl	%eax, 16(%esp)
	movl	-12(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$2, %eax
	addl	-48(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	movl	%eax, 12(%esp)
	movl	-12(%ebp), %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$2, %eax
	movl	-48(%ebp), %ebx
	flds	(%eax,%ebx)
	fstpl	4(%esp)
	movl	$LC24, (%esp)
	call	_printf
	leal	-12(%ebp), %eax
	incl	(%eax)
	jmp	L83
L84:
	movl	$LC21, (%esp)
	call	_printf
	flds	-28(%ebp)
	fdivs	-32(%ebp)
	fstps	-8(%ebp)
	movl	-8(%ebp), %eax
	movl	-44(%ebp), %esp
	movl	%eax, -52(%ebp)
	flds	-52(%ebp)
	movl	-4(%ebp), %ebx
	leave
	ret
	.section .rdata,"dr"
	.align 4
LC27:
	.ascii "------------------------------------------------------\12\0"
	.align 4
LC28:
	.ascii " x         |      f         |      fx     |     fx2   |  \12\0"
	.align 4
LC29:
	.ascii "-------------------------------------------------------  \12\0"
	.align 4
LC30:
	.ascii "%5.2f     |   %5d       |    %5d     |    %5d  |\12\0"
	.align 4
LC31:
	.ascii "-------------------------------------------------------\12\0"
	.align 8
LC32:
	.long	0
	.long	1073741824
	.align 8
LC33:
	.long	0
	.long	1071644672
	.text
.globl _ShowStdTable
	.def	_ShowStdTable;	.scl	2;	.type	32;	.endef
_ShowStdTable:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$100, %esp
	movl	%esp, -60(%ebp)
	movl	12(%ebp), %eax
	sall	$4, %eax
	addl	$15, %eax
	addl	$15, %eax
	shrl	$4, %eax
	sall	$4, %eax
	movl	%eax, -80(%ebp)
	movl	-80(%ebp), %eax
	call	__alloca
	leal	24(%esp), %eax
	addl	$15, %eax
	shrl	$4, %eax
	sall	$4, %eax
	movl	%eax, -64(%ebp)
	movl	$0, -32(%ebp)
	movl	$0, -36(%ebp)
	movl	$0, -48(%ebp)
	movl	$0x00000000, %eax
	movl	%eax, -12(%ebp)
	movl	%eax, -44(%ebp)
	movl	%eax, -40(%ebp)
	movl	12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-64(%ebp), %edx
	movl	%edx, (%esp)
	call	_init2
	movl	$0, -24(%ebp)
L89:
	movl	-24(%ebp), %eax
	cmpl	12(%ebp), %eax
	jge	L90
	movl	-36(%ebp), %eax
	movl	%eax, -32(%ebp)
	movl	$0, -48(%ebp)
L92:
	cmpl	$0, -32(%ebp)
	js	L93
	movl	-24(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	8(%ebp), %ecx
	movl	-32(%ebp), %eax
	sall	$4, %eax
	flds	(%edx,%ecx)
	movl	-64(%ebp), %ecx
	flds	(%eax,%ecx)
	fxch	%st(1)
	fucompp
	fnstsw	%ax
	sahf
	jp	L109
	je	L95
L109:
	jmp	L94
L95:
	movl	$1, -48(%ebp)
	jmp	L93
L94:
	leal	-32(%ebp), %eax
	decl	(%eax)
	jmp	L92
L93:
	cmpl	$1, -48(%ebp)
	je	L91
	movl	-36(%ebp), %eax
	movl	%eax, %ecx
	sall	$4, %ecx
	movl	-24(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	8(%ebp), %eax
	movl	(%edx,%eax), %eax
	movl	-64(%ebp), %edx
	movl	%eax, (%ecx,%edx)
	movl	-36(%ebp), %eax
	sall	$4, %eax
	addl	-64(%ebp), %eax
	addl	$4, %eax
	movl	$0, (%eax)
	movl	$0, -28(%ebp)
L97:
	movl	-28(%ebp), %eax
	cmpl	12(%ebp), %eax
	jge	L98
	movl	-36(%ebp), %eax
	movl	%eax, %ecx
	sall	$4, %ecx
	movl	-28(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	8(%ebp), %eax
	movl	-64(%ebp), %ebx
	flds	(%ecx,%ebx)
	flds	(%edx,%eax)
	fxch	%st(1)
	fucompp
	fnstsw	%ax
	sahf
	jp	L108
	je	L101
L108:
	jmp	L99
L101:
	movl	-36(%ebp), %eax
	sall	$4, %eax
	addl	-64(%ebp), %eax
	addl	$4, %eax
	incl	(%eax)
L99:
	leal	-28(%ebp), %eax
	incl	(%eax)
	jmp	L97
L98:
	leal	-36(%ebp), %eax
	incl	(%eax)
L91:
	leal	-24(%ebp), %eax
	incl	(%eax)
	jmp	L89
L90:
	movl	$0, -24(%ebp)
L102:
	movl	-24(%ebp), %eax
	cmpl	-36(%ebp), %eax
	jge	L103
	movl	-24(%ebp), %eax
	sall	$4, %eax
	addl	-64(%ebp), %eax
	leal	8(%eax), %ecx
	movl	-24(%ebp), %eax
	movl	%eax, %edx
	sall	$4, %edx
	movl	-24(%ebp), %eax
	sall	$4, %eax
	addl	-64(%ebp), %eax
	addl	$4, %eax
	fildl	(%eax)
	movl	-64(%ebp), %ebx
	fmuls	(%edx,%ebx)
	fnstcw	-54(%ebp)
	movzwl	-54(%ebp), %eax
	orw	$3072, %ax
	movw	%ax, -56(%ebp)
	fldcw	-56(%ebp)
	fistpl	(%ecx)
	fldcw	-54(%ebp)
	movl	-24(%ebp), %eax
	sall	$4, %eax
	addl	-64(%ebp), %eax
	leal	12(%eax), %edx
	movl	-24(%ebp), %eax
	sall	$4, %eax
	addl	-64(%ebp), %eax
	addl	$8, %eax
	fildl	(%eax)
	movl	-24(%ebp), %eax
	sall	$4, %eax
	movl	-64(%ebp), %ecx
	fmuls	(%eax,%ecx)
	fldcw	-56(%ebp)
	fistpl	(%edx)
	fldcw	-54(%ebp)
	movl	-24(%ebp), %eax
	sall	$4, %eax
	addl	-64(%ebp), %eax
	addl	$8, %eax
	fildl	(%eax)
	flds	-40(%ebp)
	faddp	%st, %st(1)
	fstps	-40(%ebp)
	movl	-24(%ebp), %eax
	sall	$4, %eax
	addl	-64(%ebp), %eax
	addl	$12, %eax
	fildl	(%eax)
	flds	-12(%ebp)
	faddp	%st, %st(1)
	fstps	-12(%ebp)
	movl	-24(%ebp), %eax
	sall	$4, %eax
	addl	-64(%ebp), %eax
	addl	$4, %eax
	fildl	(%eax)
	flds	-44(%ebp)
	faddp	%st, %st(1)
	fstps	-44(%ebp)
	leal	-24(%ebp), %eax
	incl	(%eax)
	jmp	L102
L103:
	movl	$LC27, (%esp)
	call	_printf
	movl	$LC28, (%esp)
	call	_printf
	movl	$LC29, (%esp)
	call	_printf
	movl	$0, -24(%ebp)
L105:
	movl	-24(%ebp), %eax
	cmpl	-36(%ebp), %eax
	jge	L106
	movl	$LC23, (%esp)
	call	_printf
	movl	-24(%ebp), %eax
	sall	$4, %eax
	addl	-64(%ebp), %eax
	addl	$12, %eax
	movl	(%eax), %eax
	movl	%eax, 20(%esp)
	movl	-24(%ebp), %eax
	sall	$4, %eax
	addl	-64(%ebp), %eax
	addl	$8, %eax
	movl	(%eax), %eax
	movl	%eax, 16(%esp)
	movl	-24(%ebp), %eax
	sall	$4, %eax
	addl	-64(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	movl	%eax, 12(%esp)
	movl	-24(%ebp), %eax
	sall	$4, %eax
	movl	-64(%ebp), %ebx
	flds	(%eax,%ebx)
	fstpl	4(%esp)
	movl	$LC30, (%esp)
	call	_printf
	leal	-24(%ebp), %eax
	incl	(%eax)
	jmp	L105
L106:
	movl	$LC31, (%esp)
	call	_printf
	flds	-12(%ebp)
	fdivs	-44(%ebp)
	fstpl	-72(%ebp)
	fldl	LC32
	fstpl	8(%esp)
	flds	-40(%ebp)
	fdivs	-44(%ebp)
	fstpl	(%esp)
	call	_pow
	fsubrl	-72(%ebp)
	fstps	-16(%ebp)
	fldl	LC33
	fstpl	8(%esp)
	flds	-16(%ebp)
	fstpl	(%esp)
	call	_pow
	fstps	-20(%ebp)
	movl	-20(%ebp), %eax
	movl	-60(%ebp), %esp
	movl	%eax, -76(%ebp)
	flds	-76(%ebp)
	movl	-4(%ebp), %ebx
	leave
	ret
	.def	_atoi;	.scl	3;	.type	32;	.endef
	.def	_atof;	.scl	3;	.type	32;	.endef
	.def	_gets;	.scl	3;	.type	32;	.endef
	.def	_printf;	.scl	3;	.type	32;	.endef
	.def	_pow;	.scl	3;	.type	32;	.endef
