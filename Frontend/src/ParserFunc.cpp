#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Parser.h"

#define SHIFT_CUR (*pointer)++;
#define VAL_COM_CUR array[*pointer]->value.com

#define CHECK(val)                                                                    \
    if (VAL_COM_CUR != val)                                                           \
    {                                                                                 \
        printf ("need [%s] instead of [%s]\n", FindStr (val), FindStr (VAL_COM_CUR)); \
        exit (0);                                                                     \
    }

const char* FindStr (ListCommand_t value)
{
    int i = 0;
    while (i < NUM_COMMAND)
    {
        if (value == array_command[i].n_com)
            return array_command[i].name;

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
    
    SHIFT_CUR

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
        SHIFT_CUR              // peresda
    }
}

Node_t* GetDefinitionFunc (int* pointer, Node_t** array)
{
    array[*pointer]->type = FUNC;
    Node_t* value = array[*pointer];
    while (array[(*pointer)+1]->value.com == F_BRACE_OPEN)
    {
        SHIFT_CUR              // <name_func>
        SHIFT_CUR              // (
        GetArgFunc (pointer, array, value);
        SHIFT_CUR              // )
        SHIFT_CUR              // udos
        GetStr (pointer, array, value);
    }
    
    return value;
}

void GetArgFunc (int* pointer, Node_t** array, Node_t* main_value)
{
    while (VAL_COM_CUR != F_BRACE_CLOSE)
    {
        Node_t* value = GetN (pointer, array);

        // if (array[(*pointer) - 1]->value.com != F_CURLY_BRACE_CLOSE &&
        //     array[(*pointer)]->value.com != ',' )
        // {
        //     printf ("need [,]\n");
        //     assert(0);
        // }

        if (VAL_COM_CUR == F_INTERRUPT) SHIFT_CUR

        Node_t* block = NodeCtor (BLOCK, 0, NULL, value);

        main_value->left = block;
        main_value = block;
    }
}

