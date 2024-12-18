#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "Parser.h"

void ReadDataBase (Tree_t* tree)
{
    size_t size = 0;
    char* buffer = ReadFile (tree->input, &size);

    // creating tokens from buffer data
    tree->array = CreateArrayTokens ();
    Tokenization (tree, buffer);


    // for (int i = 0; i < 20; i++)
    // {
    //     if (tree->array[i]->type == NUM)
    //         printf ("[%d]: type [%d], value [%g]\n", i, tree->array[i]->type, tree->array[i]->value.number);
    //     else if (tree->array[i]->type == VAR)
    //         printf ("[%d]: type [%d], value [%d]\n", i, tree->array[i]->type, tree->array[i]->value.var);
    //     else
    //         printf ("[%d]: type [%d], value [%d]\n", i, tree->array[i]->type, tree->array[i]->value.com);
    // }


    // recursive descent
    int pointer = 0;
    tree->expression = GetG (&pointer, tree->array);
    free (buffer);
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

void Tokenization (Tree_t* tree, char* buffer)
{
    Node_t** array = tree->array;
    int y = 0;

    int position = 0;    

    while (buffer[position] != 0)
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
        else if (isalpha (buffer[position]))
        {
            int n = 0;
            sscanf (buffer + position, "%*[a-zA-Z123456789_]%n", &n);

            char* str = CreateStr (buffer + position, n, tree);
            TypeCommand_t com_type = INVALID_TYPE;
            int com_value = 0;
            FindCommand (str, &com_type, &com_value, tree);

            array[y]->type = (TypeCommand_t) com_type;

            if (com_type == VAR)
                array[y]->value.var = com_value;
            else
                array[y]->value.com = (ListCommand_t) com_value;

            y++;
            position += n;
        }
        else 
        {
            int n = 0;
            sscanf (buffer + position, "%*[+-*/^{};_]%n", &n);
            if (!n)
            {
                sscanf (buffer + position, "%*[=><!]%n", &n);
            }
            if (!n)
            {
                if (buffer[position] == '(' || buffer[position] == ')')
                    n = 1;
            }
            if (n) {
            char* str = CreateStr (buffer + position, n, tree);
            TypeCommand_t com_type = INVALID_TYPE;
            int com_value = 0;
            FindCommand (str, &com_type, &com_value, tree);

            array[y]->type = (TypeCommand_t) com_type;

            if (com_type == VAR)
                array[y]->value.var = com_value;
            else
                array[y]->value.com = (ListCommand_t) com_value;

            y++;
            position += n; }
        }
    }
    array[y]->value.var = '$';
}

char* CreateStr (char* main_str, int len, Tree_t* tree)
{
    static int number = 0;
    strncpy (tree->read_data + number, main_str, (size_t) len);
    number += len + 1;
    return tree->read_data + number - len - 1;    
}

void FindCommand (char* com, TypeCommand_t* com_type, int* com_value, Tree_t* tree)
{    
    for (int i = 0; i < NUM_COMMAND; i++)
    {
        if (!strcmp (array_command[i].name, com))
        {
            *com_type = OP;
            *com_value = array_command[i].n_com;
        }
    }
    if (!(*com_value))
    {
        *com_type = VAR;
        for (int i = 0; i < SIZE_TABLE_VAR; i++)
        {
            if (!tree->table_var[i])
            {
                *com_value = i;
                tree->table_var[i] = com;
                break;
            }
            else if (!(strcmp (tree->table_var[i], com)))
            {
                *com_value = i;
                break;
            }
        }
    }
}

