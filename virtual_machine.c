#include<inttypes.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#include "virtual_machine.h"

#ifdef MEMORY_RANDOMIZE_AT_INIT
#include<time.h>
#endif

virtual_machine *
virtual_machine_new(uint32_t memory_size){
    virtual_machine* vm = (virtual_machine*)malloc(sizeof(virtual_machine*));
    vm->memory = (int8_t*)malloc(sizeof(int8_t*) * memory_size * 8);
    vm->memory_size = memory_size;
    #ifdef MEMORY_CLEAR_AT_INIT
    memset(vm->memory, 0, memory_size * 8);
    #elif defined(MEMORY_RANDOMIZE_AT_INIT)
    srand(time(NULL));
    for(uint32_t i = 0; i < memory_size * 8; i++)
        vm->memory[i] = rand() % 255;
    #endif
    memset(vm->registers, 0, 11);
    vm->MAR = 0;
    vm->PC  = 0;
    vm->IP  = 0;
    return vm;
}

void
virtual_machine_destroy(virtual_machine* vm){
    free(vm->memory);
    free(vm);
}

void
vm_memory_print(virtual_machine* vm, uint32_t lower, uint32_t upper){
    for(uint16_t i = lower; upper > 0
            ? i < upper * 8
            : i < 40; i += 8){

        printf("%04x: ", i * 8);

        for(uint8_t j = 0; j < 8; j++)
            printf("%02hhX ", vm->memory[i + j]);

        for(uint8_t j = 0; j < 8; j++)
            printf("%c",
                vm->memory[i + j] > 30 && vm->memory[i + j] < 122
                    ? vm->memory[i + j]
                    : '.');

        printf("\n");
    }
}

int32_t
vm_register_get(virtual_machine* vm, registry reg){
    switch(reg){
        case EAX: case AX: case AL: return vm->registers[0];
        case EBX: case BX: case BL: return vm->registers[1];
        case ECX: case CX: case CL: return vm->registers[2];
        case EDX: case DX: case DL: return vm->registers[3];
        case AH:                    return vm->registers[0] >> 8;
        case BH:                    return vm->registers[1] >> 8;
        case CH:                    return vm->registers[2] >> 8;
        case DH:                    return vm->registers[3] >> 8;
        case ESI: case SI:          return vm->registers[4];
        case EDI: case DI:          return vm->registers[5];
        case EBP: case BP:          return vm->registers[6];
        case ESP: case SP:          return vm->registers[7];
        case CS:                    return vm->registers[8] >> 8;
        case DS:                    return vm->registers[9] >> 8;
        case ES:                    return vm->registers[10]>> 8;
        case FS:                    return vm->registers[8];
        case GS:                    return vm->registers[9];
        case SS:                    return vm->registers[10];
    }
}

#define CB(cas, arg) case cas: arg; break

