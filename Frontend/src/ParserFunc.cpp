#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Parser.h"

#define CHECK(val) \
    if (array[*pointer]->value.com != val) \
    {\
        printf ("need [%s] instead of [%s]\n", FindStr (val), FindStr (array[*pointer]->value.com)); \
        exit (0); \
    }

const char* FindStr (ListCommand_t value)
{
    int i = 0;
    while (i < NUM_COMMAND)
    {
        if (value == array_command[i].n_com)
        {
            return array_command[i].name;
        }
        i++;
    }
    return array_command[0].name;        
}

Node_t* GetG (int* pointer, Node_t** array)
{
    Node_t* main_node = NodeCtor (BLOCK, 0, NULL, NULL);
    GetProgram (pointer, array, main_node);
    if ((int) array[*pointer]->value.var != '$')
    {
        printf ("[%d]\n", *pointer);
        assert (0);
    }
    (*pointer)++;
    return main_node;
}

void GetProgram (int* pointer, Node_t** array, Node_t* main_value)
{
    while (array[*pointer]->value.var != '$')
    {
        Node_t* value = GetDefinitionFunc (pointer, array);
        Node_t* block = NodeCtor (BLOCK, 0, value, NULL);
        main_value->right = block;
        main_value = block;
        (*pointer)++;
    }
}

Node_t* GetDefinitionFunc (int* pointer, Node_t** array)
{
    array[*pointer]->type = FUNC;
    Node_t* value = array[*pointer];
    while (array[(*pointer)+1]->value.com == F_BRACE_OPEN)
    {
        (*pointer)++;
        (*pointer)++;
        (*pointer)++;
        (*pointer)++;
        GetStr (pointer, array, value);
    }
    return value;
}

void GetStr (int* pointer, Node_t** array, Node_t* main_value)
{
    while (array[*pointer]->value.com != F_CURLY_BRACE_CLOSE)
    {
        Node_t* value = GetVariants (pointer, array);

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

Node_t* GetVariants (int* pointer, Node_t** array)
{
    Node_t* node = NULL;

    if ((node = GetIf         (pointer, array))) return node;
    if ((node = GetWhile      (pointer, array))) return node;
    if ((node = GetFuncCall   (pointer, array))) return node;
    if ((node = GetAssignment (pointer, array))) return node;

    return NULL; 
}

Node_t* GetIf (int* pointer, Node_t** array)
{
    if (array[*pointer]->value.com == F_IF)
    {
        Node_t* main_node = array[*pointer];
        (*pointer)++;

        CHECK (F_BRACE_OPEN);
        (*pointer)++;   

        Node_t* value1 = GetCompare (pointer, array);

        CHECK (F_BRACE_CLOSE);
        (*pointer)++;                                 

        CHECK (F_CURLY_BRACE_OPEN);
        (*pointer)++;         

        GetStr (pointer, array, main_node);

        CHECK (F_CURLY_BRACE_CLOSE);
        (*pointer)++;                              

        int n_else = *pointer;
        if (array[*pointer]->value.com == F_ELSE)
        {
            (*pointer)++;

            CHECK (F_CURLY_BRACE_OPEN);
            (*pointer)++;

            GetStr (pointer, array, array[n_else]);

            CHECK (F_CURLY_BRACE_CLOSE);
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
        return NULL;
    }     
}

Node_t* GetWhile (int* pointer, Node_t** array)
{
    if (array[*pointer]->value.com == F_WHILE)
    {
        Node_t* main_node = array[*pointer];
        (*pointer)++;

        CHECK (F_BRACE_OPEN);
        (*pointer)++;                                

        Node_t* value1 = GetCompare (pointer, array);

        CHECK (F_BRACE_CLOSE);
        (*pointer)++;       

        CHECK (F_CURLY_BRACE_OPEN);
        (*pointer)++;                              

        GetStr (pointer, array, main_node);

        CHECK (F_CURLY_BRACE_CLOSE);
        (*pointer)++;                             

        main_node->left  = value1;
        return main_node;
    }
        
    return NULL;
}

Node_t* GetFuncCall (int* pointer, Node_t** array)
{
    if (array[*pointer]->value.com != F_BRACE_OPEN && array[(*pointer)+1]->value.com == F_BRACE_OPEN)
    {
        Node_t* main_node = array[*pointer];
        if (main_node->type == VAR)
            main_node->type = FUNC;

        (*pointer)++;         // <name_func>
        (*pointer)++;         // F_BRACE_OPEN
        
        if (array[*pointer]->value.com != F_BRACE_CLOSE)
        {
            Node_t* node = GetE (pointer, array);

            CHECK (F_BRACE_CLOSE);

            main_node->right = node;
        }
        
        (*pointer)++;        // F_BRACE_CLOSE

        return main_node;
    }

    return NULL;
}

Node_t* GetAssignment (int* pointer, Node_t** array)
{
    Node_t* main_node = NULL;

    if (array[*pointer]->value.com == F_INT || array[*pointer]->value.com == F_DOUBLE)
    {
        main_node = array[*pointer];
        (*pointer)++;
    }

    Node_t* node_left = GetN (pointer, array);

    if (array[*pointer]->value.com != F_ASSIGNMENT)
    {
        printf ("YOU MUST INITIALIZE THE VARIABLE\n");
        assert (0);
    }

    Node_t* node = array[*pointer];
    (*pointer)++;

    Node_t* node_right = NULL;

    if (!(node_right = GetFuncCall (pointer, array)))
        node_right = GetCompare (pointer, array);

    node->left = node_left;
    node->right = node_right;

    if (main_node)
    {
        main_node->right = node;

        return main_node;
    }

    return node;        
}

Node_t* GetCompare (int* pointer, Node_t** array)
{
    Node_t* value = GetE (pointer, array);

    if (array[*pointer]->type == OP && (array[*pointer]->value.com == F_JE  ||
                                        array[*pointer]->value.com == F_JB  ||
                                        array[*pointer]->value.com == F_JA  ||
                                        array[*pointer]->value.com == F_JBE ||
                                        array[*pointer]->value.com == F_JAE ||
                                        array[*pointer]->value.com == F_JNE ))
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
    Node_t* value = GetP (pointer, array);

    while (array[*pointer]->type == OP && (array[*pointer]->value.com == F_MUL || array[*pointer]->value.com == F_DIV))
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

Node_t* GetP (int* pointer, Node_t** array)
{
    if (array[*pointer]->type == OP && array[*pointer]->value.com == F_BRACE_OPEN)
    {
        (*pointer)++;
        Node_t* value = GetE (pointer, array);

        CHECK (F_BRACE_CLOSE);
        (*pointer)++;

        return value;
    }
    else
        return GetN (pointer, array);
}

Node_t* GetN (int* pointer, Node_t** array)
{
    if (array[*pointer]->type != NUM && array[*pointer]->type != VAR)
    {
        printf ("need num or var\n");
        assert (0);
    }

    (*pointer)++;
    return array[(*pointer)-1];
}








