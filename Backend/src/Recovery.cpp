#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "Recovery.h"

void TreeCtor (Tree_t* tree, const char* name_file_input, const char* name_file_output)
{
    assert (tree);
    assert (name_file_input);
    assert (name_file_output);

    printf ("I'm backend! Open file [%s] for read and [%s] for writing\n", name_file_input, name_file_output);

    tree->input = fopen (name_file_input, "rb");

    char** table = (char**) calloc (SIZE_TABLE_VAR, sizeof (char*));
    assert (table);
    for (int i = 0; i < SIZE_TABLE_VAR; i++)
        table[i] = NULL;

    tree->table_var = table;

    ReadDataBase (tree);
    fclose (tree->input);

    tree->output = fopen (name_file_output, "wb");
}

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

void TreeDtor (Tree_t* tree)
{
    assert (tree);

    for (int i = 0; i < SIZE_ARRAY; i++)
        free (tree->array[i]);

    free (tree->array);

    free (tree->table_var);
    free (tree->read_data);
    
    if (tree->output)
        fclose (tree->output);
}

void ReadDataBase (Tree_t* tree)
{
    assert (tree);

    size_t size = 0;
    tree->read_data = ReadFile (tree->input, &size);

    // creating tokens from buffer data
    tree->array = CreateArrayTokens ();
    ReadBuffer (tree);

    int pointer = 0;
    tree->expression = RecoveryTree (&pointer, tree->array);
}

Node_t** CreateArrayTokens ()
{
    Node_t** array = (Node_t**) calloc (SIZE_ARRAY, sizeof (Node_t*));
    assert (array);

    for (int i = 0; i < SIZE_ARRAY; i++)
    {
        array[i] = NodeCtor (0, 0, NULL, NULL);
        array[i]->left = NULL;
        array[i]->right = NULL;
        array[i]->type = INVALID_TYPE;
        array[i]->value.com = F_INVALID;
        array[i]->value.number = NAN;
        array[i]->value.var = -1;
    }

    return array;
}

void ReadBuffer (Tree_t* tree)
{
    assert (tree);

    Node_t** array = tree->array;
    int n_node = 0;

    char* buffer = tree->read_data;
    int position = 0;    

    while (buffer[position] != '\n')
    {
        while (buffer[position] == ' ' || buffer[position] == '\n')
            position++;

        if (isdigit (buffer[position]))
        {            
            double d = 0;
            int n = 0;
            sscanf (buffer + position, "%lf%n", &d, &n);
            position += n;
            array[n_node]->type = NUM;
            array[n_node]->value.number = d;
            n_node++;
        }
        else if (buffer[position] == '(')
        {
            array[n_node]->type = OP;
            array[n_node]->value.com = F_BRACE_OPEN;
            n_node++;
            position++;
        }
        else if (buffer[position] == ')')
        {
            array[n_node]->type = OP;
            array[n_node]->value.com = F_BRACE_CLOSE;
            n_node++;
            position++;
        }
        else if (buffer[position] == '_')
        {
            array[n_node]->type = OP;
            array[n_node]->value.com = F_UNDERLINING;
            n_node++;
            position++;
        }  
    }
    array[n_node]->value.var = '$';
    n_node++;

    while (buffer[position] != 0)
    {
        while (buffer[position] == ' ' || buffer[position] == '\n')
            position++;
        
        int n = 0;
        sscanf (buffer + position, "%*[a-zA-Z]%n", &n);
        buffer[position + n] = '\0';

        int i = 0;
        while (tree->table_var[i])
            i++;
        
        tree->table_var[i] = buffer + position;

        position += n + 1;
    }
}

Node_t* RecoveryTree (int* pointer, Node_t** array)
{
    assert (array);

    if (array[*pointer]->value.com == F_BRACE_OPEN)
    {
        (*pointer)++;

        Node_t* node = array[*pointer];
        node->type = (TypeCommand_t) array[*pointer]->value.number;
        (*pointer)++;

        if (node->type == NUM)
            node->value.number = array[*pointer]->value.number;
        else if (node->type == VAR || node->type == FUNC)
            node->value.var = (int) array[*pointer]->value.number;
        else if (node->type == OP || node->type == BLOCK)
            node->value.com = (ListCommand_t) array[*pointer]->value.number;

        (*pointer)++;

        node->left = RecoveryTree (pointer, array);
        node->right = RecoveryTree (pointer, array);

        if (array[*pointer]->value.com == F_BRACE_CLOSE)
        {
            (*pointer)++;
        }

        return node;
    }
    else
    {
        (*pointer)++;
        return NULL;
    }
}

