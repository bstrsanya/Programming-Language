#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>

#include "header-IR.h"
#include "Elf64.h"

void WriteNasm (ListIR_t* list)
{
    FILE* file_1 = fopen ("new_asm.s", "w");
    fprintf (file_1, "%%include \"func_lib.s\"\n");
    Node_IR* array = list->list;

    int PARAM = 1;
    int ARG_IN_FUNC = 2;

    for (int i = 0; i < list->size_list; i++)
    {
        switch (array[i].num)
        {
            case IR_START:
            {
                fprintf (file_1, "\n; START\n");
                fprintf (file_1, "section .text\nglobal _start\n\n_start:\n");
                break;  
            }
            case IR_BEGIN_FUNC_DEF:
            {
                fprintf (file_1, "\n; BEGIN_FUNC_DEF\n");
                fprintf (file_1, "%s:\n"
                                 "push rbp\n"
                                 "mov rbp, rsp\n"
                                 "sub rsp, 8 * %d\n", array[i].arg1.num_str, array[i].arg2.num_int);
                break;
            }
            case IR_END_FUNC_DEF:
                fprintf (file_1, "\n; END_FUNC_DEF\n");
                break;
            case IR_CALL_FUNC:
            {
                fprintf (file_1, "\n; CALL_FUNC\n");
                fprintf (file_1, "call %s\n", array[i].arg1.num_str);
                break;
            }
            case IR_RET:
            {
                fprintf (file_1, "\n; RET\n");
                fprintf (file_1, "mov rsp, rbp\n"
                                 "pop rbp\n"
                                 "ret\n");
                break;
            }
            case IR_GET_ARG:
            {
                fprintf (file_1, "\n; GET_ARG\n");
                fprintf (file_1, "mov rbx, [rbp + 8 * %d]\n"
                                 "mov [rbp - 8 * %d], rbx\n" , ARG_IN_FUNC, array[i].arg2.num_int);

                if (array[i + 1].num == IR_GET_ARG)
                    ARG_IN_FUNC++;
                else
                    ARG_IN_FUNC = 2;
                break;
            }
            case IR_MOV:
            {
                fprintf (file_1, "\n; MOV\n");
                if (array[i].arg1.stack == 1)
                {
                    if (array[i].arg2.memory == 1)
                        fprintf (file_1, "pop qword [rbp - 8 * %d]\n", array[i].arg2.num_int);
                    else if (array[i].arg2.reg == 1)
                        fprintf (file_1, "pop rax\n");
                }
                else if (array[i].arg1.reg == 1)
                {
                    if (array[i].arg2.memory == 1)
                        fprintf (file_1, "mov [rbp - 8 * %d], rax\n", array[i].arg2.num_int);
                    else if (array[i].arg2.stack == 1)
                        fprintf (file_1, "push rax\n");
                }
                else if (array[i].arg1.memory == 1)
                {
                    if (array[i].arg2.reg == 1)
                        fprintf (file_1, "mov rax, [rbp - 8 * %d]\n", array[i].arg1.num_int);
                    else if (array[i].arg2.stack == 1)
                        fprintf (file_1, "push qword [rbp - 8 * %d]\n", array[i].arg1.num_int);
                }
                else
                {
                    if (array[i].arg2.reg == 1)
                        fprintf (file_1, "mov rax, %d\n", array[i].arg1.num_int);
                    else if (array[i].arg2.stack == 1)
                        fprintf (file_1, "push %d\n", array[i].arg1.num_int);
                    else if (array[i].arg2.memory == 1)
                        fprintf (file_1, "mov [rbp - 8 * %d], %d\n", array[i].arg2.num_int, array[i].arg1.num_int);
                }
                break;
            }
            case IR_CMP:
            {
                fprintf (file_1, "\n; CMP\n");
                fprintf (file_1, "pop rcx\n"
                                 "pop rbx\n"
                                 "cmp rbx, rcx\n");
                break;
            }
            case IR_JMP:
            {
                fprintf (file_1, "\n; JMP\n");

                if (array[i].arg1.num_int == JA)
                    fprintf (file_1, "jg "); // ja
                else if (array[i].arg1.num_int == JAE)
                    fprintf (file_1, "jge "); // jae
                else if (array[i].arg1.num_int == JB)
                    fprintf (file_1, "jle "); // jbe
                else if (array[i].arg1.num_int == JBE)
                    fprintf (file_1, "jl ");  // jb
                else if (array[i].arg1.num_int == JNE)
                    fprintf (file_1, "jne ");
                else if (array[i].arg1.num_int == JE)
                    fprintf (file_1, "je ");
                else    
                    fprintf (file_1, "jmp ");

                fprintf (file_1, ".%s\n", array[i].arg2.num_str);
                break;
            }
            case IR_MATH_OPER:
            {
                fprintf (file_1, "\n; MATH_OPER\n");
                fprintf (file_1, "pop rcx\n"
                                 "pop rbx\n");

                if (array[i].arg1.num_int == OP_ADD)
                    fprintf (file_1, "add rbx, rcx\n");
                else if (array[i].arg1.num_int == OP_SUB)
                    fprintf (file_1, "sub rbx, rcx\n");
                else if (array[i].arg1.num_int == OP_MUL)
                    fprintf (file_1, "imul rbx, rcx\n");
                else if (array[i].arg1.num_int == OP_DIV)
                    fprintf (file_1, "mov rax, rbx\n"
                                     "xor rdx, rdx\n"
                                     "dec rdx\n"
                                     "idiv rcx\n"
                                     "mov rbx, rax\n");

                fprintf (file_1, "push rbx\n");
                break;
            }
            case IR_LABEL:
            {
                fprintf (file_1, "\n; LABEL\n");
                fprintf (file_1, ".%s:\n", array[i].arg1.num_str);
                break;
            }
            case IR_PUT_ARG_FUNC:
            {
                fprintf (file_1, "\n; PUT_ARG_FUNC\n");
                fprintf (file_1, "push qword [rbp - 8 * %d]\n", array[i].arg1.num_int);
                if (array[i + 1].num == IR_PUT_ARG_FUNC)
                    PARAM++;
                break;
            }
            case IR_FREE_ARG:
            {
                fprintf (file_1, "\n; FREE_ARG\n");
                fprintf (file_1, "add rsp, 8 * %d\n", PARAM);
                PARAM = 1;
                break;
            }
            case IR_END:
            {
                fprintf (file_1, "\n; END\n");
                break;
            }
            default:
                break;
        }
    }
}