void
vm_register_set(virtual_machine* vm, registry reg, int32_t value){
    switch(reg){
        CB(EAX, vm->registers[0] = value);
        CB(EBX, vm->registers[1] = value);
        CB(ECX, vm->registers[2] = value);
        CB(EDX, vm->registers[3] = value);
        CB(AX,  vm->registers[0] = (vm->registers[0] & 0xFFFF0000) + value);
        CB(BX,  vm->registers[1] = (vm->registers[1] & 0xFFFF0000) + value);
        CB(CX,  vm->registers[2] = (vm->registers[2] & 0xFFFF0000) + value);
        CB(DX,  vm->registers[3] = (vm->registers[3] & 0xFFFF0000) + value);
        CB(AL,  vm->registers[0] = (vm->registers[0] & 0xFFFFFF00) + value);
        CB(BL,  vm->registers[1] = (vm->registers[1] & 0xFFFFFF00) + value);
        CB(CL,  vm->registers[2] = (vm->registers[2] & 0xFFFFFF00) + value);
        CB(DL,  vm->registers[3] = (vm->registers[3] & 0xFFFFFF00) + value);
        CB(AH,  vm->registers[0] = (vm->registers[0] & 0xFFFF00FF) + (value << 8));
        CB(BH,  vm->registers[1] = (vm->registers[1] & 0xFFFF00FF) + (value << 8));
        CB(CH,  vm->registers[2] = (vm->registers[2] & 0xFFFF00FF) + (value << 8));
        CB(DH,  vm->registers[3] = (vm->registers[3] & 0xFFFF00FF) + (value << 8));
        CB(ESI, vm->registers[4] = value);
        CB(EDI, vm->registers[5] = value);
        CB(EBP, vm->registers[6] = value);
        CB(ESP, vm->registers[7] = value);
        CB(SI,  vm->registers[4] = (vm->registers[4] & 0xFFFF0000) + value);
        CB(DI,  vm->registers[5] = (vm->registers[5] & 0xFFFF0000) + value);
        CB(BP,  vm->registers[6] = (vm->registers[6] & 0xFFFF0000) + value);
        CB(SP,  vm->registers[7] = (vm->registers[7] & 0xFFFF0000) + value);
        CB(FS,  vm->registers[8] = (vm->registers[8] & 0xFFFF0000) + value);
        CB(GS,  vm->registers[9] = (vm->registers[9] & 0xFFFF0000) + value);
        CB(SS,  vm->registers[10]= (vm->registers[10]& 0xFFFF0000) + value);
        CB(CS,  vm->registers[8] = (vm->registers[8] & 0x0000FFFF) + (value << 8));
        CB(DS,  vm->registers[9] = (vm->registers[9] & 0x0000FFFF) + (value << 8));
        CB(ES,  vm->registers[10]= (vm->registers[10]& 0x0000FFFF) + (value << 8));
    }
}

int8_t
vm_memory_get_byte(virtual_machine* vm, uint32_t address){
    return  vm->memory[address];
}

int16_t
vm_memory_get_word(virtual_machine* vm, uint32_t address){
    return  (vm->memory[address    ] << 8)
          +  vm->memory[address + 1];
}

int32_t
vm_memory_get_dword(virtual_machine* vm, uint32_t address){
    return  (vm->memory[address    ] << 24)
          + (vm->memory[address + 1] << 16)
          + (vm->memory[address + 2] << 8)
          +  vm->memory[address + 3];
}

