#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<inttypes.h>
#include<string.h>
#include<assert.h>

#define MEMORY_INIT_0

typedef struct{
    uint8_t* memory;
    uint32_t memory_size;

    int32_t registers[8];
    int16_t flags;
} machine_t;

typedef enum{
    EAX, AX, AH, AL,
    EBX, BX, BH, BL,
    ECX, CX, CH, CL,
    EDX, DX, DH, DL,
    ESI, SI,
    EDI, DI,
    ESP, SP,
    EBP, BP
} registry;

machine_t* machine_new(uint32_t memory_size){
    machine_t* m = (machine_t*)malloc(sizeof(machine_t*));
    assert(memory_size < (uint32_t)(-1));
    m->memory = (int8_t*)malloc(sizeof(int8_t*) * memory_size * 8);
    m->memory_size = memory_size;
    #ifdef MEMORY_INIT_0
    memset(m->memory, 0, memory_size * 8);
    #endif
    return m;
}

#define MACHINE_REGISTER_GET_U8(reg)\
    (uint8_t)machine_register_get(m ,reg)

#define MACHINE_REGISTER_GET_16(reg)\
    (int16_t)machine_register_get(m, reg)

#define MACHINE_REGISTER_GET_32(reg)\
    (int32_t)machine_register_get(m, reg)

int32_t machine_register_get(machine_t* m, registry reg){
    switch(reg){
        case EAX: case AX: case AL: return m->registers[0];
        case EBX: case BX: case BL: return m->registers[1];
        case ECX: case CX: case CL: return m->registers[2];
        case EDX: case DX: case DL: return m->registers[3];
        case ESI: case SI: m->registers[4];
        case EDI: case DI: m->registers[5];
        case ESP: case SP: m->registers[6];
        case EBP: case BP: m->registers[7];
        case AH: return m->registers[0] >> 8;
        case BH: return m->registers[1] >> 8;
        case CH: return m->registers[2] >> 8;
        case DH: return m->registers[3] >> 8;
    }
}

#define MACHINE_REGISTER_SET(registry, value)\
    machine_register_set(m, registry, (int32_t)value)

void machine_register_set(machine_t* m, registry reg, int32_t value){
    switch(reg){
        case EAX: m->registers[0] = value;                                   break;
        case AX:  m->registers[0] = (m->registers[0] & 0xFF00) + value;      break;
        case AH:  m->registers[0] = (m->registers[0] & 0xFFF0) + value;      break;
        case AL:  m->registers[0] = (m->registers[0] & 0xFF0F) + value << 8; break;
        case EBX: m->registers[1] = value;                                   break;
        case BX:  m->registers[1] = (m->registers[1] & 0xFF00) + value;      break;
        case BH:  m->registers[1] = (m->registers[1] & 0xFFF0) + value;      break;
        case BL:  m->registers[1] = (m->registers[1] & 0xFF0F) + value << 8; break;
        case ECX: m->registers[2] = value;                                   break;
        case CX:  m->registers[2] = (m->registers[2] & 0xFF00) + value;      break;
        case CH:  m->registers[2] = (m->registers[2] & 0xFFF0) + value;      break;
        case CL:  m->registers[2] = (m->registers[2] & 0xFF0F) + value << 8; break;
        case EDX: m->registers[3] = value;                                   break;
        case DX:  m->registers[3] = (m->registers[3] & 0xFF00) + value;      break;
        case DH:  m->registers[3] = (m->registers[3] & 0xFFF0) + value;      break;
        case DL:  m->registers[3] = (m->registers[3] & 0xFF0F) + value << 8; break;
        case ESI: m->registers[4] = value;                                   break;
        case SI:  m->registers[4] = (m->registers[4] & 0xFF00) + value;      break;
        case EDI: m->registers[5] = value;                                   break;
        case DI:  m->registers[5] = (m->registers[5] & 0xFF00) + value;      break;
        case ESP: m->registers[6] = value;                                   break;
        case SP:  m->registers[6] = (m->registers[6] & 0xFF00) + value;      break;
        case EBP: m->registers[7] = value;                                   break;
        case BP:  m->registers[7] = (m->registers[7] & 0xFF00) + value;      break;
    }
}

