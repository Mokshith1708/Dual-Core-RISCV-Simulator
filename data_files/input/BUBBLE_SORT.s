.data
arr: .word 1 2 3 4
.text
addi x1 x0 4
addi x2 x1 100
addi x3 x2 100
addi x4 x1 1000
mul x5 x4 x4
add x31 x0 x1
lw x1 0(x0)
add x26 x1 x1