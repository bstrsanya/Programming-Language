#include <stdio.h>
#include <stdlib.h>

#include "header-IR.h"

int main (int argc, const char *argv[])
{
    // TODO: check args
    ListIR_t list = {};
    ListIRCtor (&list, argv[1], argv[2]);
    // WriteNasm (list);
    CreateElf (&list);
    ListIRDtor (&list);

    return 0;
}