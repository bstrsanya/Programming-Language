#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Parser.h"

Node_t* NodeCtor (int type, double value, Node_t* left, Node_t* right)
{
    Node_t* new_node = (Node_t*) calloc (1, sizeof (Node_t));
    assert (new_node);

    new_node->type = (type_com) type;
    new_node->value = value;

    new_node->left = left;
    new_node->right = right;

    return new_node;
}

void NodeDtor (Node_t* node)
{
    if (!node) return;
    
    if (node->left) NodeDtor (node->left);
    if (node->right) NodeDtor (node->right);

    free (node);
}

void TreeCtor (Tree_t* tree, const char* name_file)
{
    assert (tree);
    assert (name_file);

    tree->input = fopen (name_file, "rb");
    ReadDataBase (tree);
    fclose (tree->input);

    tree->output = fopen (FILE_LATEX, "wb");
}

void TreeDtor (Tree_t* tree)
{
    assert (tree);

    NodeDtor (tree->expression);
    // for (int y = SIZE_ARRAY - 1; y != 0; y--)
    // {
    //     if ((int) tree->array[y]->value != '$')
    //         free (tree->array[y]);
    //     else
    //     {
    //         free (tree->array[y]);
    //         break;
    //     }
    // } 
    // for (int i = 0; i < SIZE_ARRAY; i++)
    // {
    //     if (tree->array[i])
    //         free (tree->array[i]);
    // }

    free (tree->array);
    NodeDtor (tree->expression_diff);

    if (tree->output)
        fclose (tree->output);
}