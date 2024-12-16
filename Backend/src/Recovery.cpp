#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "Recovery.h"

void TreeCtor (Tree_t* tree, const char* name_file)
{
    assert (tree);
    assert (name_file);

    tree->input = fopen (name_file, "rb");

    char** table = (char**) calloc (10, sizeof (char*));
    assert (table);
    for (int i = 0; i < 10; i++)
        table[i] = NULL;
    
    table[0] = "main";
    table[1] = "factorial";
    table[2] = "n";

    tree->table_var = table;

    ReadDataBase (tree);
    fclose (tree->input);

    tree->output = fopen (ASM_FILE, "wb");
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

void NodeDtor (Node_t* node)
{
    if (!node) return;
    
    if (node->left) NodeDtor (node->left);
    if (node->right) NodeDtor (node->right);

}

void TreeDtor (Tree_t* tree)
{
    assert (tree);

    // NodeDtor (tree->array[0]);

    for (int i = 0; i < SIZE_ARRAY; i++)
    {
        if (tree->array[i])
            free (tree->array[i]);
    }

    free (tree->array);
    // NodeDtor (tree->expression_diff);

    // free (tree->table_var);
    free (tree->read_data);
    free (tree->table_var);

    if (tree->output)
        fclose (tree->output);
}

void ReadDataBase (Tree_t* tree)
{
    size_t size = 0;
    tree->read_data = ReadFile (tree->input, &size);

    // creating tokens from buffer data
    tree->array = CreateArrayTokens ();
    Tokenization (tree);

    int pointer = 0;
    tree->expression = GetP (&pointer, tree->array);
    // for (int i = 0; i < 20; i++)
    // {
    //     if (tree->array[i]->type == NUM)
    //         printf ("[%d]: type [%d], value [%g]\n", i, tree->array[i]->type, tree->array[i]->value.number);
    //     else if (tree->array[i]->type == VAR)
    //         printf ("[%d]: type [%d], value [%d]\n", i, tree->array[i]->type, tree->array[i]->value.var);
    //     else
    //         printf ("[%d]: type [%d], value [%d]\n", i, tree->array[i]->type, tree->array[i]->value.com);
    // }
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

void Tokenization (Tree_t* tree)
{
    Node_t** array = tree->array;
    int y = 0;

    char* buffer = tree->read_data;
    int position = 0;    

    while (buffer[position] != '\n')
    {
        while (buffer[position] == ' ' || buffer[position] == '\n')
            position++;

        if (('0' <= buffer[position]) && (buffer[position] <= '9'))
        {            
            double d = 0;
            int n = 0;
            sscanf (buffer + position, "%lf%n", &d, &n);
            position += n;
            array[y]->type = NUM;
            array[y]->value.number = d;
            y++;
        }
        else if (buffer[position] == '(')
        {
            array[y]->type = OP;
            array[y]->value.com = F_BRACE_OPEN;
            y++;
            position++;
        }
        else if (buffer[position] == ')')
        {
            array[y]->type = OP;
            array[y]->value.com = F_BRACE_CLOSE;
            y++;
            position++;
        }
        else if (buffer[position] == '_')
        {
            array[y]->type = OP;
            array[y]->value.com = F_UNDERLINING;
            y++;
            position++;
        }  
    }
    array[y]->value.var = '$';
}

Node_t* GetP (int* pointer, Node_t** array)
{
    if (array[*pointer]->value.com == F_BRACE_OPEN)
    {
        (*pointer)++;
        Node_t* node = array[*pointer];
        node->type = (TypeCommand_t) array[*pointer]->value.number;
        (*pointer)++;
        if (node->type == NUM)
            node->value.number = array[*pointer]->value.number;
        else if (node->type == VAR)
            node->value.var = (int) array[*pointer]->value.number;
        else if (node->type == OP)
            node->value.com = (ListCommand_t) array[*pointer]->value.number;
        else if (node->type == FUNC)
            node->value.var = (int) array[*pointer]->value.number;
        else if (node->type == BLOCK)
            node->value.com = (ListCommand_t) array[*pointer]->value.number;
        (*pointer)++;

        node->left = GetP (pointer, array);
        node->right = GetP (pointer, array);

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





