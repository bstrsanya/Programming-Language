#include <stdio.h>
#include <stdlib.h>

#include "Parser.h"

int main ()
{
    Tree_t tree = {};
    TreeCtor (&tree, FILE_EXPRESSION);

    PrintDot (tree.expression, "qwe.png", &tree);
    TreeDtor (&tree);
    return 0;
}
