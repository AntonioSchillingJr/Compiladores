#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const char *bool_to_string(bool _boolean);
bool is_nullified(void *_pointer);
const char *is_nullified_string(void *_pointer);

#endif  // #ifndef UTILS_H