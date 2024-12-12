#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Parser.h"
#include "General.h"

void ReadDataBase (Tree_t* tree)
{
    size_t size = 0;
    char* s = ReadFile (tree->input, &size);

    Node_t** array = CreateTokens (s);

    for (int i = 0; i < 10; i++)
        printf ("[%d]: type [%d], value [%g]\n", i, array[i]->type, array[i]->value);

    int pointer = 0;
    // GetFunc (&pointer, array);
    Node_t* value = GetG (&pointer, array);
    
    tree->expression = value;

    tree->array = array;
    free (s);
}

Node_t** CreateTokens (char* s)
{
    int t = 0;
    Node_t** array = (Node_t**) calloc (SIZE_ARRAY, sizeof (Node_t*));
    for (int i = 0; i < SIZE_ARRAY; i++)
    {
        array[i] = NodeCtor (0, 0, NULL, NULL);
    }

    int y = 0;

    while (s[t] != 0)
    {
        while (s[t] == ' ' || s[t] == '\n')
        {
            t++;
        }
        if ((('0' <= s[t]) && (s[t] <= '9')) || (s[t] == '-' && (y == 0 || (int) array[y - 1]->value == F_OPEN)))
        {            
            double d = 0;
            int n = 0;
            sscanf (s + t, "%lf%n", &d, &n);
            t += n;
            array[y]->type = NUM;
            array[y]->value = d;
            y++;
        }
        else if (('a' <= s[t] && s[t] <= 'z') || ('A' <= s[t] && s[t] <= 'Z'))
        {
            char com[LEN_STR] = "";
            int n = 0;
            sscanf (s + t, "%[a-zA-Z]%n", com, &n);
            
            type_com com_type;
            int com_value = 0;
            FindCommand (com, &com_type, &com_value);

            array[y]->type = (type_com) com_type;
            array[y]->value = com_value;
            y++;
            t += n;
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
                 s[t] == '='  )
        {
            array[y]->type = OP;
            array[y]->value = s[t];
            t++;
            y++;
        }
        else
        {
            printf ("ER - [%d]\n", s[t-1]);
        }
    }
    array[y]->value = '$';
    return array;
}

void FindCommand (char* com, type_com* com_type, int* com_value)
{
    for (int i = 0; i < NUM_COMMAND; i++)
    {
        if (!strcmp (array_command[i].name, com))
        {
            *com_type = array_command[i].t_com;
            *com_value = array_command[i].n_com;
        }
    }
}

Node_t* GetG (int* pointer, Node_t** array)
{
    Node_t* value = GetFunc (pointer, array);
    if ((int) array[*pointer]->value != '$')
    {
        printf ("[%g]\n", array[*pointer]->value);
        assert (0);
    }
    (*pointer)++;
    return value;
}

Node_t* GetFunc (int* pointer, Node_t** array)
{
    Node_t* value = array[*pointer];
    if ((int) array[*pointer]->value == F_FUNC)
    {
        (*pointer)++;
        if ((int) array[*pointer]->value == F_CURLY_BRACE_OPEN)
        {
            (*pointer)++;
            value->right = GetEqu (pointer, array);
        }
        if ((int) array[*pointer]->value != F_CURLY_BRACE_CLOSE)
        {
            printf ("need [}]\n");
            assert (0);
        }
        else
            (*pointer)++;
    }
    else
    {
        printf ("Программа должна начинаться с функции\n");
        assert (0);
    }
    return value;
}
