#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "Recovery_IR.h"

void CreateIR (Tree_t* tree)
{
    assert (tree);
    assert (tree->output);
    assert (tree->expression);

    fprintf (tree->output, "START\n");
    fprintf (tree->output, "CALL_FUNC {main}\nCALL_FUNC {HLT}\n");
    WritingIR (tree->expression, tree);
    fprintf (tree->output, "END\n");
}

int IN_FUNC = 1;
int IN_ARG_FUNC = 1;
int IN_RETURN = 0;
int NUM_FUNC = 0;
int CALLING_FUNC = 0;

void WritingIR (Node_t* node, Tree_t* tree)
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
                if (node->left) WritingIR (node->left, tree);
                if (node->right) WritingIR (node->right, tree);
                fprintf (tree->output, "CALL_FUNC {%s}\n", tree->table_var[node->value.var]);
                fprintf (tree->output, "FREE_ARG\n");
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
                fprintf (tree->output, "BEGIN_FUNC_DEF {%s} {%d}\n", tree->table_var[node->value.var], y - NUM_FUNC - 1);
                IN_FUNC = 0;
                IN_ARG_FUNC = 0;
                if (node->left) WritingIR (node->left, tree);
                IN_ARG_FUNC = 1;
                if (node->right) WritingIR (node->right, tree);
                IN_FUNC = 1;
                fprintf (tree->output, "RET\nEND_FUNC_DEF\n");
            }
            break;
        }
        case NUM:
        {
            fprintf (tree->output, "MOV {%g} {stack}\n", node->value.number);
            break;
        }
        case VAR:
        {
            if (IN_ARG_FUNC == 0)
            {
                fprintf (tree->output, "GET_ARG {stack} {[%d]}\n", node->value.var - NUM_FUNC);
            }
            else if (CALLING_FUNC == 1)
            {
                fprintf (tree->output, "PUT_ARG_FUNC {[%d]}\n", node->value.var - NUM_FUNC);
            }
            else
                fprintf (tree->output, "MOV {[%d]} {stack}\n", node->value.var - NUM_FUNC);
            break;
        }
        case BLOCK:
        {
            if (node->left) WritingIR (node->left, tree);
            if (node->right) WritingIR (node->right, tree);
            break;
        }
        case OP:
        {
            switch ((int) node->value.com)
            {
                case F_INTERRUPT:
                {
                    if (node->left) WritingIR (node->left, tree);
                    if (node->right) WritingIR (node->right, tree);
                    break;
                }
                case F_INT:
                case F_DOUBLE:
                {
                    WritingIR (node->right, tree);
                    break;
                }
                case F_ASSIGNMENT:
                {
                    WritingIR (node->right, tree);

                    if (node->right->type == FUNC)
                        fprintf (tree->output, "MOV {AX} {stack}\n");

                    fprintf (tree->output, "MOV {stack} {[%d]}\n", node->left->value.var - NUM_FUNC);
                    break;
                }
                case F_ADD:
                {
                    WritingIR (node->left, tree);
                    WritingIR (node->right, tree);

                    if (node->right->type == FUNC)
                        fprintf (tree->output, "MOV {AX} {stack}\n");

                    fprintf (tree->output, "MATH_OPER {ADD}\n");
                    break;
                }
                case F_SUB:
                {
                    WritingIR (node->left, tree);
                    WritingIR (node->right, tree);

                    if (node->right->type == FUNC)
                        fprintf (tree->output, "MOV {AX} {stack}\n");

                    fprintf (tree->output, "MATH_OPER {SUB}\n");
                    break;
                }
                case F_MUL:
                {
                    WritingIR (node->left, tree);
                    WritingIR (node->right, tree);

                    if (node->right->type == FUNC)
                        fprintf (tree->output, "MOV {AX} {stack}\n");

                    fprintf (tree->output, "MATH_OPER {MUL}\n");
                    break;
                }
                case F_DIV:
                {
                    WritingIR (node->left, tree);
                    WritingIR (node->right, tree);

                    if (node->right->type == FUNC)
                        fprintf (tree->output, "MOV {AX} {stack}\n");
                        
                    fprintf (tree->output, "MATH_OPER {DIV}\n");
                    break;
                }
                case F_WHILE:
                {
                    fprintf (tree->output, "WHILE%p:\n", node);
                    WritingIR (node->left->left, tree);
                    WritingIR (node->left->right, tree);
                    ChangeSign (node, tree);
                    WritingIR (node->right, tree);
                    fprintf (tree->output, "JMP WHILE%p:\nLABEL%p:\n", node, node);
                    break;
                }
                case F_IF:
                {
                    WritingIR (node->left->left, tree);
                    WritingIR (node->left->right, tree);
                    ChangeSign (node, tree);
                    if (node->right->value.com != F_ELSE)
                    {
                        WritingIR (node->right, tree);
                        fprintf (tree->output, "LABEL {LABEL%p}\n", node);
                    }
                    else 
                    {
                        WritingIR (node->right->left, tree);
                        fprintf (tree->output, "JMP {ALWAYS} {LABEL%p}\n", node->right);
                        fprintf (tree->output, "LABEL {LABEL%p}\n", node);
                        WritingIR (node->right->right, tree);
                        fprintf (tree->output, "LABEL {LABEL%p}\n", node->right);
                    }
                    break;
                }
                case F_PRINT:
                {
                    WritingIR (node->right, tree);
                    fprintf (tree->output, "CALL_FUNC {OUT}\n");
                    break;
                }
                case F_PRINT_C:
                {
                    WritingIR (node->right, tree);
                    fprintf (tree->output, "CALL_FUNC {OUTC}\n");
                    break;
                }
                case F_INPUT:
                {
                    fprintf (tree->output, "CALL_FUNC {IN}\n");
                    fprintf (tree->output, "MOV {AX} {stack}\n");
                    fprintf (tree->output, "MOV {stack} {[%d]}\n", node->right->value.var - NUM_FUNC);
                    break;
                }
                case F_SQRT:
                {
                    WritingIR (node->right, tree);
                    fprintf (tree->output, "SQRT\n");
                    break;
                }
                case F_RETURN:
                {
                    WritingIR (node->right, tree);
                    fprintf (tree->output, "MOV {stack} {AX}\n");
                    fprintf (tree->output, "RET\n");
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

void ChangeSign (Node_t* node, Tree_t* tree)
{
    if (node->left->value.com == F_JBE)
    {
        fprintf (tree->output, "CMP\nJMP {JA} {LABEL%p}\n", node);
    }
    else if (node->left->value.com == F_JE)
    {
        fprintf (tree->output, "CMP\nJMP {JNE} {LABEL%p}\n", node);
    }
    else if (node->left->value.com == F_JAE)
    {
        fprintf (tree->output, "CMP\nJMP {JB} {LABEL%p}\n", node);
    }
    else if (node->left->value.com == F_JB)
    {
        fprintf (tree->output, "CMP\nJMP {JAE} {LABEL%p}\n", node);
    }
    else if (node->left->value.com == F_JA)
    {
        fprintf (tree->output, "CMP\nJMP {JBE} {LABEL%p}\n", node);
    }
    else if (node->left->value.com == F_JNE)
    {
        fprintf (tree->output, "CMP\nJMP {JE} {LABEL%p}\n", node);
    }
}
