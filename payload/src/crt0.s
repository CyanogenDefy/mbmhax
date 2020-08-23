
.section .text.start
.thumb
.global _start
_start:
	LDR R0, =0x893017FC
	MOV SP, R0

	LDR R0, =__bss_start__
	EOR R1, R1, R1
	LDR R2, =__bss_end__
	SUB R2, R2, R0
	BL memset

	LDR R0, =main
	BX R0
