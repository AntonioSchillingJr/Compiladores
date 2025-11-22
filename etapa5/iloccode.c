#include "iloccode.h"

static ILOCCode *code_register = NULL;
static List *temp_memory_list = NULL;
static unsigned long int temp_counter = 3;

ILOCCode *ILOCCode_new() {
    ILOCCode *code = malloc(sizeof(ILOCCode));
    code->instructions = malloc(ILOCCODE_START_ARRAY_SIZE * sizeof(ILOCInstruction));
    code->count = 0;
    code->max_capacity = ILOCCODE_START_ARRAY_SIZE;
    code->next_temp = 1;
    code->last_temp = -1;  
    return code;
}

void ILOCCode_start() {
    if ((temp_counter != 0) || (temp_memory_list != NULL)) {
        ILOCCode_end();
    }

    code_register = ILOCCode_new();
    temp_counter = 3;
    temp_memory_list = List_create_empty();
}

char *ILOCCode_new_temp() {
    char *temp = malloc(ILOCCODE_TEMP_MAX_SIZE * sizeof(char));
    sprintf(temp, "r%lu", temp_counter);
    temp_counter++;
    temp_memory_list = List_add_node(temp_memory_list, temp);
    return temp;
}

void *ILOCCode_temp_destroy(void *_temp) {
    if (_temp != NULL) {
        free(_temp);
    }
    return NULL;
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

void ILOCCode_direct_add_instruction(ILOCCode *_code, char *_op, char *_src1, char *_src2, char *_dest) {
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

char *ILOCCode_direct_add_instruction_with_new_temp(ILOCCode *_code, char *_op, char *_src1, char *_src2) {
    char *dest_str = ILOCCode_new_temp();
    
    ILOCCode_direct_add_instruction(_code, _op, _src1, _src2, dest_str);
    _code->last_temp = temp_counter;
    
    return dest_str;
}

char *ILOCCode_add_instruction_with_new_temp(char *_op, char *_src1, char *_src2) {
    return ILOCCode_direct_add_instruction_with_new_temp(code_register, _op, _src1, _src2);
}   

void ILOCCode_add_instruction(char *_op, char *_src1, char *_src2, char *_dest) {
    ILOCCode_direct_add_instruction(code_register, _op, _src1, _src2, _dest);
}

int ILOCCode_update_current_temp() {
    return temp_counter++;
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

void ILOCCode_end() {
    if ((temp_counter != 0) || (temp_memory_list != NULL)) {
        temp_counter = 0;
        temp_memory_list = List_destroy(temp_memory_list, ILOCCode_temp_destroy);
    }
    code_register = ILOCCode_destroy(code_register);
}

void ILOCCode_print(ILOCCode *_code) {
    if (_code != NULL) {
        printf("ILOCCode: [");
        if (_code->count != 0) {
            for (size_t i = 0; i < _code->count - 1; i++) {
                ILOCInstruction_print(&_code->instructions[i]);
                printf(", ");
            }
            ILOCInstruction_print(&_code->instructions[_code->count - 1]);
        }
        printf("]\n");
    } else {
        printf("ILOCCode: [NULL]\n");
    }
}

char *ILOCCode_current_temp_string() {
    return temp_memory_list->data;
}

void ILOCCode_temp_print(void *_temp) {
    if (_temp != NULL) {
        printf("%s", (char *)_temp);
    } else {
        printf("NULL");
    }
}

void ILOCCode_memory_temp_print() {
    List_print(temp_memory_list, ILOCCode_temp_print);
}

void ILOCCode_print_current_code_structure_memory_state() {
    printf("Code Register:");
    ILOCCode_print(code_register);        
    printf("Generated temporaries: ");
    ILOCCode_memory_temp_print();   
    printf("\n");
}

void ILOCCode_test_implementation() {
    printf("--------------- Tests for %s implementation: Running now ---------------\n", __FILE__);
        printf("Before starting library ------------------------------\n");
        ILOCCode_print_current_code_structure_memory_state();

        printf("Starting library ------------------------------\n");
        ILOCCode_start();
        ILOCCode_print_current_code_structure_memory_state();
        
        printf("Adding directly some new instruction:\n");
        ILOCCode_add_instruction("_op", "_src1", "_src2", "_dest");
        ILOCCode_print_current_code_structure_memory_state();
        printf("Again, adding directly some new instruction:\n");
        ILOCCode_add_instruction("_op1", "_src11", "_src21", "_dest1");
        ILOCCode_print_current_code_structure_memory_state();

        printf("Adding instruction with new temp:\n");
        ILOCCode_add_instruction_with_new_temp("_op2", "_src12", "_src22");
        ILOCCode_print_current_code_structure_memory_state();
        printf("Again, adding instruction with new temp:\n");
        ILOCCode_add_instruction_with_new_temp("_op3", "_src13", "_src23");
        ILOCCode_print_current_code_structure_memory_state();
        printf("Again, adding instruction with new temp:\n");
        ILOCCode_add_instruction_with_new_temp("_op3", "_src13", "_src23");
        ILOCCode_print_current_code_structure_memory_state();

        printf("Ending library ------------------------------\n");
        ILOCCode_end();
        ILOCCode_print_current_code_structure_memory_state();
    printf("--------------- Tests for %s implementation: Ended ---------------\n", __FILE__);
}