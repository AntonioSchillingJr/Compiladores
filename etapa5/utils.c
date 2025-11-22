#include "utils.h"

const char *bool_to_string(bool _boolean) {
    return ((_boolean) ? "True" : "False");
}

bool is_nullified(void *_pointer) {
    return (_pointer == NULL);
}

const char *is_nullified_string(void *_pointer) {
    return ((_pointer == NULL) ? "Nullified" : "Not Nullified");
}