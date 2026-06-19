.global verify_mesh_block
.text
verify_mesh_block:
    li t0, 0; li t1, 0
loop:
    bge t0, a2, end; ld t2, 0(a0); xor t1, t1, t2; addi a0, a0, 8; addi t0, t0, 1; j loop
end:
    beq t1, a1, sync; li a0, 0; ret
sync:
    li a0, 1; ret