void
vm_memory_set_register(virtual_machine* vm, registry reg, uint32_t address){
    switch(reg){
        case EAX:
            vm->memory[address    ] = vm->registers[0] >> 24;
            vm->memory[address + 1] = vm->registers[0] >> 16;
            vm->memory[address + 2] = vm->registers[0] >> 8;
            vm->memory[address + 3] = vm->registers[0];
            break;
        case AX:
            vm->memory[address    ] = vm->registers[0] >> 8;
            vm->memory[address + 1] = vm->registers[0];
            break;
        case AH:
            vm->memory[address    ] = vm->registers[0] >> 8;
            break;
        case AL:
            vm->memory[address    ] = vm->registers[0];
            break;
        case EBX:
            vm->memory[address    ] = vm->registers[1] >> 24;
            vm->memory[address + 1] = vm->registers[1] >> 16;
            vm->memory[address + 2] = vm->registers[1] >> 8;
            vm->memory[address + 3] = vm->registers[1];
            break;
        case BX:
            vm->memory[address    ] = vm->registers[1] >> 8;
            vm->memory[address + 1] = vm->registers[1];
            break;
        case BH:
            vm->memory[address    ] = vm->registers[1] >> 8;
            break;
        case BL:
            vm->memory[address    ] = vm->registers[1];
            break;
        case ECX:
            vm->memory[address    ] = vm->registers[2] >> 24;
            vm->memory[address + 1] = vm->registers[2] >> 16;
            vm->memory[address + 2] = vm->registers[2] >> 8;
            vm->memory[address + 3] = vm->registers[2];
            break;
        case CX:
            vm->memory[address    ] = vm->registers[2] >> 8;
            vm->memory[address + 1] = vm->registers[2];
            break;
        case CH:
            vm->memory[address    ] = vm->registers[2] >> 8;
            break;
        case CL:
            vm->memory[address    ] = vm->registers[2];
            break;
        case EDX:
            vm->memory[address    ] = vm->registers[3] >> 24;
            vm->memory[address + 1] = vm->registers[3] >> 16;
            vm->memory[address + 2] = vm->registers[3] >> 8;
            vm->memory[address + 3] = vm->registers[3];
            break;
        case DX:
            vm->memory[address    ] = vm->registers[3] >> 8;
            vm->memory[address + 1] = vm->registers[3];
            break;
        case DH:
            vm->memory[address    ] = vm->registers[3] >> 8;
            break;
        case DL:
            vm->memory[address    ] = vm->registers[3];
            break;
        case ESI:
            vm->memory[address    ] = vm->registers[4] >> 24;
            vm->memory[address + 1] = vm->registers[4] >> 16;
            vm->memory[address + 2] = vm->registers[4] >> 8;
            vm->memory[address + 3] = vm->registers[4];
            break;
        case SI:
            vm->memory[address    ] = vm->registers[4] >> 8;
            vm->memory[address + 1] = vm->registers[4];
            break;
        case EDI:
            vm->memory[address    ] = vm->registers[5] >> 24;
            vm->memory[address + 1] = vm->registers[5] >> 16;
            vm->memory[address + 2] = vm->registers[5] >> 8;
            vm->memory[address + 3] = vm->registers[5];
            break;
        case DI:
            vm->memory[address    ] = vm->registers[5] >> 8;
            vm->memory[address + 1] = vm->registers[5];
            break;
        case EBP:
            vm->memory[address    ] = vm->registers[6] >> 24;
            vm->memory[address + 1] = vm->registers[6] >> 16;
            vm->memory[address + 2] = vm->registers[6] >> 8;
            vm->memory[address + 3] = vm->registers[6];
            break;
        case BP:
            vm->memory[address    ] = vm->registers[6] >> 8;
            vm->memory[address + 1] = vm->registers[6];
            break;
        case ESP:
            vm->memory[address    ] = vm->registers[7] >> 24;
            vm->memory[address + 1] = vm->registers[7] >> 16;
            vm->memory[address + 2] = vm->registers[7] >> 8;
            vm->memory[address + 3] = vm->registers[7];
            break;
        case SP:
            vm->memory[address    ] = vm->registers[7] >> 8;
            vm->memory[address + 1] = vm->registers[7];
            break;
        case FS:
            vm->memory[address    ] = vm->registers[8] >> 8;
            vm->memory[address + 1] = vm->registers[8];
            break;
        case GS:
            vm->memory[address    ] = vm->registers[9] >> 8;
            vm->memory[address + 1] = vm->registers[9];
            break;
        case SS:
            vm->memory[address    ] = vm->registers[10]>> 8;
            vm->memory[address + 1] = vm->registers[10];
            break;
        case CS:
            vm->memory[address    ] = vm->registers[8] >> 24;
            vm->memory[address + 1] = vm->registers[8] >> 16;
            break;
        case DS:
            vm->memory[address    ] = vm->registers[9] >> 24;
            vm->memory[address + 1] = vm->registers[9] >> 16;
            break;
        case ES:
            vm->memory[address    ] = vm->registers[10]>> 24;
            vm->memory[address + 1] = vm->registers[10]>> 16;
            break;
    }
}

uint8_t
vm_flags_get(virtual_machine* vm, flags){
    switch(flags){
        case flag_carry:     return vm->flags & 0x0001;
        case flag_parity:    return vm->flags & 0x0002;
        case flag_auxiliary: return vm->flags & 0x0004;
        case flag_zero:      return vm->flags & 0x0008;
        case flag_sign:      return vm->flags & 0x0010;
        case flag_trap:      return vm->flags & 0x0020;
        case flag_interrupt: return vm->flags & 0x0040;
        case flag_direction: return vm->flags & 0x0080;
        case flag_overflow:  return vm->flags & 0x0100;
    }
    return 0;
}

void
vm_flags_set(virtual_machine* vm, flags, uint8_t value){
    switch(flags){
    }
}
