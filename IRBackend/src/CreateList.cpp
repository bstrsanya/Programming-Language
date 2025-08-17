#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "Elf64.h"
#include "header-IR.h"

void CreateIR (Tree_t* tree, ListIR_t* list_ir)
{
    assert (tree);
    assert (tree->output);
    assert (tree->expression);

    Node_IR* array = list_ir->list;
    array[list_ir->size_list].num = IR_START;
    (list_ir->size_list)++;
    array[list_ir->size_list].num = IR_CALL_FUNC;
    array[list_ir->size_list].arg1.num_str = "main"; 
    (list_ir->size_list)++;
    array[list_ir->size_list].num = IR_CALL_FUNC;
    array[list_ir->size_list].arg1.num_str = "HLT"; 
    (list_ir->size_list)++;
    // fprintf (tree->output, "START\n");
    // fprintf (tree->output, "CALL_FUNC {main}\nCALL_FUNC {HLT}\n");
    WritingIR (tree->expression, tree, list_ir);
    array[(list_ir->size_list)++].num = IR_END;
    // fprintf (tree->output, "END\n");
}

int IN_FUNC = 1;
int IN_ARG_FUNC = 1;
int IN_RETURN = 0;
int NUM_FUNC = 0;
int CALLING_FUNC = 0;
int POINTER_LABEL = 0;

