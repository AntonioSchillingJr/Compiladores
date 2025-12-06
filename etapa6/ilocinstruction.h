#ifndef ILOCINSTRUCTION_H
#define ILOCINSTRUCTION_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"
#include "list.h"

#define ILOCCODE_START_ARRAY_SIZE                   100     // padronizando tamanho maximo dos buffers utilizados
#define ILOCINSTRUCTION_MAX_INSTRUCTION_STRING_SIZE 50      // padronizando tamanho maximo dos buffers utilizados

typedef struct ILOCInstruction {
    char *op_code;  // operacao
    char *src1;     // primeiro operando
    char *src2;     // segundo operando  
    char *dest;     // operando de destino
} ILOCInstruction;

ILOCInstruction *ILOCInstruction_new_empty();
ILOCInstruction *ILOCInstruction_new(char *op_code, char *src1, char *src2, char *dest);
void *ILOCInstruction_destroy(ILOCInstruction *_instruction);
void ILOCInstruction_print(ILOCInstruction *_instruction);
void ILOCInstruction_test_implementation();
#endif  // #ifndef ILOCINSTRUCTION_H