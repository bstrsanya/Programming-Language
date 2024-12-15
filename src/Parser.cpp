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

    // creating tokens from buffer data
    tree->array = CreateArrayTokens ();
    Tokenization (tree);

    // recursive descent
    int pointer = 0;
    tree->expression = GetG (&pointer, tree->array);
}

    // for (int i = 0; i < 20; i++)
    // {
    //     if (array[i]->type == NUM)
    //         printf ("[%d]: type [%d], value [%g]\n", i, array[i]->type, array[i]->value.number);
    //     else if (array[i]->type == VAR)
    //         printf ("[%d]: type [%d], value [%d]\n", i, array[i]->type, array[i]->value.var);
    //     else
    //         printf ("[%d]: type [%d], value [%d]\n", i, array[i]->type, array[i]->value.com);
    // }


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

    while (buffer[position] != 0)
    {
        while (buffer[position] == ' ' || buffer[position] == '\n')
            position++;

        if ((('0' <= buffer[position]) && (buffer[position] <= '9')) || (buffer[position] == '-' && (y == 0 || (int) array[y - 1]->value.com == F_BRACE_OPEN)))
        {            
            double d = 0;
            int n = 0;
            sscanf (buffer + position, "%lf%n", &d, &n);
            position += n;
            array[y]->type = NUM;
            array[y]->value.number = d;
            y++;
        }
        
        else if (('a' <= buffer[position] && buffer[position] <= 'z') || ('A' <= buffer[position] && buffer[position] <= 'Z'))
        {
            int n = 0;
            sscanf (buffer + position, "%*[a-zA-Z]%n", &n);

            if (buffer[position+n] != ' ' && buffer[position+n] != '\n')
            {
                printf ("Please observe the aesthetics of the code, namely, put spaces\n");
                assert (0);
            }
            buffer[position+n] = '\0';
            TypeCommand_t com_type;
            int com_value = 0;
            FindCommand (buffer+position, &com_type, &com_value, tree);

            array[y]->type = (TypeCommand_t) com_type;

            if (com_type == VAR)
                array[y]->value.var = com_value;
            else
                array[y]->value.com = (ListCommand_t) com_value;

            y++;
            position += n + 1;
        }
        else if (buffer[position] == '(' || 
                 buffer[position] == ')' || 
                 buffer[position] == '{' ||
                 buffer[position] == '}' ||
                 buffer[position] == '+' || 
                 buffer[position] == '-' || 
                 buffer[position] == '*' || 
                 buffer[position] == '/' || 
                 buffer[position] == '^' || 
                 buffer[position] == ';'  )
        {
            array[y]->type = OP;
            array[y]->value.com = (ListCommand_t) buffer[position];
            position++;
            y++;
        }

        else if (buffer[position] == '=')
        {
            position++;
            array[y]->type = OP;
            if (buffer[position] == '=')
            {
                array[y]->value.com = F_JE;
                position++;
            }
            else
                array[y]->value.com = F_ASSIGNMENT;

            y++;
        }

        else if (buffer[position] == '<')
        {
            position++;
            array[y]->type = OP;
            if (buffer[position] == '=')
            {
                array[y]->value.com = F_JBE;
                position++;
            }
            else
                array[y]->value.com = F_JB;

            y++;
        }

        else if (buffer[position] == '>')
        {
            position++;
            array[y]->type = OP;
            if (buffer[position] == '=')
            {
                array[y]->value.com = F_JAE;
                position++;
            }
            else
                array[y]->value.com = F_JA;

            y++;
        }

        else if (buffer[position] == '!')
        {
            position++;
            array[y]->type = OP;
            if (buffer[position] == '=')
            {
                array[y]->value.com = F_JNE;
                position++;
            }
            else
                printf ("AAAAAAAAA\n");

            y++;
        }
        
        else
        {
            printf ("ER - [%d]\n", buffer[position-1]);
        }
    }
    array[y]->value.var = '$';
}

void FindCommand (char* com, TypeCommand_t* com_type, int* com_value, Tree_t* tree)
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
        return GetV (pointer, array);
    }
}
