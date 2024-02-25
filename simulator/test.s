.data
    arr: .word 40000 3 2100 1 90 5 6000 6 -100 500 300 200 
    str: .string "nibib,bububu"
#ninin guug ig
.text
la x5 arr
add x6 x5 x0 
 addi x7 x0 12
addi x8 x0 11
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
li x31 23
