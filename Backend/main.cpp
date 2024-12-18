#include <stdio.h>
#include <stdlib.h>

#include "Recovery.h"

int main ()
{
    Tree_t tree = {};
    TreeCtor (&tree, INPUT_FILE);

    PrintDot (tree.expression, "backend.png", &tree);
    fprintf (tree.output, "CALL main:\nHLT\n");
    CreateAsmFile (tree.expression, &tree);

    TreeDtor (&tree);

    return 0;
}