void GetStr (int* pointer, Node_t** array, Node_t* main_value)
{
    while (VAL_COM_CUR != F_CURLY_BRACE_CLOSE)
    {
        Node_t* value = GetVariants (pointer, array);

        if (array[(*pointer) - 1]->value.com != F_CURLY_BRACE_CLOSE &&
            array[(*pointer)]->value.com != F_INTERRUPT )
        {
            printf ("need [;] %d %d\n", array[(*pointer) - 1]->value.com, array[(*pointer)]->value.com);
            assert(0);
        }

        if (VAL_COM_CUR == F_INTERRUPT) SHIFT_CUR

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
    if ((node = GetReturn     (pointer, array))) return node;
    if ((node = GetAssignment (pointer, array))) return node;

    return NULL; 
}

Node_t* GetReturn (int* pointer, Node_t** array)
{
    if (VAL_COM_CUR == F_RETURN)
    {
        Node_t* main_node = array[*pointer];
        SHIFT_CUR

        Node_t* value = GetE (pointer, array);

        main_node->right = value;

        return main_node;
    }

    return NULL;
}

Node_t* GetIf (int* pointer, Node_t** array)
{
    if (VAL_COM_CUR == F_IF)
    {
        Node_t* main_node = array[*pointer];
        SHIFT_CUR

        CHECK (F_BRACE_OPEN);
        SHIFT_CUR

        Node_t* value1 = GetCompare (pointer, array);

        CHECK (F_BRACE_CLOSE);
        SHIFT_CUR                                 

        CHECK (F_CURLY_BRACE_OPEN);
        SHIFT_CUR         

        GetStr (pointer, array, main_node);

        CHECK (F_CURLY_BRACE_CLOSE);
        SHIFT_CUR                              

        int n_else = *pointer;
        if (VAL_COM_CUR == F_ELSE)
        {
            SHIFT_CUR

            CHECK (F_CURLY_BRACE_OPEN);
            SHIFT_CUR

            GetStr (pointer, array, array[n_else]);

            CHECK (F_CURLY_BRACE_CLOSE);
            SHIFT_CUR
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
    if (VAL_COM_CUR == F_WHILE)
    {
        Node_t* main_node = array[*pointer];
        SHIFT_CUR

        CHECK (F_BRACE_OPEN);
        SHIFT_CUR                                

        Node_t* value1 = GetCompare (pointer, array);

        CHECK (F_BRACE_CLOSE);
        SHIFT_CUR       

        CHECK (F_CURLY_BRACE_OPEN);
        SHIFT_CUR                              

        GetStr (pointer, array, main_node);

        CHECK (F_CURLY_BRACE_CLOSE);
        SHIFT_CUR                             

        main_node->left  = value1;
        return main_node;
    }
        
    return NULL;
}

Node_t* GetFuncCall (int* pointer, Node_t** array)
{
    if (VAL_COM_CUR != F_BRACE_OPEN && array[(*pointer)+1]->value.com == F_BRACE_OPEN)
    {
        Node_t* main_node = array[*pointer];
        if (main_node->type == VAR)
            main_node->type = FUNC;

        SHIFT_CUR         // <name_func>
        SHIFT_CUR         // F_BRACE_OPEN
        
        if (main_node->value.com == F_INPUT || main_node->value.com == F_PRINT)
        {
            Node_t* node = GetE (pointer, array);

            CHECK (F_BRACE_CLOSE);

            main_node->right = node;
        }
        else
        {
            Node_t* copy = main_node;
            while (VAL_COM_CUR != F_BRACE_CLOSE)
            {
                Node_t* value = GetE (pointer, array);

                if (VAL_COM_CUR == F_INTERRUPT) SHIFT_CUR

                Node_t* block = NodeCtor (BLOCK, 0, value, NULL);

                copy->right = block;
                copy = block;
            }
        }
        
        SHIFT_CUR        // F_BRACE_CLOSE

        return main_node;
    }

    return NULL;
}

Node_t* GetAssignment (int* pointer, Node_t** array)
{
    Node_t* main_node = NULL;

    if (VAL_COM_CUR == F_INT || VAL_COM_CUR == F_DOUBLE)
    {
        main_node = array[*pointer];
        SHIFT_CUR
    }

    Node_t* node_left = GetN (pointer, array);

    if (VAL_COM_CUR != F_ASSIGNMENT)
    {
        printf ("YOU MUST INITIALIZE THE VARIABLE\n");
        assert (0);
    }

    Node_t* node = array[*pointer];
    SHIFT_CUR

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

    if (array[*pointer]->type == OP && (VAL_COM_CUR == F_JE  ||
                                        VAL_COM_CUR == F_JB  ||
                                        VAL_COM_CUR == F_JA  ||
                                        VAL_COM_CUR == F_JBE ||
                                        VAL_COM_CUR == F_JAE ||
                                        VAL_COM_CUR == F_JNE ))
    {
        int num = *pointer;
        SHIFT_CUR
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

    while (array[*pointer]->type == OP && (VAL_COM_CUR == F_ADD || VAL_COM_CUR == F_SUB))
    {
        int num = *pointer;
        SHIFT_CUR
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

    while (array[*pointer]->type == OP && (VAL_COM_CUR == F_MUL || VAL_COM_CUR == F_DIV))
    {
        int num = *pointer;
        SHIFT_CUR
        Node_t* value2 = GetP (pointer, array);
        array[num]->left = value;
        array[num]->right = value2;
        value = array[num];
    }

    return value;
}

Node_t* GetP (int* pointer, Node_t** array)
{
    if (array[*pointer]->type == OP && VAL_COM_CUR == F_BRACE_OPEN)
    {
        SHIFT_CUR
        Node_t* value = GetE (pointer, array);

        CHECK (F_BRACE_CLOSE);
        SHIFT_CUR

        return value;
    }
    else
        return GetN (pointer, array);
}

Node_t* GetN (int* pointer, Node_t** array)
{
    Node_t* node = GetFuncCall (pointer, array);
    if (node)
    {
        return node;
    }
    if (array[*pointer]->type != NUM && array[*pointer]->type != VAR)
    {
        printf ("need num or var\n");
        assert (0);
    }

    SHIFT_CUR
    return array[(*pointer)-1];
}