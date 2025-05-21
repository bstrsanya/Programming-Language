#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>

#include "header-IR.h"
#include "Elf64.h"

void ListIRCtor (ListIR_t* list_ir, const char* name_file_input, const char* name_file_output)
{
    assert (list_ir);
    assert (name_file_input);
    assert (name_file_output);

    printf ("I'm backend_IR! Open file [%s] for read and [%s] for writing\n", name_file_input, name_file_output);

    list_ir->input = fopen (name_file_input, "rb");

    Node_IR* array = (Node_IR*) calloc (1000, sizeof (Node_IR));
    list_ir->list = array;

    uint8_t* b_code = (uint8_t*) calloc (1000, sizeof (uint8_t));
    list_ir->byte = b_code;

    list_ir->size_list = 0;

    size_t size = 0;
    char* init_array = ReadFile (list_ir->input, &size);
    list_ir->data = init_array;

    list_ir->name_output_file = name_file_output;

    ReadFileIR (list_ir);
    fclose (list_ir->input);

    list_ir->output = fopen (name_file_output, "wb");
}

void ListIRDtor (ListIR_t* list_ir)
{
    assert (list_ir);

    free (list_ir->data);
    free (list_ir->list);
    free (list_ir->byte);
    
    if (list_ir->output)
        fclose (list_ir->output);
}

void ReadFileIR (ListIR_t* list_ir)
{
    char* init_array = list_ir->data;
    Node_IR* array = list_ir->list;

    int position = 0;
    int cur_node = 0;
    while (init_array[position] != 0)
    {
        while (init_array[position] == ' ' || init_array[position] == '\n')
            position++;
        
        int n = 0;
        sscanf (init_array + position, "%*[a-zA-Z_]%n", &n);
        init_array[position + n] = '\0';

        for (int i = 0; i < NUM_COMMAND_IR; i++)
        {
            if (!strcmp (array_command_ir[i].name, init_array + position))
            {
                position += n + 1;
                array[cur_node].num = array_command_ir[i].n_com; 

                switch (array[cur_node].num)
                {
                    case IR_START:
                    case IR_END:
                    case IR_END_FUNC_DEF:
                    case IR_RET:
                    case IR_FREE_ARG:
                    case IR_CMP:
                        break;
                    
                    case IR_BEGIN_FUNC_DEF:
                    {
                        sscanf (init_array + position, "%*[a-zA-Z{}]%n", &n);
                        init_array[position + n - 1] = '\0';
                        array[cur_node].arg1.num_str = init_array + position + 1;
                        position += n + 1;
                        sscanf (init_array + position, "{%d}%n", &array[cur_node].arg2.num_int, &n);
                        position += n + 1;
                        break;
                    }
                    case IR_CALL_FUNC:
                    {
                        sscanf (init_array + position, "%*[a-zA-Z{}]%n", &n);
                        init_array[position + n - 1] = '\0';
                        array[cur_node].arg1.num_str = init_array + position + 1;
                        position += n + 1;
                        break;
                    }
                    case IR_MOV:
                    case IR_GET_ARG:
                    {
                        if (init_array[position + 1] == 's')
                        {
                            array[cur_node].arg1.stack = 1;
                            position += 8;
                        }
                        else if (init_array[position + 1] == 'A')
                        {
                            array[cur_node].arg1.reg = 1;
                            position += 5;
                        }
                        else if (init_array[position + 1] == '[')
                        {
                            sscanf (init_array + position, "{[%d]}%n", &array[cur_node].arg1.num_int, &n);
                            position += n + 1;
                            array[cur_node].arg1.memory = 1;
                        }
                        else
                        {
                            sscanf (init_array + position, "{%d}%n", &array[cur_node].arg1.num_int, &n);
                            position += n + 1;
                        }

                        if (init_array[position + 1] == 's')
                        {
                            array[cur_node].arg2.stack = 1;
                            position += 8;
                        }
                        else if (init_array[position + 1] == 'A')
                        {
                            array[cur_node].arg2.reg = 1;
                            position += 5;
                        }
                        else if (init_array[position + 1] == '[')
                        {
                            sscanf (init_array + position, "{[%d]}%n", &array[cur_node].arg2.num_int, &n);
                            position += n + 1;
                            array[cur_node].arg2.memory = 1;
                        }
                        else
                        {
                            sscanf (init_array + position, "{%d}%n", &array[cur_node].arg2.num_int, &n);
                            position += n + 1;
                        }
                        break;
                    }
                    case IR_MATH_OPER:
                    {
                        sscanf (init_array + position, "%*[a-zA-Z{}]%n", &n);
                        init_array[position + n - 1] = '\0';

                        if (!strcmp (init_array + position + 1, "SUB"))
                            array[cur_node].arg1.num_int = OP_SUB;
                        else if (!strcmp (init_array + position + 1, "ADD"))
                            array[cur_node].arg1.num_int = OP_ADD;
                        else if (!strcmp (init_array + position + 1, "MUL"))
                            array[cur_node].arg1.num_int = OP_MUL;
                        else if (!strcmp (init_array + position + 1, "DIV"))
                            array[cur_node].arg1.num_int = OP_DIV;

                        position += n + 1;
                        break;
                    }
                    case IR_JMP:
                    {
                        sscanf (init_array + position, "%*[a-zA-Z{}1234567890]%n", &n);
                        init_array[position + n - 1] = '\0';
                        if (!strcmp (init_array + position + 1, "JA"))
                            array[cur_node].arg1.num_int = JA;
                        else if (!strcmp (init_array + position + 1, "JE"))
                            array[cur_node].arg1.num_int = JE;
                        else if (!strcmp (init_array + position + 1, "JB"))
                            array[cur_node].arg1.num_int = JB;
                        else if (!strcmp (init_array + position + 1, "JBE"))
                            array[cur_node].arg1.num_int = JBE;
                        else if (!strcmp (init_array + position + 1, "JAE"))
                            array[cur_node].arg1.num_int = JAE;
                        else if (!strcmp (init_array + position + 1, "JNE"))
                            array[cur_node].arg1.num_int = JNE;    
                        else 
                            array[cur_node].arg1.num_int = ALWAYS;                
                        
                        position += n + 1;

                        sscanf (init_array + position, "%*[a-zA-Z{}1234567890]%n", &n);
                        init_array[position + n - 1] = '\0';
                        array[cur_node].arg2.num_str = init_array + position + 1;
                        position += n + 1;

                        break;                       
                    }
                    case IR_LABEL:
                    {
                        sscanf (init_array + position, "%*[a-zA-Z{}1234567890]%n", &n);
                        init_array[position + n - 1] = '\0';
                        array[cur_node].arg1.num_str = init_array + position + 1;
                        position += n + 1;   
                        break;
                    }
                    case IR_PUT_ARG_FUNC:
                    {
                        sscanf (init_array + position, "{[%d]}%n", &array[cur_node].arg1.num_int, &n);
                        position += n + 1;
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        cur_node++;
    }

    list_ir->size_list = cur_node;
}