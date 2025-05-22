#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "header-IR.h"

int main (int argc, const char *argv[])
{
    // TODO: check args
    ListIR_t list = {};
    ListIRCtor (&list, argv[2], argv[3]);
    if (!strcmp (argv[1], "elf64"))
        CreateElf (&list);
    else if (!strcmp (argv[1], "nasm"))
        WriteNasm (&list);
    ListIRDtor (&list);

    return 0;
}