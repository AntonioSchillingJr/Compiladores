#include "list.h"

List *List_create(void *_data) {
    List *new_node = malloc(sizeof(List));
    new_node->data = _data;
    new_node->next = NULL;
    return new_node;
}

List *List_create_empty() {
    List *new_node = malloc(sizeof(List));
    new_node->data = NULL;
    new_node->next = NULL;
    return new_node;
}

List *List_add_node(List *_list, void *_data) {
    
    if (_list == NULL) {
        return List_create(_data);
    }

    // empilha sobre a lista ja existente
    List *new_node = malloc(sizeof(List));
    new_node->data = _data;
    new_node->next = _list;

    return new_node;
}

List *List_add_node_at_the_end(List *_list, void *_data) {
    
    if (_list == NULL) {
        return List_create(_data);
    }

    List *aux = _list;
    while (aux->next != NULL) {
        aux = aux->next;
    }

    List *new_node = malloc(sizeof(List));
    new_node->data = _data;
    new_node->next = NULL;

    aux->next = new_node;

    return _list;
}

void *List_default_destroy_function(void *_data) {
    if (_data != NULL) {
        free(_data);
    }

    return NULL;
}

List *List_destroy(List *_list, void* destroy_function(void*)) {

    while (_list != NULL) {

        // salva valor antes da mudanca
        List *aux = _list;

        // update para proximo valor antes de destruir memoria
        _list = _list->next;

        // limpa recursos do node atual
        aux->data = destroy_function(aux->data);
        free(aux);
    }

    return NULL;
}

void List_default_print_function(void *_data) {
    printf("%p", _data);
}

void List_print(List *_list, void print_function(void *)) {
    printf("[ ");
    while (_list != NULL) {
        print_function(_list->data);
        if (_list->next != NULL) {
            printf(", ");
        } 
        _list = _list->next;
    }
    printf(" ]\n");
}

void List_test_implementation() {
    printf("--------------- Tests for %s implementation: Running now ---------------\n", __FILE__);

        int *test_0 = malloc(sizeof(int));
        char *test_1 = malloc(sizeof(char));
        double *test_2 = malloc(sizeof(double));

        printf("Creating list:\n");
        List *List_test = List_create(test_0);
        List_print(List_test, List_default_print_function);

        printf("Adding data:\n");
        List_test = List_add_node(List_test, test_1);
        List_print(List_test, List_default_print_function);

        printf("Adding data at the end of the list:\n");
        List_test = List_add_node_at_the_end(List_test, test_2);
        List_print(List_test, List_default_print_function);

        List_test = List_destroy(List_test, List_default_destroy_function);
        printf("Pointer state after destroy operation: %s.\n", is_nullified_string(List_test));
    printf("--------------- Tests for %s implementation: Ended ---------------\n", __FILE__);
}