void machine_memory_set_from_register(machine_t* m, registry reg, uint32_t address){
    switch(reg){
        case EAX:
            m->memory[address] = m->registers[0] >> 24;
            m->memory[address + 1] = m->registers[0] >> 16;
            m->memory[address + 2] = m->registers[0] >> 8;
            m->memory[address + 3] = m->registers[0];
            break;
        case AX:
            m->memory[address] = m->registers[0] >> 8;
            m->memory[address + 1] = m->registers[0];
            break;
        case AH:
            m->memory[address] = m->registers[0] >> 8;
            break;
        case AL:
            m->memory[address] = m->registers[0];
            break;
        case EBX:
            m->memory[address] = m->registers[1] >> 24;
            m->memory[address + 1] = m->registers[1] >> 16;
            m->memory[address + 2] = m->registers[1] >> 8;
            m->memory[address + 3] = m->registers[1];
            break;
        case BX:
            m->memory[address] = m->registers[1] >> 8;
            m->memory[address + 1] = m->registers[1];
            break;
        case BH:
            m->memory[address] = m->registers[1] >> 8;
            break;
        case BL:
            m->memory[address] = m->registers[1];
            break;
        case ECX:
            m->memory[address] = m->registers[2] >> 24;
            m->memory[address + 1] = m->registers[2] >> 16;
            m->memory[address + 2] = m->registers[2] >> 8;
            m->memory[address + 3] = m->registers[2];
            break;
        case CX:
            m->memory[address] = m->registers[2] >> 8;
            m->memory[address + 1] = m->registers[2];
            break;
        case CH:
            m->memory[address] = m->registers[2] >> 8;
            break;
        case CL:
            m->memory[address] = m->registers[2];
            break;
        case EDX:
            m->memory[address] = m->registers[3] >> 24;
            m->memory[address + 1] = m->registers[3] >> 16;
            m->memory[address + 2] = m->registers[3] >> 8;
            m->memory[address + 3] = m->registers[3];
            break;
        case DX:
            m->memory[address] = m->registers[3] >> 8;
            m->memory[address + 1] = m->registers[3];
            break;
        case DH:
            m->memory[address] = m->registers[3] >> 8;
            break;
        case DL:
            m->memory[address] = m->registers[3];
            break;
        case ESI:
            m->memory[address] = m->registers[4] >> 24;
            m->memory[address + 1] = m->registers[4] >> 16;
            m->memory[address + 2] = m->registers[4] >> 8;
            m->memory[address + 3] = m->registers[4];
            break;
        case SI:
            m->memory[address] = m->registers[4] >> 8;
            m->memory[address + 1] = m->registers[4];
            break;
        case EDI:
            m->memory[address] = m->registers[5] >> 24;
            m->memory[address + 1] = m->registers[5] >> 16;
            m->memory[address + 2] = m->registers[5] >> 8;
            m->memory[address + 3] = m->registers[5];
            break;
        case DI:
            m->memory[address] = m->registers[5] >> 8;
            m->memory[address + 1] = m->registers[5];
            break;
        case ESP:
            m->memory[address] = m->registers[6] >> 24;
            m->memory[address + 1] = m->registers[6] >> 16;
            m->memory[address + 2] = m->registers[6] >> 8;
            m->memory[address + 3] = m->registers[6];
            break;
        case SP:
            m->memory[address] = m->registers[6] >> 8;
            m->memory[address + 1] = m->registers[6];
            break;
        case EBP:
            m->memory[address] = m->registers[7] >> 24;
            m->memory[address + 1] = m->registers[7] >> 16;
            m->memory[address + 2] = m->registers[7] >> 8;
            m->memory[address + 3] = m->registers[7];
            break;
        case BP:
            m->memory[address] = m->registers[7] >> 8;
            m->memory[address + 1] = m->registers[7];
            break;
    }
}

#define debug_padding // add padding to output of DEBUG_print

#define printi(integer) printf("%d", integer)
#define prints(string) printf("%s", string)

#define sizeoftype(x) _Generic((x), \
    int8_t:  8, \
    int16_t: 16, \
    default: 0)

