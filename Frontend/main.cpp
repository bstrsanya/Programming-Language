#include <stdio.h>
#include <stdlib.h>

#include "Parser.h"

int main ()
{
    Tree_t tree = {};
    TreeCtor (&tree, FILE_EXPRESSION);

    PrintDot (tree.expression, "frontend.png", &tree);

    CreateTreeTxt (&tree);

    TreeDtor (&tree);
    return 0;
}