void WritingIR (Node_t* node, Tree_t* tree, ListIR_t* list_ir)
{
    assert (node);
    assert (tree);

    switch (node->type)
    {
        case INVALID_TYPE:
        {
            fprintf (tree->output, "df\n");
            break;
        }
        case FUNC:
        {
            if (IN_FUNC == 0)
            {   
                // int i = node->value.var + 1;
                // while (tree->table_var[i] && NUM_FUNC == node->value.var)
                // {
                //     fprintf (tree->output, "SAVE {[%d]}\n", i - NUM_FUNC);
                //     i++;
                // }

                CALLING_FUNC = 1;
                if (node->left) WritingIR (node->left, tree, list_ir);
                if (node->right) WritingIR (node->right, tree, list_ir);
                list_ir->list[list_ir->size_list].num = IR_CALL_FUNC;
                list_ir->list[list_ir->size_list].arg1.num_str = tree->table_var[node->value.var];
                (list_ir->size_list)++;
                // fprintf (tree->output, "CALL_FUNC {%s}\n", tree->table_var[node->value.var]);
                // fprintf (tree->output, "FREE_ARG\n");
                list_ir->list[list_ir->size_list].num = IR_FREE_ARG;
                (list_ir->size_list)++;
                CALLING_FUNC = 0;
                // i--;
                // while (i > node->value.var && NUM_FUNC == node->value.var)
                // {
                //     fprintf (tree->output, "RECOVERY {[%d]}\n", i - NUM_FUNC);
                //     i--;
                // }
            }
            else
            {
                NUM_FUNC = node->value.var;
                int y = NUM_FUNC;
                while (tree->table_var[y] && tree->table_var[y][0] != '-')
                    y++;
                list_ir->list[list_ir->size_list].num = IR_BEGIN_FUNC_DEF;
                list_ir->list[list_ir->size_list].arg1.num_str = tree->table_var[node->value.var];
                list_ir->list[list_ir->size_list].arg2.num_int = y - NUM_FUNC - 1;
                (list_ir->size_list)++;
                // fprintf (tree->output, "BEGIN_FUNC_DEF {%s} {%d}\n", tree->table_var[node->value.var], y - NUM_FUNC - 1);
                IN_FUNC = 0;
                IN_ARG_FUNC = 0;
                if (node->left) WritingIR (node->left, tree, list_ir);
                IN_ARG_FUNC = 1;
                if (node->right) WritingIR (node->right, tree, list_ir);
                IN_FUNC = 1;
                list_ir->list[(list_ir->size_list)++].num = IR_RET;
                list_ir->list[(list_ir->size_list)++].num = IR_END_FUNC_DEF;
                // fprintf (tree->output, "RET\nEND_FUNC_DEF\n");
            }
            break;
        }
        case NUM:
        {
            list_ir->list[list_ir->size_list].num = IR_MOV;
            list_ir->list[list_ir->size_list].arg1.num_int = (int) node->value.number;
            list_ir->list[list_ir->size_list].arg2.stack = 1;
            (list_ir->size_list)++;

            // fprintf (tree->output, "MOV {%g} {stack}\n", node->value.number);
            break;
        }
        case VAR:
        {
            if (IN_ARG_FUNC == 0)
            {
                list_ir->list[list_ir->size_list].num = IR_GET_ARG;
                list_ir->list[list_ir->size_list].arg1.stack = 1;
                list_ir->list[list_ir->size_list].arg2.num_int = node->value.var - NUM_FUNC;
                list_ir->list[list_ir->size_list].arg2.memory = 1;
                (list_ir->size_list)++;
                // fprintf (tree->output, "GET_ARG {stack} {[%d]}\n", node->value.var - NUM_FUNC);
            }
            else if (CALLING_FUNC == 1)
            {
                list_ir->list[list_ir->size_list].num = IR_PUT_ARG_FUNC;
                list_ir->list[list_ir->size_list].arg1.num_int = node->value.var - NUM_FUNC;
                list_ir->list[list_ir->size_list].arg1.memory = 1;
                (list_ir->size_list)++;
                // fprintf (tree->output, "PUT_ARG_FUNC {[%d]}\n", node->value.var - NUM_FUNC);
            }
            else
            {
                list_ir->list[list_ir->size_list].num = IR_MOV;
                list_ir->list[list_ir->size_list].arg1.num_int = node->value.var - NUM_FUNC;
                list_ir->list[list_ir->size_list].arg1.memory = 1;
                list_ir->list[list_ir->size_list].arg2.stack = 1;
                (list_ir->size_list)++;
                // fprintf (tree->output, "MOV {[%d]} {stack}\n", node->value.var - NUM_FUNC);
            }
            break;
        }
        case BLOCK:
        {
            if (node->left) WritingIR (node->left, tree, list_ir);
            if (node->right) WritingIR (node->right, tree, list_ir);
            break;
        }
        case OP:
        {
            switch ((int) node->value.com)
            {
                case F_INTERRUPT:
                {
                    if (node->left) WritingIR (node->left, tree, list_ir);
                    if (node->right) WritingIR (node->right, tree, list_ir);
                    break;
                }
                case F_INT:
                case F_DOUBLE:
                {
                    WritingIR (node->right, tree, list_ir);
                    break;
                }
                case F_ASSIGNMENT:
                {
                    WritingIR (node->right, tree, list_ir);

                    if (node->right->type == FUNC)
                    {
                        list_ir->list[list_ir->size_list].num = IR_MOV;
                        list_ir->list[list_ir->size_list].arg1.reg = 1;
                        list_ir->list[list_ir->size_list].arg2.stack = 1;
                        (list_ir->size_list)++;
                        // fprintf (tree->output, "MOV {AX} {stack}\n");
                    }

                    list_ir->list[list_ir->size_list].num = IR_MOV;
                    list_ir->list[list_ir->size_list].arg1.stack = 1;
                    list_ir->list[list_ir->size_list].arg2.memory = 1;
                    list_ir->list[list_ir->size_list].arg2.num_int = node->left->value.var - NUM_FUNC;
                    (list_ir->size_list)++;
                    // fprintf (tree->output, "MOV {stack} {[%d]}\n", node->left->value.var - NUM_FUNC);
                    break;
                }
                case F_ADD:
                {
                    WritingIR (node->left, tree, list_ir);
                    WritingIR (node->right, tree, list_ir);

                    if (node->right->type == FUNC)
                    {
                        list_ir->list[list_ir->size_list].num = IR_MOV;
                        list_ir->list[list_ir->size_list].arg1.reg = 1;
                        list_ir->list[list_ir->size_list].arg2.stack = 1;
                        (list_ir->size_list)++;
                        // fprintf (tree->output, "MOV {AX} {stack}\n");
                    }

                    list_ir->list[list_ir->size_list].num = IR_MATH_OPER;
                    list_ir->list[list_ir->size_list].arg1.num_int = OP_ADD;
                    (list_ir->size_list)++;
                    // fprintf (tree->output, "MATH_OPER {ADD}\n");
                    break;
                }
                case F_SUB:
                {
                    WritingIR (node->left, tree, list_ir);
                    WritingIR (node->right, tree, list_ir);

                    if (node->right->type == FUNC)
                    {
                        list_ir->list[list_ir->size_list].num = IR_MOV;
                        list_ir->list[list_ir->size_list].arg1.reg = 1;
                        list_ir->list[list_ir->size_list].arg2.stack = 1;
                        (list_ir->size_list)++;
                        // fprintf (tree->output, "MOV {AX} {stack}\n");
                    }

                    list_ir->list[list_ir->size_list].num = IR_MATH_OPER;
                    list_ir->list[list_ir->size_list].arg1.num_int = OP_SUB;
                    (list_ir->size_list)++;

                    // fprintf (tree->output, "MATH_OPER {SUB}\n");
                    break;
                }
                case F_MUL:
                {
                    WritingIR (node->left, tree, list_ir);
                    WritingIR (node->right, tree, list_ir);

                    if (node->right->type == FUNC)
                    {
                        list_ir->list[list_ir->size_list].num = IR_MOV;
                        list_ir->list[list_ir->size_list].arg1.reg = 1;
                        list_ir->list[list_ir->size_list].arg2.stack = 1;
                        (list_ir->size_list)++;
                        // fprintf (tree->output, "MOV {AX} {stack}\n");
                    }

                    list_ir->list[list_ir->size_list].num = IR_MATH_OPER;
                    list_ir->list[list_ir->size_list].arg1.num_int = OP_MUL;
                    (list_ir->size_list)++;
                    // fprintf (tree->output, "MATH_OPER {MUL}\n");
                    break;
                }
                case F_DIV:
                {
                    WritingIR (node->left, tree, list_ir);
                    WritingIR (node->right, tree, list_ir);

                    if (node->right->type == FUNC)
                    {
                        list_ir->list[list_ir->size_list].num = IR_MOV;
                        list_ir->list[list_ir->size_list].arg1.reg = 1;
                        list_ir->list[list_ir->size_list].arg2.stack = 1;
                        (list_ir->size_list)++;
                        // fprintf (tree->output, "MOV {AX} {stack}\n");
                    }
                    list_ir->list[list_ir->size_list].num = IR_MATH_OPER;
                    list_ir->list[list_ir->size_list].arg1.num_int = OP_DIV;
                    (list_ir->size_list)++;
                    // fprintf (tree->output, "MATH_OPER {DIV}\n");
                    break;
                }
                case F_WHILE:
                {
                    fprintf (tree->output, "WHILE%p:\n", node);
                    WritingIR (node->left->left, tree, list_ir);
                    WritingIR (node->left->right, tree, list_ir);
                    ChangeSign (node, tree, list_ir);
                    WritingIR (node->right, tree, list_ir);
                    fprintf (tree->output, "JMP WHILE%p:\nLABEL%p:\n", node, node);
                    break;
                }
                case F_IF:
                {
                    WritingIR (node->left->left, tree, list_ir);
                    WritingIR (node->left->right, tree, list_ir);
                    ChangeSign (node, tree, list_ir);
                    if (node->right->value.com != F_ELSE)
                    {
                        WritingIR (node->right, tree, list_ir);
                        list_ir->list[list_ir->size_list].num = IR_LABEL;

                        sprintf (&(list_ir->buffer_for_label[POINTER_LABEL]), "LABEL%p", node);
        list_ir->list[list_ir->size_list].arg1.num_str = list_ir->buffer_for_label + POINTER_LABEL; 
        POINTER_LABEL += 20;
                        (list_ir->size_list)++;
                        // fprintf (tree->output, "LABEL {LABEL%p}\n", node);
                    }
                    else 
                    {
                        WritingIR (node->right->left, tree, list_ir);
                        list_ir->list[list_ir->size_list].num = IR_JMP;
                        sprintf (&(list_ir->buffer_for_label[POINTER_LABEL]), "LABEL%p", node->right);
        list_ir->list[list_ir->size_list].arg2.num_str = list_ir->buffer_for_label + POINTER_LABEL; 
        POINTER_LABEL += 20;
                        list_ir->list[list_ir->size_list].arg1.num_int = ALWAYS;
                        (list_ir->size_list)++;
                        // fprintf (tree->output, "JMP {ALWAYS} {LABEL%p}\n", node->right);
                        list_ir->list[list_ir->size_list].num = IR_LABEL;
                        sprintf (&(list_ir->buffer_for_label[POINTER_LABEL]), "LABEL%p", node);
                        list_ir->list[list_ir->size_list].arg1.num_str = list_ir->buffer_for_label + POINTER_LABEL; 
                        POINTER_LABEL += 20;
                        (list_ir->size_list)++;
                        // fprintf (tree->output, "LABEL {LABEL%p}\n", node);
                        WritingIR (node->right->right, tree, list_ir);
                        // fprintf (tree->output, "LABEL {LABEL%p}\n", node->right);
                        list_ir->list[list_ir->size_list].num = IR_LABEL;
                        sprintf (&(list_ir->buffer_for_label[POINTER_LABEL]), "LABEL%p", node->right);
                        list_ir->list[list_ir->size_list].arg1.num_str = list_ir->buffer_for_label + POINTER_LABEL; 
                        POINTER_LABEL += 20;
                        (list_ir->size_list)++;
                    }
                    break;
                }
                case F_PRINT:
                {
                    WritingIR (node->right, tree, list_ir);
                    list_ir->list[(list_ir->size_list)].num = IR_CALL_FUNC;
                    list_ir->list[(list_ir->size_list)].arg1.num_str = "OUT";
                    (list_ir->size_list)++;
                    // fprintf (tree->output, "CALL_FUNC {OUT}\n");
                    break;
                }
                case F_PRINT_C:
                {
                    WritingIR (node->right, tree, list_ir);
                    list_ir->list[(list_ir->size_list)].num = IR_CALL_FUNC;
                    list_ir->list[(list_ir->size_list)].arg1.num_str = "OUTC";
                    (list_ir->size_list)++;
                    // fprintf (tree->output, "CALL_FUNC {OUTC}\n");
                    break;
                }
                case F_INPUT:
                {
                    list_ir->list[(list_ir->size_list)].num = IR_CALL_FUNC;
                    list_ir->list[(list_ir->size_list)].arg1.num_str = "IN";
                    (list_ir->size_list)++;
                    // fprintf (tree->output, "CALL_FUNC {IN}\n");
                    list_ir->list[list_ir->size_list].num = IR_MOV;
                    list_ir->list[list_ir->size_list].arg1.reg = 1;
                    list_ir->list[list_ir->size_list].arg2.stack = 1;
                    (list_ir->size_list)++;
                    // fprintf (tree->output, "MOV {AX} {stack}\n");
                    list_ir->list[list_ir->size_list].num = IR_MOV;
                    list_ir->list[list_ir->size_list].arg1.stack = 1;
                    list_ir->list[list_ir->size_list].arg2.memory = 1;
                    list_ir->list[list_ir->size_list].arg2.num_int = node->right->value.var - NUM_FUNC;
                    (list_ir->size_list)++;
                    // fprintf (tree->output, "MOV {stack} {[%d]}\n", node->right->value.var - NUM_FUNC);
                    break;
                }
                case F_SQRT:
                {
                    WritingIR (node->right, tree, list_ir);
                    fprintf (tree->output, "SQRT\n");
                    break;
                }
                case F_RETURN:
                {
                    WritingIR (node->right, tree, list_ir);
                    list_ir->list[list_ir->size_list].num = IR_MOV;
                    list_ir->list[list_ir->size_list].arg1.stack = 1;
                    list_ir->list[list_ir->size_list].arg2.reg = 1;
                    (list_ir->size_list)++;
                    // fprintf (tree->output, "MOV {stack} {AX}\n");
                    list_ir->list[list_ir->size_list].num = IR_RET;
                    (list_ir->size_list)++;
                    // fprintf (tree->output, "RET\n");
                    break;
                }
                default:
                {
                    fprintf (tree->output, "I don't now this command\n");
                    break;
                }
            }
        }
        default: break;
    }
}

