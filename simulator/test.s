addi x3 x0 x1
loop:
add x2 x1 x3
sub x4 x3 x9
beq x1 x2 loop
lw x1 0(x2)
