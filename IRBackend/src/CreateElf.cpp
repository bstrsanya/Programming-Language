#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>

#include "header-IR.h"
#include "Elf64.h"

void CreateElf (ListIR_t* list)
{
    Node_IR* array = list->list;

    par array_par[100] = {};
    int j = 0;
    uint8_t* byte = list->byte;
    int b = 0;

    int PARAM = 1;
    int II = 2;

    for (int i = 0; i < list->size_list; i++)
    {
        switch (array[i].num)
        {
            case IR_START:
            case IR_END_FUNC_DEF:
                break;  
            
            case IR_BEGIN_FUNC_DEF:
            {
                array[i].arg1.num_int = b;
                PUSH_REG (RBP, byte, b);
                MOV_REG_REG (RBP, RSP, byte, b);
                MATH_OP_REG_NUM (SUB, RSP, 8 * array[i].arg2.num_int, byte, b);
                break;
            }
            case IR_CALL_FUNC:
            {
                CALLING_FUNCTION (byte, b);
                array_par[j].str = array[i].arg1.num_str;
                array_par[j].index = b - 4;
                j++;
                break;
            }
            case IR_RET:
            {
                MOV_REG_REG (RSP, RBP, byte, b);
                POP_REG (RBP, byte, b);
                RET_COM (byte, b);
                break;
            }
            case IR_GET_ARG:
            {
                MOV_REG_MEM (RBX, RBP, 8 * II, byte, b);
                MOV_MEM_REG (RBP, -8 * array[i].arg2.num_int, RBX, byte, b);
                if (array[i + 1].num == IR_GET_ARG)
                    II++;
                else
                    II = 2;
                break;
            }
            case IR_MOV:
            {
                if (array[i].arg1.stack == 1)
                {
                    if (array[i].arg2.memory == 1)
                        POP_MEM (RBP, -8 * array[i].arg2.num_int, byte, b);
                    else if (array[i].arg2.reg == 1)
                        POP_REG (RAX, byte, b);
                    else
                        printf ("HEEEEEElP %d\n", __LINE__);
                }
                else if (array[i].arg1.reg == 1)
                {
                    if (array[i].arg2.memory == 1)
                        MOV_MEM_REG (RBP, -8 * array[i].arg2.num_int, RAX, byte, b);
                    else if (array[i].arg2.stack == 1)
                        PUSH_REG (RAX, byte, b);
                    else
                        printf ("HEEEEEElP %d\n", __LINE__);
                }
                else if (array[i].arg1.memory == 1)
                {
                    if (array[i].arg2.reg == 1)
                        MOV_REG_MEM (RAX, RBP, -8 * array[i].arg1.num_int,  byte, b);
                    else if (array[i].arg2.stack == 1)
                        PUSH_MEM (RBP, -8 * array[i].arg1.num_int, byte, b);
                    else
                        printf ("HEEEEEEEEEEEEEELP %d\n", __LINE__);
                }
                else
                {
                    if (array[i].arg2.reg == 1)
                        MOV_REG_NUM (RAX, array[i].arg1.num_int, byte, b);
                    else if (array[i].arg2.stack == 1)
                        PUSH_NUM (array[i].arg1.num_int, byte, b);
                    else if (array[i].arg2.memory == 1)
                        MOV_MEM_NUM (RBP, -8 * array[i].arg2.num_int, array[i].arg1.num_int, byte, b);
                    else
                        printf ("HEEEEEEELP %d\n", __LINE__);
                }
                break;
            }
            case IR_CMP:
            {
                POP_REG (RCX, byte, b);
                POP_REG (RBX, byte, b);
                CMP_REG_REG (RBX, RCX, byte, b);
                break;
            }
            case IR_JMP:
            {
                JMP_COM (array[i].arg1.num_int, byte, b);
                array_par[j].str = array[i].arg2.num_str;
                array_par[j].index = b - 4;
                j++;
                break;
            }
            case IR_MATH_OPER:
            {
                POP_REG (RCX, byte, b);
                POP_REG (RBX, byte, b);
                MATH_OP_REG_REG (array[i].arg1.num_int, RBX, RCX, byte, b);
                PUSH_REG (RBX, byte, b);
                break;
            }
            case IR_LABEL:
            {
                array[i].arg1.num_int = b;
                break;
            }
            case IR_PUT_ARG_FUNC:
            {
                PUSH_MEM (RBP,  -8 * array[i].arg1.num_int, byte, b);
                if (array[i + 1].num == IR_PUT_ARG_FUNC)
                    PARAM++;
                break;
            }
            case IR_FREE_ARG:
            {
                MATH_OP_REG_NUM (ADD, RSP, 8 * PARAM, byte, b);
                PARAM = 1;
                break;
            }
        }
    }

    for (int k = 0; k < j; k++)
    {
        for (int i = 0; i < list->size_list; i++)
        {
            if (array[i].num == IR_LABEL || array[i].num == IR_BEGIN_FUNC_DEF)
            {
                if (!strcmp (array[i].arg1.num_str, array_par[k].str))
                {
                    int value = array[i].arg1.num_int - array_par[k].index - 4;
                    for (int shift = 0; shift < 4; shift++)
                        byte[array_par[k].index + shift] = (value >> (8 * shift)) & 0xFF;
                }
            }
        }
        if (!strcmp (array_par[k].str, "IN"))
        {
            int value = -array_par[k].index - sizeof (asmCode) - 4;
            for (int shift = 0; shift < 4; shift++)
                byte[array_par[k].index + shift] = (value >> (8 * shift)) & 0xFF;
        }
        else if (!strcmp (array_par[k].str, "OUT"))
        {
            int value = -array_par[k].index - sizeof (asmCode) + 2 - 4;
            for (int shift = 0; shift < 4; shift++)
                byte[array_par[k].index + shift] = (value >> (8 * shift)) & 0xFF;
        }
        else if (!strcmp (array_par[k].str, "HLT"))
        {
            int value = -array_par[k].index - sizeof (asmCode) + 4 - 4;
            for (int shift = 0; shift < 4; shift++)
                byte[array_par[k].index + shift] = (value >> (8 * shift)) & 0xFF;
        }
    }

    list->size_byte = b;
    WriteElf (list);
}

