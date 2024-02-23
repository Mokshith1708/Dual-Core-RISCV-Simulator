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
addi x1 x1 1
addi x1 x1 1
addi x1 x1 1