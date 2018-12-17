#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<inttypes.h>
#include<string.h>

#include "virtual_machine.h"
#include "interpreter.h"

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

    virtual_machine* vm = virtual_machine_new(200);

    vm_register_set(vm, AH, 0x04);
    vm_register_set(vm, AL, 0x03);

    vm_memory_set_register(vm, AX, 0);

    vm_register_set(vm, AL, 0x01);

    vm_memory_print(vm, 0, 5);
    vm_register_set(vm, IP, 0);

    interpreter_execute(vm);
    printf("%d\n", vm->registers[0]);
    vm_memory_set_register(vm, AX, 0);
    vm_memory_print(vm, 0, 5);

    virtual_machine_destroy(vm);

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
