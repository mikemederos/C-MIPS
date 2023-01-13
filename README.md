# C-MIPS
Partial implementation of MIPS instructions written in C

## **To use the multiplication algorithm**
(1) compile CMIPS.c\
(2) run ./[name of executable] multiply.txt\
(2) ALWAYS input 1 as first input\
(3) for faster operation, input higher operand first\
(4) input second operand\
(4) Output will be in the form, OUTPUT -> R0 = X		<-- Where X is the result

## Explanation of how the multiplication algorithm works
```assembly
sio1 0 0 5  ;Ask user to input 1, store in RF[0]          PC 0
sw 0 0 0    ;Store input in DM[0]                         PC 4
sio1 0 0 5  ;Ask user for first operand                   PC 8
sw 0 0 1    ;Store input in DM[1]                         PC 12
sio1 0 0 5  ;Ask user for second operand	          PC 16
sw 0 0 2    ;Store input in DM[2]                         PC 20
lw 1 0 0    ;Load DM[0] into RF[1]                        PC 24
lw 3 0 1    ;Load DM[1] into RF[3]                        PC 32
lw 4 0 2    ;Load DM[2] into RF[4]                        PC 36
sub 4 4 1   ;Subtract RF[4]-RF[1], Store in RF[4]         PC 44
add 2 2 3   ;Add RF[2]+RF[3], Store in RF[2]	          PC 40
beqz 4 0 0  ;If 0, end loop                               PC 48
jmp 0 0 36  ;Otherwise, jump to 36	                  PC 52
sub 2 3 4   ;Subtract RF[3]-RF[4], Store in RF[2]         PC 56
sio2 2 0 9  ;Output RF[2]                                 PC 60
sio3 0 0 0  ;Halt
```
