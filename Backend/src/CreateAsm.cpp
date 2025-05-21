#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "Recovery.h"

void CreateAsmFile (Tree_t* tree)
{
    assert (tree);
    assert (tree->output);
    assert (tree->expression);

    fprintf (tree->output, "CALL main:\nHLT\n");
    WritingAsmFile (tree->expression, tree);
}

int IN_FUNC = 1;
int IN_ARG_FUNC = 1;
int IN_RETURN = 0;

void WritingAsmFile (Node_t* node, Tree_t* tree)
{
    assert (node);
    assert (tree);

    switch (node->type)
    {
        case INVALID_TYPE:
        {
            printf ("df\n");
            break;
        }
        case FUNC:
        {
            if (IN_FUNC == 0)
            {   
                
                int i = node->value.var + 1;
                while (tree->table_var[i])
                {
                    fprintf (tree->output, "PUSH [%d]\n", i);
                    i++;
                }
                
                if (node->left) WritingAsmFile (node->left, tree);
                if (node->right) WritingAsmFile (node->right, tree);
                fprintf (tree->output, "CALL %s:\n", tree->table_var[node->value.var]);
                i--;
                while (i > node->value.var)
                {
                    fprintf (tree->output, "POP [%d]\n", i);
                    i--;
                }
            }
            else
            {
                fprintf (tree->output, "%s:\n", tree->table_var[node->value.var]);
                IN_FUNC = 0;
                IN_ARG_FUNC = 0;
                if (node->left) WritingAsmFile (node->left, tree);
                IN_ARG_FUNC = 1;
                if (node->right) WritingAsmFile (node->right, tree);
                IN_FUNC = 1;
                fprintf (tree->output, "RET\n");
            }
            break;
        }
        case NUM:
        {
            fprintf (tree->output, "PUSH %g\n", node->value.number);
            break;
        }
        case VAR:
        {
            if (IN_ARG_FUNC == 0)
                fprintf (tree->output, "POP [%d]\n", node->value.var);
            else
                fprintf (tree->output, "PUSH [%d]\n", node->value.var);
            break;
        }
        case BLOCK:
        {
            if (node->left) WritingAsmFile (node->left, tree);
            if (node->right) WritingAsmFile (node->right, tree);
            break;
        }
        case OP:
        {
            switch ((int) node->value.com)
            {
                case F_INTERRUPT:
                {
                    if (node->left) WritingAsmFile (node->left, tree);
                    if (node->right) WritingAsmFile (node->right, tree);
                    break;
                }
                case F_INT:
                case F_DOUBLE:
                {
                    WritingAsmFile (node->right, tree);
                    break;
                }
                case F_ASSIGNMENT:
                {
                    WritingAsmFile (node->right, tree);
                    if (node->right->type == FUNC)
                    {
                        fprintf (tree->output, "PUSH AX\n");
                    }
                    fprintf (tree->output, "POP [%d]\n", node->left->value.var);
                    break;
                }
                case F_ADD:
                {
                    WritingAsmFile (node->left, tree);
                    WritingAsmFile (node->right, tree);
                    fprintf (tree->output, "ADD\n");
                    break;
                }
                case F_SUB:
                {
                    WritingAsmFile (node->left, tree);
                    WritingAsmFile (node->right, tree);
                    fprintf (tree->output, "SUB\n");
                    break;
                }
                case F_MUL:
                {
                    WritingAsmFile (node->left, tree);
                    WritingAsmFile (node->right, tree);

                    if (node->right->type == FUNC)
                        fprintf (tree->output, "PUSH AX\n");

                    fprintf (tree->output, "MUL\n");
                    break;
                }
                case F_DIV:
                {
                    WritingAsmFile (node->left, tree);
                    WritingAsmFile (node->right, tree);
                    fprintf (tree->output, "DIV\n");
                    break;
                }
                case F_WHILE:
                {
                    fprintf (tree->output, "WHILE%p:\n", node);
                    WritingAsmFile (node->left->left, tree);
                    WritingAsmFile (node->left->right, tree);
                    ChangeSign (node, tree);
                    WritingAsmFile (node->right, tree);
                    fprintf (tree->output, "JMP WHILE%p:\nLABEL%p:\n", node, node);
                    break;
                }
                case F_IF:
                {
                    WritingAsmFile (node->left->left, tree);
                    WritingAsmFile (node->left->right, tree);
                    ChangeSign (node, tree);
                    if (node->right->value.com != F_ELSE)
                    {
                        WritingAsmFile (node->right, tree);
                        fprintf (tree->output, "LABEL%p:\n", node);
                    }
                    else 
                    {
                        WritingAsmFile (node->right->left, tree);
                        fprintf (tree->output, "JMP LABEL%p:\n", node->right);
                        fprintf (tree->output, "LABEL%p:\n", node);
                        WritingAsmFile (node->right->right, tree);
                        fprintf (tree->output, "LABEL%p:\n", node->right);
                    }
                    break;
                }
                case F_PRINT:
                {
                    WritingAsmFile (node->right, tree);
                    fprintf (tree->output, "OUT\n");
                    break;
                }
                case F_PRINT_C:
                {
                    WritingAsmFile (node->right, tree);
                    fprintf (tree->output, "OUTC\n");
                    break;
                }
                case F_INPUT:
                {
                    fprintf (tree->output, "IN\n");
                    fprintf (tree->output, "POP [%d]\n", node->right->value.var);
                    break;
                }
                case F_SQRT:
                {
                    WritingAsmFile (node->right, tree);
                    fprintf (tree->output, "SQRT\n");
                    break;
                }
                case F_RETURN:
                {
                    WritingAsmFile (node->right, tree);
                    fprintf (tree->output, "POP AX\n");
                    fprintf (tree->output, "RET\n");
                    break;
                }
                default:
                {
                    printf ("I don't now this command\n");
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
        fprintf (tree->output, "JA LABEL%p:\n", node);
    }
    else if (node->left->value.com == F_JE)
    {
        fprintf (tree->output, "JNE LABEL%p:\n", node);
    }
    else if (node->left->value.com == F_JAE)
    {
        fprintf (tree->output, "JB LABEL%p:\n", node);
    }
    else if (node->left->value.com == F_JB)
    {
        fprintf (tree->output, "JAE LABEL%p:\n", node);
    }
    else if (node->left->value.com == F_JA)
    {
        fprintf (tree->output, "JBE LABEL%p:\n", node);
    }
    else if (node->left->value.com == F_JNE)
    {
        fprintf (tree->output, "JE LABEL%p:\n", node);
    }
}




