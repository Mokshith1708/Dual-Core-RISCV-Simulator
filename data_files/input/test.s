.data
arr: .word 1 2 3
.text
la x1 arr
lw x2 0(x1)
add x3 x2 x0