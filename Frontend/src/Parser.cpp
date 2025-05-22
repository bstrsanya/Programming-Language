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

    // for (int i = 0; i < 10; i++)
    // {
    //     printf ("type: %d; com: %d; number: %.2lf; var: %d.\n", tree->array[i]->type, tree->array[i]->value.com, tree->array[i]->value.number, tree->array[i]->value.var);
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

int HAS_OPEN_BRACE = 0;
int I_GLOBAL = 0;

void Tokenization (Tree_t* tree, char* buffer)
{
    Node_t** array = tree->array;
    int n_node = 0;

    int position = 0;    

    while (buffer[position] != 0)
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

        else 
        {
            int n = 0;
            if (!n) sscanf (buffer + position, "%*[a-zA-Z1234567890_]%n", &n);
            if (!n) sscanf (buffer + position, "%*[+-*/^;_]%n", &n);
            if (!n) sscanf (buffer + position, "%*[=><!]%n", &n);
            if (!n) sscanf (buffer + position, "%*1[()]%n", &n);

            if (n)
            {
                char* str = CreateStr (buffer + position, n, tree);
                TypeCommand_t com_type = INVALID_TYPE;
                int com_value = 0;
                FindCommand (str, &com_type, &com_value, tree);

                array[n_node]->type = (TypeCommand_t) com_type;

                if (com_type == VAR)
                    array[n_node]->value.var = com_value;
                else
                    array[n_node]->value.com = (ListCommand_t) com_value;

                n_node++;
                position += n;
            }
        }
    }
    array[n_node]->value.var = '$';
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
            if (array_command[i].n_com == F_CURLY_BRACE_OPEN ||
                array_command[i].n_com == F_BRACE_OPEN)
            {
                HAS_OPEN_BRACE += 1;
            }
            if (array_command[i].n_com == F_CURLY_BRACE_CLOSE ||
                array_command[i].n_com == F_BRACE_CLOSE)
            {
                HAS_OPEN_BRACE -= 1;
            }
            *com_value = array_command[i].n_com;
        }
    }
    if (!(*com_value))
    {
        // printf ("[%s]\n", com);
        // printf ("[%s] [%d] [%d]\n", com, HAS_OPEN_BRACE, HAS_OPEN_BRACE_2);
        if (HAS_OPEN_BRACE == 0)
        {
            for (int j = 0; j < SIZE_TABLE_VAR; j++)
            {
                if (!tree->table_var[j])
                {
                    I_GLOBAL = j;
                    // printf ("I_GLOBAL = %d\n", I_GLOBAL);
                    tree->table_var[j] = "-----NEWFUNC-----";
                    I_GLOBAL++;
                    break;
                }
            }
        }
        *com_type = VAR;
        for (int i = I_GLOBAL; i < SIZE_TABLE_VAR; i++)
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

