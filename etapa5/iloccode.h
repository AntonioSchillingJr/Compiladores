#ifndef ILOCCODE_H
#define ILOCCODE_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ilocinstruction.h"

#define ILOCCODE_START_ARRAY_SIZE 100     // padronizando tamanho maximo dos buffers utilizados

typedef struct ILOCCode {
    ILOCInstruction *instructions;  // array de intrucoes
    int count;                      // numero de intrucoes
    int max_capacity;               // capacidade maxima do array
    int next_temp;                  // proximo registrador temporario
    int last_temp;                  // ultimo temporario resultante
} ILOCCode;

ILOCCode *ILOCCode_new();
void ILOCCode_increase_capacity(ILOCCode *_code);
bool ILOCCode_is_valid_temp_register(const char *_str);
int ILOCCode_add_instruction_with_temp(ILOCCode *_code, char *_op, char *_src1, char *_src2);
void ILOCCode_add_instruction_no_temp(ILOCCode *_code, char *_op, char *_src1, char *_src2, char *_dest);
void ILOCCode_add_instruction(ILOCCode *_code, char *_op, char *_src1, char *_src2, char *_dest);
int ILOCCode_update_current_temp(ILOCCode *_code);
int ILOCCode_calculate_next_temp(ILOCCode *_code);
char *ILOCCode_temp_to_string(int _temp);
ILOCCode *ILOCCode_destroy(ILOCCode *_code);

#endif  // #ifndef ILOCCODE_H