.data
arr: .word 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1
str: .string "nibib,bububu"
#ninin guug ig
.text
la x5 arr
add x6 x5 x0 
 addi x7 x0 20
addi x8 x0 19
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
li x10 1000
addi x17 x0 1
ecall
la x10 str
addi x17 x0 4
ecall