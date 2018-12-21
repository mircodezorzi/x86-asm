#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

/* many operating systems automatically zero allocated memory */
//#define MEMORY_CLEAR_AT_INIT
#define MEMORY_RANDOMIZE_AT_INIT

typedef struct{
    uint8_t* memory;
    uint32_t memory_size;

    int32_t registers[11];
    int16_t  flags;

    uint32_t MAR;
    uint32_t PC;
    uint8_t  IP;
} virtual_machine;

typedef enum{
    EAX, AX, AH, AL,
    EBX, BX, BH, BL,
    ECX, CX, CH, CL,
    EDX, DX, DH, DL,

    ESI, SI,
    EDI, DI,
    EBP, BP,
    ESP, SP,

    CS, FS,
    DS, GS,
    ES, SS
} registry;

typedef enum{
    flag_carry,
    flag_parity,
    flag_auxiliary,
    flag_zero,
    flag_sign,
    flag_trap,
    flag_interrupt,
    flag_direction,
    flag_overflow
} flags;

/* memory */
#define MEMORY_GET_BYTE(addr)   vm_memory_get_byte(vm, addr)
#define MEMORY_GET_WORD(addr)   vm_memory_get_word(vm, addr)
#define MEMORY_GET_DWORD(addr)  vm_memory_get_dword(vm, addr)

/* registers */
#define REGISTER_SET(reg, val)  vm_register_set(vm, reg, val)
#define REGISTER_GET_BYTE(reg)  (int8_t)vm_register_get(vm, reg)
#define REGISTER_GET_WORD(reg)  (int16_t)vm_register_get(vm, reg)
#define REGISTER_GET_DWORD(reg) (int32_t)vm_register_get(vm, reg)

/* flags */
#define FLAGS_GET(flag)         vm_flags_get(vm, flag)
#define FLAGS_SET(flag, val)    vm_flags_set(vm, flag, val)

virtual_machine* virtual_machine_new(uint32_t);
void    virtual_machine_destroy(virtual_machine*);

int32_t vm_register_get(virtual_machine*, registry);
void    vm_register_set(virtual_machine*, registry, int32_t);

int8_t  vm_memory_get_byte(virtual_machine*, uint32_t);
int16_t vm_memory_get_word(virtual_machine*, uint32_t);
int32_t vm_memory_get_dword(virtual_machine*, uint32_t);
void    vm_memory_set_register(virtual_machine*, registry, uint32_t);
void    vm_memory_print(virtual_machine*, uint32_t, uint32_t);

int16_t vm_flags_get(virtual_machine*, flags);
void    vm_flags_set(virtual_machine*, flags, uint8_t);

#endif /* VIRTUAL_MACHINE_H */
