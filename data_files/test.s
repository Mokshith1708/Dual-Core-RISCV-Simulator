.data
srr: .word 1 2 3 4 5 6 7 8 9 10 11
base: .word 0x10000000
str1: .string "initial array : "
w1:.string " "
nl: .string "\nFinal array : "
.text
lw x6 base
li x31 3 #s
li t3 11 #i
la a0 str1
li a7 4