#include "utils.h"

void *smalloc(size_t n)
{
    void *pointer;

    pointer = malloc(n);
    if (!pointer)
        exit_failure("malloc");
    return pointer;
}