void DEBUG_REGISTRY_print(int16_t x){
    printf("%dd %04xh ", x, x);
    uint16_t size = sizeoftype(x);
    char buffer[size];
    memset(buffer, 0, size);
    int16_t i = 0;
#ifdef debug_padding
    uint16_t counter = size;
    while(counter--){
#else
    while(x){
#endif
        buffer[i++] = x & 1 ? '1' : '0';
        x >>= 1;
    }
    for(i = size; i >= 0; i--)
        printf("%c", buffer[i]);
    printf("b\n");
}

void DEBUG_FLAGS_print(int8_t x){
    uint16_t size = sizeoftype(x);
    char buffer[size];
    memset(buffer, 0, size);
    int16_t i = 0;
    uint16_t counter = size;
    while(counter--){
        printf("%c: %c\n", " zs     "[i++], x & 1 ? '1' : '0');
        x >>= 1;
    }
}

uint16_t flags = 0x00;

#define OP_MOV(a, b) OP_mov(&a, &b)
#define OP_ADD(a, b) OP_add(&a, &b)
#define OP_SUB(a, b) OP_sub(&a, &b)
#define OP_MOL(a, b) OP_mol(&a, &b)
#define OP_DIV(a, b) OP_div(&a, &b)
#define OP_INC(a) OP_inc(&a)
#define OP_DEC(a) OP_dec(&a)

void OP_mov(int16_t* a, int16_t* b){ *a=*b; }
void OP_add(int16_t* a, int16_t* b){ *a+=*b; }
void OP_sub(int16_t* a, int16_t* b){ *a-=*b; }
void OP_mol(int16_t* a, int16_t* b){ *a*=*b; }
void OP_div(int16_t* a, int16_t* b){ *a/=*b; }

void OP_inc(int16_t* a){
    *a += 1;
    flags = ((*a == 0) << 1) + ((*a < 0) << 2);
//    for(uint8_t i = sizeof(*a * 8); i > 0; i /= 2 ) *a ^= *a >> i;
//    flags |= ((~(*a) & 1) == 1  ? 0x08 : 0x0);
}

void OP_dec(int16_t* a){
    *a -= 1;
    flags = ((*a == 0) << 1) + ((*a < 0) << 2);
//    for(uint8_t i = sizeof(*a * 8); i > 0; i /= 2 ) *a ^= *a >> i;
//    flags |= ((~(*a) & 1) == 1  ? 0x08 : 0x0);
}

void DEBUG_MEMORY_print(int8_t* memory, uint16_t lower, uint16_t upper){
    if(lower <= upper){
        for(uint16_t i = lower; upper > 0 ? i < upper * 8: 0; i+=8){

            printf("%04x: ", i * 8);

            for(uint8_t j = 0; j < 8; j++)
                printf("%02hhX ", memory[i + j]);

            for(uint8_t j = 0; j < 8; j++)
                printf("%c",
                    memory[i + j] > 30 && memory[i + j] < 122
                    ? memory[i + j]
                    : '.');

            printf("\n");
        }
    }
}

void memory_set_byte(int8_t* memory, uint16_t address, int8_t new){
    memory[address] = new;
}

void memory_set_word(int8_t* memory, uint16_t address, int16_t new){
    memory[address] = new >> 8;
    memory[address + 1] = new;
}

void memory_set_double_word(int8_t* memory, uint16_t address, int32_t new){
    memory[address + 0] = new >> 24;
    memory[address + 1] = new >> 16;
    memory[address + 2] = new >> 8;
    memory[address + 3] = new >> 0;
}

char* load_file(char* filepath){
    FILE  *file = fopen(filepath, "r");
    char *buffer;
    long length;

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    rewind(file);

    buffer = malloc((length + 1) * sizeof(*buffer));
    fread(buffer, length, 1, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

void editor(){
    uint16_t current_address = 10;
    char instruction[3] = {0};
    while(1){
        printf("%04X> ", current_address);
        fgets(instruction, 3, stdin);
        current_address++;
    }
}

int main(){

    machine_t* m = machine_new(20000);
    MACHINE_REGISTER_SET(EAX, 2115456);
    printf("%d\n", MACHINE_REGISTER_GET_32(EAX));

    /*
    machine_memory_set_from_register(m, EAX, 100);
    printf("%d", m->memory[100]);
    printf("%d", m->memory[101]);
    printf("%d", m->memory[102]);
    printf("%d", m->memory[103]);

    //printf("%s", load_file("main.asm"));

    // create memory
    int8_t* memory = (int8_t*)malloc(sizeof(int8_t) * 8 * 200);
    for(int i = 0; i < 8 * 200; i++)
        memory[i] = -1;

    // write hello world to memory
    memory_set_byte(memory, 0x000, 0x68);
    memory_set_byte(memory, 0x001, 0x65);
    memory_set_byte(memory, 0x002, 0x6c);
    memory_set_byte(memory, 0x003, 0x6c);
    memory_set_byte(memory, 0x004, 0x6f);
    memory_set_byte(memory, 0x005, 0x20);
    memory_set_byte(memory, 0x006, 0x77);
    memory_set_byte(memory, 0x007, 0x6f);
    memory_set_byte(memory, 0x008, 0x72);
    memory_set_byte(memory, 0x009, 0x6c);
    memory_set_byte(memory, 0x00A, 0x64);

    int16_t AX = 1;
    int16_t BX = 3;
    int16_t CX = 0;
    int16_t DX = 0;

    char instruction[64] = {0};

    while(1){
        system("clear");

        prints("AX: ");
        DEBUG_REGISTRY_print(AX);
        prints("BX: ");
        DEBUG_REGISTRY_print(BX);
        prints("CX: ");
        DEBUG_REGISTRY_print(CX);
        prints("DX: ");
        DEBUG_REGISTRY_print(DX);

        prints("\n");
        //DEBUG_FLAGS_print(flags);

        DEBUG_MEMORY_print(memory, 0, 5);
        memory_set_word(memory, 0x000, AX);
        memory_set_word(memory, 0x002, BX);

        prints("instruction: ");
        fgets(instruction, 64, stdin);
        switch(instruction[0]){
            case 'a':
                OP_ADD(AX, BX);
                break;
            case 's':
                OP_SUB(AX, BX);
                break;
            case 'm':
                OP_MOL(AX, BX);
                break;
            case 'd':
                OP_DIV(AX, BX);
                break;
        }
    }
    */

    return 0;
}
