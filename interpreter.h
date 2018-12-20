#ifndef INTERPRETER_H
#define INTERPRETER_H

void interpreter_execute(virtual_machine*);
void interpreter_interrupts(virtual_machine*, uint8_t);
void interpreter_dos(virtual_machine*);

#endif /* INTERPRETER_H */
