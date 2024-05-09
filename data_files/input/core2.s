.data
base: .word 111 11 112 21 131 31 141 41 115 51 116 61 117 71 118 18 119 91 210 110
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
    la x20 base
    loop:
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