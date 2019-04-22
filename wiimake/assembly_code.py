PRESERVE_REGISTERS_ASSEMBLY = '''
.global backup_reg
backup_reg:
    subi 1,1,128
    stmw 3,12(1)
    stw 0,8(1)
    mfcr 0
    stw 0,4(1)
    mfctr 0
    stw 0,0(1)
    blr

.global restore_reg
restore_reg:
    lwz 0,0(1)
    mtctr 0
    lwz 0,4(1)
    mtcr 0
    lwz 0,8(1)
    lmw 3,12(1)
    addi 1,1,128
    blr
'''

INJECTION_POINT_ASSEMBLY = '''
.global inject_point_{num}
inject_point_{num}:
    b entry_point_{num}
'''

BRANCH_TO_ENTRY_POINT_ASSEMBLY = '''
.global entry_point_{num}
entry_point_{num}:
    subi 1,1,4
    stw  0,4(1)
    mflr 0
    bl backup_reg
    bl {symbol}
    bl restore_reg
    mtlr 0
    lwz 0,4(1)
    addi 1,1,4
    nop
    b inject_point_{num} + 0x04
'''