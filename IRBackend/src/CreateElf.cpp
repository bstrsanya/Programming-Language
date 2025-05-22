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

    pair array_pair[SIZE_ARRAY_PAIR] = {};
    int pointer_pair = 0;

    uint8_t* byte = list->byte;
    int pointer_byte = 0;

    int PARAM = 1;
    int ARG_IN_FUNC = 2;

    for (int i = 0; i < list->size_list; i++)
    {
        switch (array[i].num)
        {
            case IR_START:
            case IR_END_FUNC_DEF:
                break;  
            
            case IR_BEGIN_FUNC_DEF:
            {
                array[i].arg1.num_int = pointer_byte;
                PUSH_REG (RBP, byte, pointer_byte);
                MOV_REG_REG (RBP, RSP, byte, pointer_byte);
                MATH_OP_REG_NUM (SUB, RSP, 8 * array[i].arg2.num_int, byte, pointer_byte);
                break;
            }
            case IR_CALL_FUNC:
            {
                CALLING_FUNCTION (byte, pointer_byte);
                array_pair[pointer_pair].str = array[i].arg1.num_str;
                array_pair[pointer_pair].index = pointer_byte - 4;
                pointer_pair++;
                break;
            }
            case IR_RET:
            {
                MOV_REG_REG (RSP, RBP, byte, pointer_byte);
                POP_REG (RBP, byte, pointer_byte);
                RET_COM (byte, pointer_byte);
                break;
            }
            case IR_GET_ARG:
            {
                MOV_REG_MEM (RBX, RBP, 8 * ARG_IN_FUNC, byte, pointer_byte);
                MOV_MEM_REG (RBP, -8 * array[i].arg2.num_int, RBX, byte, pointer_byte);
                if (array[i + 1].num == IR_GET_ARG)
                    ARG_IN_FUNC++;
                else
                    ARG_IN_FUNC = 2;
                break;
            }
            case IR_MOV:
            {
                if (array[i].arg1.stack == 1)
                {
                    if (array[i].arg2.memory == 1)
                        POP_MEM (RBP, -8 * array[i].arg2.num_int, byte, pointer_byte);
                    else if (array[i].arg2.reg == 1)
                        POP_REG (RAX, byte, pointer_byte);
                }
                else if (array[i].arg1.reg == 1)
                {
                    if (array[i].arg2.memory == 1)
                        MOV_MEM_REG (RBP, -8 * array[i].arg2.num_int, RAX, byte, pointer_byte);
                    else if (array[i].arg2.stack == 1)
                        PUSH_REG (RAX, byte, pointer_byte);
                }
                else if (array[i].arg1.memory == 1)
                {
                    if (array[i].arg2.reg == 1)
                        MOV_REG_MEM (RAX, RBP, -8 * array[i].arg1.num_int,  byte, pointer_byte);
                    else if (array[i].arg2.stack == 1)
                        PUSH_MEM (RBP, -8 * array[i].arg1.num_int, byte, pointer_byte);
                }
                else
                {
                    if (array[i].arg2.reg == 1)
                        MOV_REG_NUM (RAX, array[i].arg1.num_int, byte, pointer_byte);
                    else if (array[i].arg2.stack == 1)
                        PUSH_NUM (array[i].arg1.num_int, byte, pointer_byte);
                    else if (array[i].arg2.memory == 1)
                        MOV_MEM_NUM (RBP, -8 * array[i].arg2.num_int, array[i].arg1.num_int, byte, pointer_byte);
                }
                break;
            }
            case IR_CMP:
            {
                POP_REG (RCX, byte, pointer_byte);
                POP_REG (RBX, byte, pointer_byte);
                CMP_REG_REG (RBX, RCX, byte, pointer_byte);
                break;
            }
            case IR_JMP:
            {
                JMP_COM (array[i].arg1.num_int, byte, pointer_byte);
                array_pair[pointer_pair].str = array[i].arg2.num_str;
                array_pair[pointer_pair].index = pointer_byte - 4;
                pointer_pair++;
                break;
            }
            case IR_MATH_OPER:
            {
                POP_REG (RCX, byte, pointer_byte);
                POP_REG (RBX, byte, pointer_byte);
                MATH_OP_REG_REG (array[i].arg1.num_int, RBX, RCX, byte, pointer_byte);
                PUSH_REG (RBX, byte, pointer_byte);
                break;
            }
            case IR_LABEL:
            {
                array[i].arg1.num_int = pointer_byte;
                break;
            }
            case IR_PUT_ARG_FUNC:
            {
                PUSH_MEM (RBP,  -8 * array[i].arg1.num_int, byte, pointer_byte);
                if (array[i + 1].num == IR_PUT_ARG_FUNC)
                    PARAM++;
                break;
            }
            case IR_FREE_ARG:
            {
                MATH_OP_REG_NUM (ADD, RSP, 8 * PARAM, byte, pointer_byte);
                PARAM = 1;
                break;
            }
        }
    }

    for (int k = 0; k < pointer_pair; k++)
    {
        for (int i = 0; i < list->size_list; i++)
        {
            if (array[i].num == IR_LABEL || array[i].num == IR_BEGIN_FUNC_DEF)
            {
                if (!strcmp (array[i].arg1.num_str, array_pair[k].str))
                {
                    int value = array[i].arg1.num_int - array_pair[k].index - 4;
                    for (int shift = 0; shift < 4; shift++)
                        byte[array_pair[k].index + shift] = (value >> (8 * shift)) & 0xFF;
                }
            }
        }
        if (!strcmp (array_pair[k].str, "IN"))
        {
            int value = -array_pair[k].index - list->size_asm_code + list->asm_code[0] + (list->asm_code[1] << 8);
            for (int shift = 0; shift < 4; shift++)
                byte[array_pair[k].index + shift] = (value >> (8 * shift)) & 0xFF;
        }
        else if (!strcmp (array_pair[k].str, "OUT"))
        {
            int value = -array_pair[k].index - list->size_asm_code  + list->asm_code[8] + (list->asm_code[9] << 8);
            for (int shift = 0; shift < 4; shift++)
                byte[array_pair[k].index + shift] = (value >> (8 * shift)) & 0xFF;
        }
        else if (!strcmp (array_pair[k].str, "HLT"))
        {
            int value = -array_pair[k].index - list->size_asm_code  + list->asm_code[16] + (list->asm_code[17] << 8);
            for (int shift = 0; shift < 4; shift++)
                byte[array_pair[k].index + shift] = (value >> (8 * shift)) & 0xFF;
        }
    }

    list->size_byte = pointer_byte;
    WriteElf (list);
}

