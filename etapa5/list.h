#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"

typedef struct List {
    void *data;
    struct List *next;
} List;

List *List_create_empty();
List *List_create(void *_data);
List *List_stack(List *_list, void *_data);
List *List_append(List *_list, void *_data);
List *List_add_node(List *_list, void *_data);
List *List_destroy(List *_list, void *destroy_function(void *));
void List_print(List *_list, void print_function(void *));
void List_test_implementation();

#endif  // #ifndef LIST_H