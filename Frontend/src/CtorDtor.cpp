#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Parser.h"



Node_t* NodeCtor (int type, double value, Node_t* left, Node_t* right)
{
    Node_t* new_node = (Node_t*) calloc (1, sizeof (Node_t));
    assert (new_node);

    new_node->type = (TypeCommand_t) type;

    if (type == NUM)
        new_node->value.number = value;
    else if (type == VAR)
        new_node->value.var = (int) value;
    else
        new_node->value.com = (ListCommand_t) value;

    new_node->left = left;
    new_node->right = right;

    return new_node;
}

void NodeDtor (Node_t* node)
{
    if (!node) return;
    
    if (node->left) NodeDtor (node->left);
    if (node->right) NodeDtor (node->right);

    if (node->type == BLOCK) free (node);
}

void TreeCtor (Tree_t* tree, const char* name_file_input, const char* name_file_output)
{
    assert (tree);
    assert (name_file_input);
    assert (name_file_output);

    printf ("I'm frontend! Open file [%s] for read and [%s] for writing\n", name_file_input, name_file_output);

    tree->input = fopen (name_file_input, "rb");

    char** table = (char**) calloc (SIZE_TABLE_VAR, sizeof (char*));
    assert (table);
    for (int i = 0; i < SIZE_TABLE_VAR; i++)
        table[i] = NULL;

    char* array = (char*) calloc (SIZE_ARRAY, sizeof (char));
    assert (array);

    tree->read_data = array;
    tree->table_var = table;

    ReadDataBase (tree);
    fclose (tree->input);

    tree->output = fopen (name_file_output, "wb");
}

void TreeDtor (Tree_t* tree)
{
    assert (tree);

    NodeDtor (tree->expression);

    for (int i = 0; i < SIZE_ARRAY; i++)
        free (tree->array[i]);

    free (tree->array);

    free (tree->table_var);
    free (tree->read_data);

    if (tree->output)
        fclose (tree->output);
}

void CreateTreeTxt (Tree_t* tree)
{
    PrintTxt (tree->expression, tree);

    fprintf (tree->output, "\n\n");
    int i = 0;
    while (tree->table_var[i])
    {
        fprintf (tree->output, "%s\n", tree->table_var[i]);
        i++;
    }
}

void PrintTxt (Node_t* node, Tree_t* tree)
{
    switch ((int) node->type)
    {
        case FUNC:
        {
            fprintf (tree->output, "(5 %d ", node->value.var);
            if (node->left) PrintTxt (node->left, tree);
            else fprintf (tree->output, "_ ");

            if (node->right) PrintTxt (node->right, tree);
            else fprintf (tree->output, "_ ");

            fprintf (tree->output, ")");
            break;
        }
        case BLOCK:
        {
            fprintf (tree->output, "(3 %d ", F_INTERRUPT);
            if (node->left) PrintTxt (node->left, tree);
            else fprintf (tree->output, "_ ");

            if (node->right) PrintTxt (node->right, tree);
            else fprintf (tree->output, "_ ");

            fprintf (tree->output, ")");
            break;
        }
        case NUM:
        {
            fprintf (tree->output, "(1 %g _ _ )", node->value.number);
            break;
        }
        case VAR:
        {
            fprintf (tree->output, "(2 %d _ _ )", node->value.var);
            break;
        }
        case OP:
        {
            if (node->value.com != F_PRINT  && 
                node->value.com != F_INT    && 
                node->value.com != F_DOUBLE && 
                node->value.com != F_INPUT  && 
                node->value.com != F_SQRT   &&
                node->value.com != F_PRINT_C )
            {
                fprintf (tree->output, "(3 %d ", node->value.com);
                PrintTxt (node->left, tree);
                PrintTxt (node->right, tree);
                fprintf (tree->output, ")");
            }
            else 
            {
                fprintf (tree->output, "(3 %d _ ", node->value.com);
                PrintTxt (node->right, tree);
                fprintf (tree->output, ")");
            }
            break;
        }
        default: break;
    }
}