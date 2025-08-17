#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "header-IR.h"

int main (int argc, const char *argv[])
{
    // TODO: check args

    Tree_t tree = {};
    TreeCtor (&tree, argv[2], argv[3]);

    ListIR_t list = {};
    ListIRCtor (&list, argv[3], argv[4]);

    CreateIR (&tree, &list);
    Dump (&list);

    // if (!strcmp (argv[1], "elf64"))
        CreateElf (&list);
    // else if (!strcmp (argv[1], "nasm"))
        WriteNasm (&list);

    TreeDtor (&tree);
    ListIRDtor (&list);

}