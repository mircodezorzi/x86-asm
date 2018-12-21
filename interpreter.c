#include<inttypes.h>
#include<stdio.h>

#include "virtual_machine.h"
#include "interpreter.h"
#include "opcodes.h"

/* TODO: add return type for error cheking (?) */
void
interpreter_execute(virtual_machine* vm){

    /*
        MAR: Address in RAM of current instruction
        PC:  Address of next instruction
        IP:  Current instruction
    */

    vm->IP = MEMORY_GET_BYTE(vm->MAR);

    switch(vm->IP){

        case OPCODE_INT_IB:
            interpreter_interrupts(vm,
                MEMORY_GET_BYTE(vm->MAR + 1));
            vm->PC += 2;
            break;

        case OPCODE_LONG_JUMP:
            switch(MEMORY_GET_BYTE(vm->MAR + 1)){
                case OPCODE_JNE_LL:
                    if(!FLAGS_GET(flag_zero))
                        vm->PC += MEMORY_GET_WORD(vm->MAR + 2);
                    else
                        vm->PC += 4;
                    break;
            }

        case OPCODE_JCXZ_SL:
            if(!REGISTER_GET_WORD(CX))
                vm->PC += MEMORY_GET_BYTE(vm->MAR + 1);
            else
                vm->PC += 2;
            break;
        case OPCODE_JECXZ_SL:
            if(!REGISTER_GET_WORD(CX) && MEMORY_GET_BYTE(vm->MAR + 1) == OPCODE_JCXZ_SL)
                vm->PC += MEMORY_GET_BYTE(vm->MAR + 2);
            else
                vm->PC += 3;
            break;
        case OPCODE_JNE_SL:
            if(!FLAGS_GET(flag_zero))
                vm->PC += MEMORY_GET_BYTE(vm->MAR + 1);
            else
                vm->PC += 2;
            break;

        case OPCODE_MOV_AL_IB:
            REGISTER_SET(AL, MEMORY_GET_BYTE(vm->MAR + 1));
            vm->PC += 2;
            break;
        case OPCODE_MOV_AH_IB:
            REGISTER_SET(AH, MEMORY_GET_BYTE(vm->MAR + 1));
            vm->PC += 2;
            break;
        case OPCODE_MOV_AX_IW:
            REGISTER_SET(AX, MEMORY_GET_WORD(vm->MAR + 1));
            vm->PC += 3;
            break;
        case OPCODE_MOV_BL_IB:
            REGISTER_SET(BL, MEMORY_GET_BYTE(vm->MAR + 1));
            vm->PC += 2;
            break;
        case OPCODE_MOV_BH_IB:
            REGISTER_SET(BH, MEMORY_GET_BYTE(vm->MAR + 1));
            vm->PC += 2;
            break;
        case OPCODE_MOV_BX_IW:
            REGISTER_SET(BX, MEMORY_GET_WORD(vm->MAR + 1));
            vm->PC += 3;
            break;
        case OPCODE_MOV_CL_IB:
            REGISTER_SET(CL, MEMORY_GET_BYTE(vm->MAR + 1));
            vm->PC += 2;
            break;
        case OPCODE_MOV_CH_IB:
            REGISTER_SET(CH, MEMORY_GET_BYTE(vm->MAR + 1));
            vm->PC += 2;
            break;
        case OPCODE_MOV_CX_IW:
            REGISTER_SET(CX, MEMORY_GET_WORD(vm->MAR + 1));
            vm->PC += 3;
            break;
        case OPCODE_MOV_DL_IB:
            REGISTER_SET(DL, MEMORY_GET_BYTE(vm->MAR + 1));
            vm->PC += 2;
            break;
        case OPCODE_MOV_DH_IB:
            REGISTER_SET(DH, MEMORY_GET_BYTE(vm->MAR + 1));
            vm->PC += 2;
            break;
        case OPCODE_MOV_DX_IW:
            REGISTER_SET(DX, MEMORY_GET_WORD(vm->MAR + 1));
            vm->PC += 3;
            break;
        case OPCODE_MOV_SP_IW:
            REGISTER_SET(SP, MEMORY_GET_WORD(vm->MAR + 1));
            vm->PC += 3;
            break;
        case OPCODE_MOV_BP_IW:
            REGISTER_SET(BP, MEMORY_GET_WORD(vm->MAR + 1));
            vm->PC += 3;
            break;
        case OPCODE_MOV_SI_IW:
            REGISTER_SET(SI, MEMORY_GET_WORD(vm->MAR + 1));
            vm->PC += 3;
            break;
        case OPCODE_MOV_DI_IW:
            REGISTER_SET(DI, MEMORY_GET_WORD(vm->MAR + 1));
            vm->PC += 3;
            break;

        case OPCODE_ADD_AL_IB:
            REGISTER_SET(AL,
                REGISTER_GET_BYTE(AL)
              + MEMORY_GET_BYTE(vm->MAR + 1)
            );
            vm->PC += 2;
            break;
        case OPCODE_ADD_AX_IW:
            REGISTER_SET(AH,
                REGISTER_GET_WORD(AH)
              + MEMORY_GET_WORD(vm->MAR + 1)
            );
            vm->PC += 3;
            break;
    }

    /* Set address of next instruction */
    vm->MAR = vm->PC;

}

void
interpreter_interrupts(virtual_machine* vm, uint8_t int_code){
    switch(int_code){
        case 0x21:
            interpreter_dos(vm);
            break;
    }
}

void
interpreter_dos(virtual_machine* vm){
    char c;
    uint32_t index;
    switch(REGISTER_GET_BYTE(AH)){
        case 0x01:
            REGISTER_SET(AL, getchar());
            break;

        case 0x02:
            putchar(REGISTER_GET_BYTE(DL));
            break;

        case 0x09:
            index = REGISTER_GET_WORD(DX);
            while(MEMORY_GET_BYTE(index + 1) != '$')
                putchar(MEMORY_GET_BYTE(index++));
            break;

        default:
            fprintf(stderr, "interrupt error: %d", REGISTER_GET_BYTE(AH));
            break;
    }
}
