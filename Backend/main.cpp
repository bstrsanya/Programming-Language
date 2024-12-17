#include <stdio.h>
#include <stdlib.h>

#include "Recovery.h"

int main ()
{
    Tree_t tree = {};
    TreeCtor (&tree, INPUT_FILE);

    PrintDot (tree.expression, "backend.png", &tree);
    CreateAsmFile (tree.expression, &tree);
    fprintf (tree.output, "HLT\n");

    TreeDtor (&tree);

    return 0;
}