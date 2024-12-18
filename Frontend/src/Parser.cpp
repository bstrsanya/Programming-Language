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
            TypeCommand_t com_type;
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
            TypeCommand_t com_type;
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

Node_t* GetG (int* pointer, Node_t** array)
{
    Node_t* main_node = NodeCtor (BLOCK, 0, NULL, NULL);
    GetProg (pointer, array, main_node);
    if ((int) array[*pointer]->value.var != '$')
    {
        printf ("[%d]\n", *pointer);
        assert (0);
    }
    (*pointer)++;
    return main_node;
}

void GetProg (int* pointer, Node_t** array, Node_t* main_value)
{
    while (array[*pointer]->value.var != '$')
    {
        Node_t* value = GetFunc (pointer, array);
        Node_t* block = NodeCtor (BLOCK, 0, value, NULL);
        main_value->right = block;
        main_value = block;
        (*pointer)++;
    }
}

Node_t* GetFunc (int* pointer, Node_t** array)
{
    array[*pointer]->type = FUNC;
    Node_t* value = array[*pointer];
    while (array[(*pointer)+1]->value.com == F_BRACE_OPEN)
    {
        (*pointer)++;
        (*pointer)++;
        (*pointer)++;
        (*pointer)++;
        GetStop (pointer, array, value);
    }
    return value;
}

void GetStop (int* pointer, Node_t** array, Node_t* main_value)
{
    while (array[*pointer]->value.com != F_CURLY_BRACE_CLOSE)
    {
        Node_t* value = GetIf (pointer, array);

        if (array[(*pointer) - 1]->value.com != F_CURLY_BRACE_CLOSE &&
            array[(*pointer)]->value.com != F_INTERRUPT )
        {
            printf ("need [;]\n");
            assert(0);
        }
        if (array[*pointer]->value.com == F_INTERRUPT) (*pointer)++;

        Node_t* block = NodeCtor (BLOCK, 0, value, NULL);

        main_value->right = block;
        main_value = block;
    }
}

Node_t* GetIf (int* pointer, Node_t** array)
{
    if (array[*pointer]->value.com == F_IF)
    {
        Node_t* main_node = array[*pointer];
        (*pointer)++;

        if (array[*pointer]->value.com != F_BRACE_OPEN)
        {
            printf ("вместо [%d] нужно [(]\n", array[*pointer]->value.com);
            assert (0);
        }

        (*pointer)++;                                 // начало условия для if
        Node_t* value1 = GetEqu (pointer, array);

        if (array[*pointer]->value.com != F_BRACE_CLOSE)
        {
            printf ("вместо [%d] нужно [)]\n", array[*pointer]->value.com);
            assert (0);
        }

        (*pointer)++;                                 // конец условия для if

        if (array[*pointer]->value.com != F_CURLY_BRACE_OPEN)
        {
            printf ("вместо [%d] нужно [{]\n", array[*pointer]->value.com);
            assert (0);
        }

        (*pointer)++;                              // начало подифного выражения
        GetStop (pointer, array, main_node);

        if (array[*pointer]->value.com != F_CURLY_BRACE_CLOSE)
        {
            printf ("вместо [%d] нужно [}]\n", array[*pointer]->value.com);
            assert (0);
        }
        (*pointer)++;                              // конец подифного выражения

        int n_else = *pointer;
        if (array[*pointer]->value.com == F_ELSE)
        {
            (*pointer)++;
            if (array[*pointer]->value.com != F_CURLY_BRACE_OPEN)
            {
                printf ("вместо [%d] нужно [{]\n", array[*pointer]->value.com);
                assert (0);
            }
            (*pointer)++;

            GetStop (pointer, array, array[n_else]);

            if (array[*pointer]->value.com != F_CURLY_BRACE_CLOSE)
            {
                printf ("вместо [%d] нужно [}]\n", array[*pointer]->value.com);
                assert (0);
            }
            (*pointer)++;
        }

        if (array[n_else]->right)
        {
            main_node->left = value1;
            Node_t* copy_node = main_node->right;
            main_node->right = array[n_else];
            array[n_else]->left = copy_node;
        }

        main_node->left  = value1;
        return main_node;
    }
    else
    {
        return GetWhile (pointer, array);
    }     
}

Node_t* GetWhile (int* pointer, Node_t** array)
{
    if (array[*pointer]->value.com == F_WHILE)
    {
        Node_t* main_node = array[*pointer];
        (*pointer)++;

        if (array[*pointer]->value.com != F_BRACE_OPEN)
        {
            printf ("вместо [%d] нужно [(]\n", array[*pointer]->value.com);
            assert (0);
        }

        (*pointer)++;                                 // начало условия для if
        Node_t* value1 = GetEqu (pointer, array);

        if (array[*pointer]->value.com != F_BRACE_CLOSE)
        {
            printf ("вместо [%d] нужно [)]\n", array[*pointer]->value.com);
            assert (0);
        }

        (*pointer)++;                                 // конец условия для if

        if (array[*pointer]->value.com != F_CURLY_BRACE_OPEN)
        {
            printf ("вместо [%d] нужно [{]\n", array[*pointer]->value.com);
            assert (0);
        }

        (*pointer)++;                              // начало подифного выражения
        GetStop (pointer, array, main_node);

        if (array[*pointer]->value.com != F_CURLY_BRACE_CLOSE)
        {
            printf ("вместо [%d] нужно [}]\n", array[*pointer]->value.com);
            assert (0);
        }
        (*pointer)++;                              // конец подифного выражения

        main_node->left  = value1;
        return main_node;
    }
    else
    {
        return GetO (pointer, array);
    }
}

Node_t* GetO (int* pointer, Node_t** array)
{
    if (array[*pointer]->value.com == F_PRINT || array[*pointer]->value.com == F_INPUT)
    {
        Node_t* main_node = array[*pointer];
        (*pointer)++;

        if (array[*pointer]->value.com != F_BRACE_OPEN)
        {
            printf ("need [(]\n");
            assert (0);
        }
        (*pointer)++;
        
        Node_t* node = GetE (pointer, array);

        if (array[*pointer]->value.com != F_BRACE_CLOSE)
        {
            printf ("need [)]\n");
            assert (0);
        }
        (*pointer)++;

        main_node->right = node;
        return main_node;
    }
    return GetF (pointer, array);
}

Node_t* GetF (int* pointer, Node_t** array)
{
    if (array[(*pointer) + 1]->value.com == F_BRACE_OPEN)
    {
        array[*pointer]->type = FUNC;
        (*pointer)++;
        (*pointer)++;
        (*pointer)++;
        return array[(*pointer)-3];
    }
    
    return GetV (pointer, array);
}
