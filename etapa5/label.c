#include "label.h"

static unsigned long int label_counter = 0;
static List *label_memory_list = NULL;

void Label_start() {
    if ((label_counter != 0) || (label_memory_list != NULL)) {
        Label_end();
    }

    label_counter = 0;
    label_memory_list = List_create_empty();
}

void Label_add_label_to_memory_list(char *_label) {
    label_memory_list = List_add_node(label_memory_list, _label);
}

char *Label_new() {
    char *label = malloc(LABEL_MAX_SIZE * sizeof(char));
    sprintf(label, "L%ld", label_counter);
    label_counter++;
    Label_add_label_to_memory_list(label);
    return label;
}

void Label_print(void *_label) {
    if (_label != NULL) {
        printf("%s", (char *)_label);
    } else {
        printf("NULL");
    }
}

void *Label_destroy(void *_label) {
    if (_label != NULL) {
        free(_label);
    }
    return NULL;
}

void Label_end() {
    if ((label_counter != 0) || (label_memory_list != NULL)) {
        label_counter = 0;
        label_memory_list = List_destroy(label_memory_list, Label_destroy);
    }
}

void Label_test_implementation() {
    printf("--------------- Tests for %s implementation: Running now ---------------\n", __FILE__);
        printf("Pointer state before start operation: %s.\n", is_nullified_string(label_memory_list));
        printf("Memory list: ");
        List_print(label_memory_list, Label_print);

        printf("Starting lib:\n");
        Label_start();
        List_print(label_memory_list, Label_print);

        printf("New generated label: %s\n", Label_new());
        printf("Memory list: ");
        List_print(label_memory_list, Label_print);
        printf("New generated label: %s\n", Label_new());
        printf("Memory list: ");
        List_print(label_memory_list, Label_print);
        printf("New generated label: %s\n", Label_new());
        printf("Memory list: ");
        List_print(label_memory_list, Label_print);
        
        Label_end();
        printf("Pointer state after end operation: %s.\n", is_nullified_string(label_memory_list));
        printf("Memory list: ");
        List_print(label_memory_list, Label_print);
    printf("--------------- Tests for %s implementation: Ended ---------------\n", __FILE__);
}