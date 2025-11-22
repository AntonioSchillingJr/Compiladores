#ifndef ILOCCODE_H
#define ILOCCODE_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ilocinstruction.h"

#define ILOCCODE_START_ARRAY_SIZE   100     // padronizando tamanho maximo dos buffers utilizados
#define ILOCCODE_TEMP_MAX_SIZE      500     // padronizando tamanho maximo dos buffers utilizados

typedef struct ILOCCode {
    ILOCInstruction *instructions;  // array de intrucoes
    int count;                      // numero de intrucoes
    int max_capacity;               // capacidade maxima do array
    int next_temp;                  // proximo registrador temporario
    int last_temp;                  // ultimo temporario resultante
} ILOCCode;

void ILOCCode_start();
char *ILOCCode_new_temp();
char *ILOCCode_add_instruction_with_new_temp(char *_op, char *_src1, char *_src2);
void ILOCCode_add_instruction(char *_op, char *_src1, char *_src2, char *_dest);
int ILOCCode_update_current_temp();
char *ILOCCode_current_temp_string();
void ILOCCode_end();
void ILOCCode_test_implementation();

#endif  // #ifndef ILOCCODE_H