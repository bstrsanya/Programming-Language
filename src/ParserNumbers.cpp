#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Parser.h"
#include "General.h"

Node_t* GetN (int* pointer, Node_t** array)
{
    (*pointer)++;
    return array[(*pointer)-1];
}

Node_t* GetE (int* pointer, Node_t** array)
{
    Node_t* value = GetT (pointer, array);

    while (array[*pointer]->type == OP && (array[*pointer]->value.com == F_ADD || array[*pointer]->value.com == F_SUB))
    {
        int num = *pointer;
        (*pointer)++;
        Node_t* value2 = GetT (pointer, array);
        array[num]->left = value;
        array[num]->right = value2;
        value = array[num];
    }
    return value;
}

Node_t* GetT (int* pointer, Node_t** array)
{
    Node_t* value = GetS (pointer, array);

    while (array[*pointer]->type == OP && (array[*pointer]->value.com == F_MUL || array[*pointer]->value.com == F_DIV))
    {
        int num = *pointer;
        (*pointer)++;
        Node_t* value2 = GetS (pointer, array);
        array[num]->left = value;
        array[num]->right = value2;
        value = array[num];
    }

    return value;
}


Node_t* GetS (int* pointer, Node_t** array)
{
    Node_t* value = GetP (pointer, array);

    if (array[*pointer]->type == OP && array[*pointer]->value.com == F_DEG)
    {
        int num = *pointer;
        (*pointer)++;
        Node_t* value2 = GetP (pointer, array);
        array[num]->left = value;
        array[num]->right = value2;
        value = array[num];
    }

    return value;
}

Node_t* GetEqu (int* pointer, Node_t** array)
{
    Node_t* value = GetE (pointer, array);

    if (array[*pointer]->type == OP && (array[*pointer]->value.com == F_ASSIGNMENT ||
                                        array[*pointer]->value.com == F_EQUAL))
    {
        int num = *pointer;
        (*pointer)++;
        Node_t* value2 = GetE (pointer, array);
        array[num]->left = value;
        array[num]->right = value2;
        value = array[num];
    }

    return value;
}


Node_t* GetP (int* pointer, Node_t** array)
{
    if (array[*pointer]->type == OP && array[*pointer]->value.com == F_BRACE_OPEN)
    {
        (*pointer)++;
        Node_t* value = GetE (pointer, array);
        if (array[*pointer]->value.com != F_BRACE_CLOSE)
        {
            printf ("вместо [%d] должна быть [)]\n", array[*pointer]->value.com);
            assert (0);
        }
        (*pointer)++;
        return value;
    }
    else if (array[*pointer]->type == OP && array[*pointer]->value.com == F_COS)
    {
        int num = *pointer;
        (*pointer)++;
        Node_t* value = GetP (pointer, array);
        array[num]->right = value;
        return array[num];
    }
    else if (array[*pointer]->type == OP && array[*pointer]->value.com == F_SIN)
    {
        int num = *pointer;
        (*pointer)++;
        Node_t* value = GetP (pointer, array);
        array[num]->right = value;
        return array[num];
    }
    else if (array[*pointer]->type == OP && array[*pointer]->value.com == F_LN)
    {
        int num = *pointer;
        (*pointer)++;
        Node_t* value = GetP (pointer, array);
        array[num]->right = value;
        return array[num];
    }
    else if (array[*pointer]->type == OP && array[*pointer]->value.com == F_TAN)
    {
        int num = *pointer;
        (*pointer)++;
        Node_t* value = GetP (pointer, array);
        array[num]->right = value;
        return array[num];
    }
    else if (array[*pointer]->type == OP && array[*pointer]->value.com == F_CTG)
    {
        int num = *pointer;
        (*pointer)++;
        Node_t* value = GetP (pointer, array);
        array[num]->right = value;
        return array[num];
    }
    else
        return GetN (pointer, array);
}