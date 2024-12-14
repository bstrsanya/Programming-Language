#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "Parser.h"
#include "General.h"

void ReadDataBase (Tree_t* tree)
{
    size_t size = 0;
    tree->read_data = ReadFile (tree->input, &size);

    Node_t** array = CreateTokens (tree);

    // for (int i = 0; i < 20; i++)
    // {
    //     if (array[i]->type == NUM)
    //         printf ("[%d]: type [%d], value [%g]\n", i, array[i]->type, array[i]->value.number);
    //     else if (array[i]->type == VAR)
    //         printf ("[%d]: type [%d], value [%d]\n", i, array[i]->type, array[i]->value.var);
    //     else
    //         printf ("[%d]: type [%d], value [%d]\n", i, array[i]->type, array[i]->value.com);
    // }

    int pointer = 0;

    Node_t* value = GetG (&pointer, array);
    
    tree->expression = value;

    tree->array = array;
}

Node_t** CreateTokens (Tree_t* tree)
{
    char* s = tree->read_data;
    int t = 0;
    Node_t** array = (Node_t**) calloc (SIZE_ARRAY, sizeof (Node_t*));
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

    int y = 0;

    while (s[t] != 0)
    {
        while (s[t] == ' ' || s[t] == '\n')
            t++;

        if ((('0' <= s[t]) && (s[t] <= '9')) || (s[t] == '-' && (y == 0 || (int) array[y - 1]->value.com == F_OPEN)))
        {            
            double d = 0;
            int n = 0;
            sscanf (s + t, "%lf%n", &d, &n);
            t += n;
            array[y]->type = NUM;
            array[y]->value.number = d;
            y++;

            while (s[t] == ' ' || s[t] == '\n')
            t++;
        }
        
        else if (('a' <= s[t] && s[t] <= 'z') || ('A' <= s[t] && s[t] <= 'Z'))
        {
            int n = 0;
            sscanf (s + t, "%*[a-zA-Z]%n", &n);

            if (s[t+n] != ' ' && s[t+n] != '\n')
            {
                printf ("Please observe the aesthetics of the code, namely, put spaces\n");
                assert (0);
            }
            s[t+n] = '\0';
            type_com com_type;
            int com_value = 0;
            FindCommand (s+t, &com_type, &com_value, tree);

            array[y]->type = (type_com) com_type;

            if (com_type == VAR)
                array[y]->value.var = com_value;
            else
                array[y]->value.com = (command) com_value;

            y++;
            t += n + 1;

            while (s[t] == ' ' || s[t] == '\n')
            t++;
        }
        else if (s[t] == '(' || 
                 s[t] == ')' || 
                 s[t] == '{' ||
                 s[t] == '}' ||
                 s[t] == '+' || 
                 s[t] == '-' || 
                 s[t] == '*' || 
                 s[t] == '/' || 
                 s[t] == '^' || 
                 s[t] == ';'  )
        {
            array[y]->type = OP;
            array[y]->value.com = (command) s[t];
            t++;
            y++;

            while (s[t] == ' ' || s[t] == '\n')
            t++;
        }

        else if (s[t] == '=')
        {
            t++;
            array[y]->type = OP;
            if (s[t] == '=')
            {
                array[y]->value.com = F_EQUAL;
                t++;
            }
            else
                array[y]->value.com = F_ASSIGNMENT;

            y++;
            while (s[t] == ' ' || s[t] == '\n')
                t++;
        }
        
        else
        {
            printf ("ER - [%d]\n", s[t-1]);
        }
    }
    array[y]->value.var = '$';
    return array;
}

void FindCommand (char* com, type_com* com_type, int* com_value, Tree_t* tree)
{    
    for (int i = 0; i < NUM_COMMAND; i++)
    {
        if (!strcmp (array_command[i].name, com))
        {
            *com_type = array_command[i].t_com;
            *com_value = array_command[i].n_com;
        }
    }
    if (!(*com_value))
    {
        *com_type = VAR;
        for (int i = 0; i < 10; i++)
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
    Node_t* value = GetFunc (pointer, array);
    if ((int) array[*pointer]->value.var != '$')
    {
        printf ("[%d]\n", *pointer);
        assert (0);
    }
    (*pointer)++;
    return value;
}

Node_t* GetFunc (int* pointer, Node_t** array)
{
    Node_t* value = array[*pointer];
    if (array[*pointer]->value.com == F_FUNC)
    {
        (*pointer)++;
        (*pointer)++;
        GetStop (pointer, array, value);
        (*pointer)++;
    }
    else
    {
        printf ("Программа должна начинаться с функции\n");
        assert (0);
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

        if (array[*pointer]->value.com != F_OPEN)
        {
            printf ("вместо [%d] нужно [(]\n", array[*pointer]->value.com);
            assert (0);
        }

        (*pointer)++;                                 // начало условия для if
        Node_t* value1 = GetEqu (pointer, array);

        if (array[*pointer]->value.com != F_CLOSE)
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

        if (array[*pointer]->value.com != F_OPEN)
        {
            printf ("вместо [%d] нужно [(]\n", array[*pointer]->value.com);
            assert (0);
        }

        (*pointer)++;                                 // начало условия для if
        Node_t* value1 = GetEqu (pointer, array);

        if (array[*pointer]->value.com != F_CLOSE)
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
        return GetEqu (pointer, array);
    }
}