void ChangeSign (Node_t* node, Tree_t* tree, ListIR_t* list_ir)
{
    if (node->left->value.com == F_JBE)
    {
        list_ir->list[list_ir->size_list].num = IR_CMP;
        (list_ir->size_list)++;
        list_ir->list[list_ir->size_list].num = IR_JMP;
        list_ir->list[list_ir->size_list].arg1.num_int = JA;
        sprintf (&(list_ir->buffer_for_label[POINTER_LABEL]), "LABEL%p", node);
        list_ir->list[list_ir->size_list].arg2.num_str = list_ir->buffer_for_label + POINTER_LABEL; 
        POINTER_LABEL += 20;
        (list_ir->size_list)++;
        // fprintf (tree->output, "CMP\nJMP {JA} {LABEL%p}\n", node);
    }
    else if (node->left->value.com == F_JE)
    {
        list_ir->list[list_ir->size_list].num = IR_CMP;
        (list_ir->size_list)++;
        list_ir->list[list_ir->size_list].num = IR_JMP;
        list_ir->list[list_ir->size_list].arg1.num_int = JNE;
        sprintf (&(list_ir->buffer_for_label[POINTER_LABEL]), "LABEL%p", node);
        list_ir->list[list_ir->size_list].arg2.num_str = list_ir->buffer_for_label + POINTER_LABEL; 
        POINTER_LABEL += 20;
        (list_ir->size_list)++;
        // fprintf (tree->output, "CMP\nJMP {JNE} {LABEL%p}\n", node);
    }
    else if (node->left->value.com == F_JAE)
    {
        list_ir->list[list_ir->size_list].num = IR_CMP;
        (list_ir->size_list)++;
        list_ir->list[list_ir->size_list].num = IR_JMP;
        list_ir->list[list_ir->size_list].arg1.num_int = JB;
        sprintf (&(list_ir->buffer_for_label[POINTER_LABEL]), "LABEL%p", node);
        list_ir->list[list_ir->size_list].arg2.num_str = list_ir->buffer_for_label + POINTER_LABEL; 
        POINTER_LABEL += 20;
        (list_ir->size_list)++;
        // fprintf (tree->output, "CMP\nJMP {JB} {LABEL%p}\n", node);
    }
    else if (node->left->value.com == F_JB)
    {
        list_ir->list[list_ir->size_list].num = IR_CMP;
        (list_ir->size_list)++;
        list_ir->list[list_ir->size_list].num = IR_JMP;
        list_ir->list[list_ir->size_list].arg1.num_int = JAE;
        sprintf (&(list_ir->buffer_for_label[POINTER_LABEL]), "LABEL%p", node);
        list_ir->list[list_ir->size_list].arg2.num_str = list_ir->buffer_for_label + POINTER_LABEL; 
        POINTER_LABEL += 20;
        (list_ir->size_list)++;
        // fprintf (tree->output, "CMP\nJMP {JAE} {LABEL%p}\n", node);
    }
    else if (node->left->value.com == F_JA)
    {
        list_ir->list[list_ir->size_list].num = IR_CMP;
        (list_ir->size_list)++;
        list_ir->list[list_ir->size_list].num = IR_JMP;
        list_ir->list[list_ir->size_list].arg1.num_int = JBE;
        sprintf (&(list_ir->buffer_for_label[POINTER_LABEL]), "LABEL%p", node);
        list_ir->list[list_ir->size_list].arg2.num_str = list_ir->buffer_for_label + POINTER_LABEL; 
        POINTER_LABEL += 20;
        (list_ir->size_list)++;
        // fprintf (tree->output, "CMP\nJMP {JBE} {LABEL%p}\n", node);
    }
    else if (node->left->value.com == F_JNE)
    {
        list_ir->list[list_ir->size_list].num = IR_CMP;
        (list_ir->size_list)++;
        list_ir->list[list_ir->size_list].num = IR_JMP;
        list_ir->list[list_ir->size_list].arg1.num_int = JE;
        sprintf (&(list_ir->buffer_for_label[POINTER_LABEL]), "LABEL%p", node);
        list_ir->list[list_ir->size_list].arg2.num_str = list_ir->buffer_for_label + POINTER_LABEL; 
        POINTER_LABEL += 20;
        (list_ir->size_list)++;
        // fprintf (tree->output, "CMP\nJMP {JE} {LABEL%p}\n", node);
    }
}
