.data
base: .word 20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1
.text
    li x1 0
    li x2 0
    li x3 18
    li x17 20
    la x16 base
    loop:
        addi x1 x1 1
        la x15 base
        li x8 0
        loop1:
            blt x3 x8 exit
            lw x5 0(x15)
            lw x6 4(x15)
            addi x15 x15 4
            addi x8 x8 1
            blt x5 x6 loop1
            sw x5 0(x15)
            sw x6 -4(x15)
            j loop1
        exit:
        bne x1 x17 loop