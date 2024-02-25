.data
arr: .word 4 3 21 1 9 5 6 6 -1
#ninin guug ig
.text
add x5 x0 x0
add x6 x5 x0 
addi x7 x0 9 
addi x8 x0 8 
loop1:
beq x0 x7 exit
addi x7 x7 -1

# hihifninbi
loop2:
addi x1 x1 1
lw x4 0(x5)
lw x9 4(x5)
blt x4 x9 noswap 
sw x4 4(x5)
sw x9 0(x5) #irnfitnigb
noswap:
addi x5 x5 4
bne x1 x8 loop2
add x1 x0 x0
add x5 x6 x0
j loop1
exit:
add x0 x0 x0
add x1 x1 x5
add x1 x1 x8 
