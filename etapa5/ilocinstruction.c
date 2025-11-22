#include "ilocinstruction.h"

ILOCInstruction *ILOCInstruction_new_empty() {
    ILOCInstruction *new_instruction = malloc(sizeof(ILOCInstruction));
    new_instruction->op_code = NULL;
    new_instruction->src1 = NULL;
    new_instruction->src2 = NULL;
    new_instruction->dest = NULL;
    return new_instruction;
}

ILOCInstruction *ILOCInstruction_new(char *op_code, char *src1, char *src2, char *dest) {
    ILOCInstruction *new_instruction = ILOCInstruction_new_empty();
    new_instruction->op_code = malloc(ILOCINSTRUCTION_MAX_INSTRUCTION_STRING_SIZE * sizeof(char));
    sprintf(new_instruction->op_code, op_code);
    if (src1 != NULL) {
        new_instruction->src1 = malloc(ILOCINSTRUCTION_MAX_INSTRUCTION_STRING_SIZE * sizeof(char));
        sprintf(new_instruction->src1, src1);
    }
    if (src2 != NULL) {
        new_instruction->src2 = malloc(ILOCINSTRUCTION_MAX_INSTRUCTION_STRING_SIZE * sizeof(char));
        sprintf(new_instruction->src2, src2);
    }
    if (dest != NULL) {
        new_instruction->dest = malloc(ILOCINSTRUCTION_MAX_INSTRUCTION_STRING_SIZE * sizeof(char));
        sprintf(new_instruction->dest, dest);
    }
    return new_instruction;
}

void *ILOCInstruction_destroy(ILOCInstruction *_instruction) {
    if (_instruction != NULL) {
        if (_instruction->op_code != NULL) {
            free(_instruction->op_code);
        }
        if (_instruction->src1 != NULL) {
            free(_instruction->src1);
        }
        if (_instruction->src2 != NULL) {
            free(_instruction->src2);
        }
        if (_instruction->dest != NULL) {
            free(_instruction->dest);
        }
        free(_instruction);
    }

    return NULL;
}

void ILOCInstruction_print(ILOCInstruction *_instruction) {
    if (_instruction != NULL) {
        printf("ILOCInstruction: [");
        printf("%s", _instruction->op_code);
        if (_instruction->src1 != NULL) {
            printf(", %s", _instruction->src1);
        }
        if (_instruction->src2 != NULL) {
            printf(", %s", _instruction->src2);
        }
        if (_instruction->dest != NULL) {
            printf(", %s", _instruction->dest);
        }
        printf("]");
    } else {
        printf("NULL");
    }
}

void ILOCInstruction_test_implementation() {
    printf("--------------- Tests for %s implementation: Running now ---------------\n", __FILE__);
        printf("Creating new instruction: ");
        ILOCInstruction *instruction = ILOCInstruction_new("add", "r1", "r2", "r3");
        ILOCInstruction_print(instruction);
        printf("\n");
        instruction = ILOCInstruction_destroy(instruction);
        printf("Pointer state after destroy operation: %s.\n", is_nullified_string(instruction));
        printf("Creating new instruction: ");
        instruction = ILOCInstruction_new("store", "r1", "r2", NULL);
        ILOCInstruction_print(instruction);
        printf("\n");
        instruction = ILOCInstruction_destroy(instruction);
        printf("Pointer state after destroy operation: %s.\n", is_nullified_string(instruction));
    printf("--------------- Tests for %s implementation: Ended ---------------\n", __FILE__);
}