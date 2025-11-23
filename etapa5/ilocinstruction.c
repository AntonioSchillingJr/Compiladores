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

    new_instruction->op_code = op_code ? strdup(op_code) : NULL;
    new_instruction->src1    = src1 ? strdup(src1) : NULL;
    new_instruction->src2    = src2 ? strdup(src2) : NULL;
    new_instruction->dest    = dest ? strdup(dest) : NULL;

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

void ILOCInstruction_print(ILOCInstruction *i) {
    if (!i || !i->op_code) return;

    size_t n = strlen(i->op_code);
    if (n > 0 && i->op_code[n-1] == ':') {
        printf("%s nop\n", i->op_code);
        return;
    }

    if (!strcmp(i->op_code, "jumpI")) {
        printf("jumpI -> %s\n", i->dest);
        return;
    }

    if (!strcmp(i->op_code, "cbr")) {
        printf("cbr %s -> %s\n", i->src1, i->dest);
        return;
    }

    if (!strncmp(i->op_code, "cmp_", 4)) {
        printf("%s %s, %s -> %s\n", i->op_code, i->src1, i->src2, i->dest);
        return;
    }

    if (!strcmp(i->op_code, "storeAI")) {
        printf("storeAI %s => %s\n", i->src1, i->dest);
        return;
    }

    if (!strcmp(i->op_code, "loadAI")) {
        printf("loadAI %s, %s => %s\n", i->src1, i->src2, i->dest);
        return;
    }

    if (i->src1 && i->src2 && i->dest) {
        printf("%s %s, %s => %s\n", i->op_code, i->src1, i->src2, i->dest);
        return;
    }

    if (i->src1 && i->dest) {
        printf("%s %s => %s\n", i->op_code, i->src1, i->dest);
        return;
    }

    printf("%s\n", i->op_code);
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