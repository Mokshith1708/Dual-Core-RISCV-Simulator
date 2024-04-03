.data
base: .word 11 1 12 2 13 3 14 4 15 5 16 6 17 7 18 8 19 9 20 10
.text
start:
    li x1 0
    li x2 0
    li x3 18
    li x19 19
    li x18 20
    la x16 base
    la x17 base
    la x21 base
    loop:
    la x20 base
        addi x2 x1 1
        addi x16 x20 0
        addi x15 x20 4
        loop1:
            beq x18 x2 exit
            lw x5 0(x16)
            lw x6 0(x15)
            blt x5 x6 exit2
            sw x5 0(x15)
            sw x6 0(x16)
            exit2: 
            addi x15 x15 4
            addi x2 x2 1
            j loop1
            
        exit:
        addi x1 x1 1
        addi x20 x20 4
        bne x1 x19 loop