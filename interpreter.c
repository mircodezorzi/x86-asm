#include<inttypes.h>

#include "virtual_machine.h"
#include "interpreter.h"
#include<stdio.h>
void
interpreter_execute(virtual_machine* vm)
{
    vm_register_set(vm, IP, vm_memory_get_byte(vm, MAR));
    uint8_t a, b, c;
    switch(vm_memory_get_byte(vm, VM_REGISTER_GET_U8(vm, IP))){
        case 0x04:
            a = VM_REGISTER_GET_U8(vm, AL);
            b = vm_memory_get_byte(vm, VM_REGISTER_GET_U8(vm, MAR) + 1);
            c = a + b;
            vm_register_set(vm, AL, c);
            printf("\n%d%\n", c);
            printf("%d\n%d\n",
            VM_REGISTER_GET_U8(vm, AL),
            vm_memory_get_byte(vm, VM_REGISTER_GET_U8(vm, MAR) + 1));
            printf("%d\n",
            VM_REGISTER_GET_U8(vm, AL) +
            vm_memory_get_byte(vm, VM_REGISTER_GET_U8(vm, MAR) + 1));
            break;
    }
    vm->registers[12]++;
    vm->registers[11] = vm->registers[12] ;
}
