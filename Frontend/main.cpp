#include <stdio.h>
#include <stdlib.h>

#include "Parser.h"

int main (int argc, const char *argv[])
{
    if (argc != 3) 
    {
        fprintf (stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1; 
    }

    Tree_t tree = {};
    TreeCtor (&tree, argv[1], argv[2]);

    PrintDot (&tree, FRONTEND_TREE_PNG);

    CreateTreeTxt (&tree);

    TreeDtor (&tree);
}
