#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

// many operating systems automatically zero allocated memory
// so there isn't really a point into this option
#define MEMORY_CLEAR_AT_INIT

typedef struct{
    uint8_t* memory;
    uint32_t memory_size;

    int32_t registers[13];
    int16_t flags;
} virtual_machine;

typedef enum{
    // general purpose registers
    EAX, AX, AH, AL,
    EBX, BX, BH, BL,
    ECX, CX, CH, CL,
    EDX, DX, DH, DL,
    // stack registers
    EDS, DS,
    EES, ES,
    EFS, FS,
    EGS, GS,
    ESS, SS,
    ECS, CS,
    EIP, IP,
    // instructions
    MAR,
    PC
} registry;

/**************************************/

#define VM_REGISTER_GET_U8(vm, reg)\
    (uint8_t)vm_register_get(vm, reg)

#define VM_REGISTER_GET_16(vm, reg)\
    (int16_t)vm_register_get(vm, reg)

#define VM_REGISTER_GET_32(vm, reg)\
    (int32_t)vm_register_get(vm, reg)

/**************************************/

/* registers */
#define VM_REGISTER_GET_BYTE(vm, reg)\
    (uint8_t)vm_register_get(vm, reg)

#define VM_REGISTER_GET_WORD(vm, reg)\
    (int16_t)vm_register_get(vm, reg)

#define VM_REGISTER_GET_DWORD(vm, reg)\
    (int32_t)vm_register_get(vm, reg)

virtual_machine* virtual_machine_new(uint32_t);
void    virtual_machine_destroy(virtual_machine*);
int32_t vm_register_get(virtual_machine*, registry);
void    vm_register_set(virtual_machine*, registry, int32_t);
int8_t  vm_memory_get_byte(virtual_machine*, uint32_t);
int16_t vm_memory_get_word(virtual_machine*, uint32_t);
int32_t vm_memory_get_dword(virtual_machine*, uint32_t);
void    vm_memory_set_register(virtual_machine*, registry, uint32_t);
void    vm_memory_print(virtual_machine*, uint32_t, uint32_t);

#endif /* VIRTUAL_MACHINE_H */