void WriteElf (ListIR_t* list)
{
    int va = sizeof (asmCode);
    const uint8_t qw[] = {0xE9, (va >> (8 * 0)) & 0xFF, (va >> (8 * 1)) & 0xFF, (va >> (8 * 2)) & 0xFF, (va >> (8 * 3)) & 0xFF };

    const int headersSize = sizeof(Elf64Header) + sizeof(Elf64_PhtEntry);
    const uint64_t codeSize = sizeof(asmCode) + list->size_byte + sizeof (qw);
    // const int fileSize = headersSize + codeSize;

    Elf64Header header = 
    {
        .EI_MAG0       = 0x7f,                      // magic num
        .EI_MAG3       = {'E', 'L', 'F'},           // 'E' 'L' 'F'
        .EI_CLASS      = 2,                         // 64-bit format
        .EI_DATA       = 1,                         // little endian
        .EI_VERSION    = 1,                         // version of ELF
        .EI_OSABI      = 0,                         // system V
        .EI_ABIVERSION = 0,                         // ignored
        .EI_PAD        = {0,0,0,0,0,0,0},           // reserved
        .e_type        = 2,                         // file type
        .e_machine     = 0x3E,                      // instruction set architecture
        .e_version     = 1,                         // original version
        .e_entry       = 0x400000 + headersSize,     /* PROVIDE (__executable_start = SEGMENT_START("text-segment", 0x400000)); 
                                                        . = SEGMENT_START("text-segment", 0x400000) + SIZEOF_HEADERS; */
        .e_phoff       = sizeof (Elf64Header),      // point start program header table
        .e_shoff       = 0,                         // point start section header table
        .e_flags       = 0,                         // processor flags
        .e_ehsize      = sizeof (Elf64Header),      // size this header
        .e_phentsize   = sizeof (Elf64_PhtEntry),   // size program header table 
        .e_phnum       = 1,                         // num program header table 
        .e_shentsize   = 0,                         // size section header table
        .e_shnum       = 0,                         // num section header table
        .e_shstrndx    = 0                          // index section header table
    };

    Elf64_PhtEntry phtEntry = 
    {
        .p_type   = 1, // 1: PT_LOAD
        .p_flags  = 0x7, // 0: execute, 1: write, 2: read
        .p_offset = headersSize,
        .p_vaddr  = 0x400000 + headersSize, // linux 
        .p_paddr  = 0x400000 + headersSize,
        .p_filesz = codeSize,
        .p_memsz  = codeSize,
        .p_align  = 0x1000
    };

    fwrite (&header, 1, sizeof (header), list->output);
    fwrite (&phtEntry, 1, sizeof (phtEntry), list->output);
    fwrite (qw, 1, sizeof (qw), list->output);
    fwrite (asmCode, 1, sizeof (asmCode), list->output);
    fwrite (list->byte, 1, list->size_byte, list->output);

    char str[30] = "";
    sprintf (str, "chmod +x %s", list->name_output_file);
    system (str);
}