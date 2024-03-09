.data
base: .word 11 1 12 2 13 3 14 4 15 5 16 6 17 7 18 8 19 9 20 10
.text
addi x2 x0 8
addi x3 x2 8
lw x5 0(x3)
addi x3 x3 1
addi x15 x5 100