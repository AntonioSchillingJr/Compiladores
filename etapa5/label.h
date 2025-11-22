#ifndef LABEL_H
#define LABEL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"
#include "list.h"

#define LABEL_MAX_SIZE 50           // padronizando tamanho maximo dos buffers utilizados

void Label_start();
char *Label_new();
void Label_end();
void Label_test_implementation();

#endif  // #ifndef LABEL_H