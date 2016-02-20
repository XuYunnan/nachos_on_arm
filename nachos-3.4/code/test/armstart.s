# 1 "armStart.c"
# 1 "<command-line>"
# 1 "armStart.c"
.globl _start
_start:
 bl main
 b Exit


.globl Halt
Halt:


.globl Exit
Exit:
