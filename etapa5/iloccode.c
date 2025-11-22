#include "iloccode.h"

ILOCCode *ILOCCode_new() {
    ILOCCode *code = malloc(sizeof(ILOCCode));
    code->instructions = malloc(ILOCCODE_START_ARRAY_SIZE * sizeof(ILOCInstruction));
    code->count = 0;
    code->max_capacity = ILOCCODE_START_ARRAY_SIZE;
    code->next_temp = 1;
    code->last_temp = -1;  
    return code;
}

void ILOCCode_increase_capacity(ILOCCode *_code) {
    _code->max_capacity *= 2;
    _code->instructions = realloc(_code->instructions, _code->max_capacity * sizeof(ILOCInstruction));
}

bool ILOCCode_is_valid_temp_register(const char *_str) {
    if ((_str == NULL) || (strlen(_str) < 2)) {
        return false;
    }
    if (_str[0] != 'r') { 
        return false; 
    }
    for (int i = 1; _str[i] != '\0'; i++) {
        if (!isdigit(_str[i])) {
            return false;
        }    
    }
    return true;
}

int ILOCCode_add_instruction_with_temp(ILOCCode *_code, char *_op, char *_src1, char *_src2) {
    int new_temp = ILOCCode_update_current_temp(_code);
    char *dest_str = ILOCCode_temp_to_string(new_temp);
    
    ILOCCode_add_instruction(_code, _op, _src1, _src2, dest_str);
    _code->last_temp = new_temp;
    
    free(dest_str);
    return new_temp;
}

void ILOCCode_add_instruction_no_temp(ILOCCode *_code, char *_op, char *_src1, char *_src2, char *_dest) {
    ILOCCode_add_instruction(_code, _op, _src1, _src2, _dest);
    // nao sei se faz sentido sobreescrever o temporario da ultima operacao
    // _code->last_temp = -1;
}

void ILOCCode_add_instruction(ILOCCode *_code, char *_op, char *_src1, char *_src2, char *_dest) {
    // resize se necessario
    if (_code->count >= _code->max_capacity) {
        ILOCCode_increase_capacity(_code);
    }
    
    ILOCInstruction *instr = &_code->instructions[_code->count++];
    instr->op_code = strdup(_op);
    instr->src1 = _src1 ? strdup(_src1) : NULL;
    instr->src2 = _src2 ? strdup(_src2) : NULL;
    instr->dest = _dest ? strdup(_dest) : NULL;
}

int ILOCCode_update_current_temp(ILOCCode *_code) {
    return _code->next_temp++;
}

int ILOCCode_calculate_next_temp(ILOCCode *_code) {
    return _code->next_temp + 1;
}

char *ILOCCode_temp_to_string(int _temp) {
    char *str = malloc(ILOCINSTRUCTION_MAX_INSTRUCTION_STRING_SIZE);
    sprintf(str, "r%d", _temp);
    return str;
}

ILOCCode *ILOCCode_destroy(ILOCCode *_code) {
    if (_code != NULL) {
        if (_code->instructions != NULL) {
            free(_code->instructions);
        }
        free(_code);
    }
    return NULL;
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