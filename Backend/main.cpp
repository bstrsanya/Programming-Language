#include <stdio.h>
#include <stdlib.h>

#include "Recovery.h"

int main (int argc, const char *argv[])
{
    //TODO: check args
    Tree_t tree = {};
    TreeCtor (&tree, argv[1], argv[2]);

    PrintDot (tree.expression, "backend.png", &tree);

    CreateAsmFile (&tree);

    TreeDtor (&tree);

    return 0;
}