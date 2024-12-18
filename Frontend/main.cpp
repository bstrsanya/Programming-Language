#include <stdio.h>
#include <stdlib.h>

#include "Parser.h"

int main (int argc, const char *argv[])
{
    //TODO: check args
    Tree_t tree = {};
    TreeCtor (&tree, argv[1], argv[2]);

    PrintDot (tree.expression, "frontend.png", &tree);

    CreateTreeTxt (&tree);

    TreeDtor (&tree);
    return 0;
}
