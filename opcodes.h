#ifndef OPCODES_H
#define OPCODES_H

#define OPCODE_ADD_AL_IB 0x04
#define OPCODE_ADD_AX_IW 0x05

#define OPCODE_INT_IB 0xCD

/* All conditional long jumps have 2 byte opcodes, first byte is always 0F */

#define OPCODE_LONG_JUMP 0x0F

#define OPCODE_JCXZ_SL  0xE3
#define OPCODE_JECXZ_SL 0x67

/* Same as JNZ */
#define OPCODE_JNE_LL 0x85
#define OPCODE_JNE_SL 0x75

#define OPCODE_MOV_AL_IB 0xB0
#define OPCODE_MOV_AH_IB 0xB4
#define OPCODE_MOV_AX_IW 0xB8
#define OPCODE_MOV_BL_IB 0xB3
#define OPCODE_MOV_BH_IB 0xB7
#define OPCODE_MOV_BX_IW 0xBB
#define OPCODE_MOV_CL_IB 0xB1
#define OPCODE_MOV_CH_IB 0xB5
#define OPCODE_MOV_CX_IW 0xB9
#define OPCODE_MOV_DL_IB 0xB2
#define OPCODE_MOV_DH_IB 0xB6
#define OPCODE_MOV_DX_IW 0xBA
#define OPCODE_MOV_SP_IW 0xBC
#define OPCODE_MOV_BP_IW 0xBD
#define OPCODE_MOV_SI_IW 0xBE
#define OPCODE_MOV_DI_IW 0xBF

#endif /* OPCODES_H */
