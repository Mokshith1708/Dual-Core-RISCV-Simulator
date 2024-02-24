.data
srr: .word 1 2 3 4 5 6 7 8 9 10 11
base: .word 0x10000000
str1: .string "initial array : "
w1: .string " "
nl: .string "\nFinal array : "
.text
addi x5 x0 10
jalr x3 x5 0
addi x31 x0 1
addi x1 x2 1
add x1 x1 x1
add x2 x1 x1
addi x3 x0 5
addi x21 x0 100
j loop
addi x31 x0 100
loop:
add x7 x7 x31
addi x6 x6 4
addi x3 x3 -1
bne x3 x0 loop
addi x0 x0 0
addi x20 x0 1
addi x15 x0 20
lw x20 0(x15)
sw x29 0(x15)
blt x30 x29 l2
addi x24 x0 5
l2:
addi x25 x0 213
sw x23 1(x0)