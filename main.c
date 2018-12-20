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

    REGISTER_SET(AH, 0x0004);
    REGISTER_SET(AL, 0x0005);
    vm_memory_set_register(vm, AX, 0);
    vm_memory_print(vm, 0, 4);

    //printf("AL:%d\n", REGISTER_GET_BYTE(AL));
    interpreter_execute(vm);
    //printf("AL:%d\n", REGISTER_GET_BYTE(AL));

    interpreter_execute(vm);

    vm->memory[2] = 0xCD;
    vm->memory[3] = 0x21;
    vm->memory[4] = 0x76;
    vm->memory[5] = 0x65;
    vm->memory[6] = '$';
    REGISTER_SET(AH, 0x2);
    //REGISTER_SET(DL, 'a');
    //REGISTER_SET(DL, 0x10);
    //vm->registers[0] = 0x00000200;
    //printf("%d\n", REGISTER_GET_BYTE(AH));
    //vm_memory_print(vm, 0, 4);

    interpreter_execute(vm);

    printf("%d\n", REGISTER_GET_BYTE(AL));

    virtual_machine_destroy(vm);

    return 0;
}