void WriteElf (ListIR_t* list)
{
    int size_asm_code = list->size_asm_code;
    const uint8_t jmp_to_main[] = {0xE9, (size_asm_code >> (8 * 0)) & 0xFF, (size_asm_code >> (8 * 1)) & 0xFF, (size_asm_code >> (8 * 2)) & 0xFF, (size_asm_code >> (8 * 3)) & 0xFF};

    const int headersSize = sizeof(Elf64Header) + sizeof(Elf64_PhtEntry);
    const uint64_t codeSize = size_asm_code + list->size_byte + sizeof (jmp_to_main);

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
        .p_type   = 1,                              // 1: PT_LOAD
        .p_flags  = 0x7,                            // 0: execute, 1: write, 2: read
        .p_offset = headersSize,
        .p_vaddr  = 0x400000 + headersSize,         // linux 
        .p_paddr  = 0x400000 + headersSize,
        .p_filesz = codeSize,
        .p_memsz  = codeSize,
        .p_align  = 0x1000
    };

    fwrite (&header,        1, sizeof (header),      list->output);
    fwrite (&phtEntry,      1, sizeof (phtEntry),    list->output);
    fwrite (jmp_to_main,    1, sizeof (jmp_to_main), list->output);
    fwrite (list->asm_code, 1, list->size_asm_code , list->output);
    fwrite (list->byte,     1, list->size_byte,      list->output);

    char str[BUFFER] = "";
    sprintf (str, "chmod +x %s", list->name_output_file);
    system (str);
}

void CopyLib (ListIR_t* list, const char* name_file)
{
    FILE* file = fopen (name_file, "rb");
    size_t size = 0;
    uint8_t* array = (uint8_t*) ReadFile (file, &size);

    unsigned size_code = 0;

    for (int i = 0; i < 4; i++) 
    {
        size_code <<= 8;
        size_code |= array[ADDR_BEGIN_SIZE_CODE + (3 - i)];
    }


    uint8_t* asm_code = (uint8_t*) calloc (size_code - 0x1000, sizeof (uint8_t));
    int size_asm_code = 0;
    for (int i = 0x1000; i < size_code; i++)
    {
        asm_code[i - 0x1000] = array[i];
        size_asm_code++;
    }

    free (array);
    
    list->asm_code = asm_code;
    list->size_asm_code = size_asm_code;
}