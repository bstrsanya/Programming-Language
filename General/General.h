#ifndef GENERAL_H
#define GENERAL_H

#include <stdint.h>

enum TypeCommand_t
{
    NUM = 1,               // CONSTANT
    VAR = 2,               // IDENTIFIER
    OP  = 3,               // KEYWORD
    FUNC = 5,
    BLOCK = 6,
    INVALID_TYPE = -1
};

enum ListCommand_t 
{
    F_INVALID    = -1,

    F_IF         = 11,
    F_WHILE      = 12,
    F_ELSE       = 13,
    F_ASSIGNMENT = 14,

    F_SIN        = 21,
    F_COS        = 22,
    F_TAN        = 23,
    F_CTG        = 24,
    F_ADD        = 25,
    F_SUB        = 26,
    F_MUL        = 27,
    F_DIV        = 28,
    F_LN         = 29,
    F_DEG        = 30,

    F_JE         = 31,
    F_JB         = 32,
    F_JA         = 33,
    F_JBE        = 34,
    F_JAE        = 35,
    F_JNE        = 36,

    F_INTERRUPT         = 41,
    F_BRACE_OPEN        = 42,
    F_BRACE_CLOSE       = 43,
    F_CURLY_BRACE_OPEN  = 44,
    F_CURLY_BRACE_CLOSE = 45,

    F_FUNC        = 51,
    F_INT         = 52,
    F_DOUBLE      = 53,
    F_PRINT       = 55,
    F_UNDERLINING = 56,
    F_INPUT       = 57,
    F_SQRT        = 58,
    F_PRINT_C     = 59,

    F_RETURN      = 61
};

struct Node_t 
{
    TypeCommand_t type;

    struct {
        double number;
        ListCommand_t com;
        int var;
    } value;
    
    Node_t* left;
    Node_t* right;
};

struct Tree_t
{
    Node_t* expression;
    FILE* output;
    FILE* input;
    Node_t** array;
    char* read_data;
    char** table_var;
};

struct Command_t
{
    const char* name;
    ListCommand_t n_com;
};


constexpr Command_t array_command[] = 
{
    {"invalid com", F_INVALID          },
    {"cos"        , F_COS              },
    {"sin"        , F_SIN              },
    {"tg"         , F_TAN              },
    {"ctg"        , F_CTG              },
    {"ln"         , F_LN               },
    {"+"          , F_ADD              },
    {"-"          , F_SUB              },
    {"*"          , F_MUL              },
    {"/"          , F_DIV              },
    {"^"          , F_DEG              },
    {"("          , F_BRACE_OPEN       },
    {")"          , F_BRACE_CLOSE      },
    {"="          , F_ASSIGNMENT       },
    {"if_alive"   , F_IF               },
    {"udos"       , F_CURLY_BRACE_OPEN },
    {"peresda"    , F_CURLY_BRACE_CLOSE},
    {";"          , F_INTERRUPT        },
    {"=="         , F_JE               },
    {"dead"       , F_ELSE             },
    {"when_good"  , F_WHILE            },
    {"<"          , F_JB               },
    {"<="         , F_JBE              },
    {">"          , F_JA               },
    {">="         , F_JAE              },
    {"!="         , F_JNE              },
    {"int"        , F_INT              },
    {"double"     , F_DOUBLE           },
    {"output"     , F_PRINT            },
    {"_"          , F_UNDERLINING      },
    {"input"      , F_INPUT            },
    {"sqrt"       , F_SQRT             },
    {"symbol"     , F_PRINT_C          },
    {"return"     , F_RETURN           }
};

const int NUM_COMMAND = sizeof (array_command) / sizeof (array_command[0]);

void CreateDotUSER (Node_t* node, FILE* file_dot, Tree_t* tree);
void PrintDot (Tree_t* tree, const char* file_input);

enum Name
{
    IR_START          = 1,
    IR_END            = 2,
    IR_BEGIN_FUNC_DEF = 3,
    IR_END_FUNC_DEF   = 4,
    IR_CALL_FUNC      = 5,
    IR_RET            = 6,
    IR_MOV            = 7,
    IR_CMP            = 8,
    IR_MATH_OPER      = 9,
    IR_JMP            = 10,
    IR_LABEL          = 11,
    IR_GET_ARG        = 12,
    IR_PUT_ARG_FUNC   = 13,
    IR_FREE_ARG     = 14
};

struct Command_t_ir
{
    const char* name;
    Name n_com;
};

constexpr Command_t_ir array_command_ir[] = 
{
    {"START",          IR_START         },
    {"END",            IR_END           },
    {"BEGIN_FUNC_DEF", IR_BEGIN_FUNC_DEF},
    {"END_FUNC_DEF",   IR_END_FUNC_DEF  },
    {"CALL_FUNC",      IR_CALL_FUNC     },
    {"RET",            IR_RET           },
    {"MOV",            IR_MOV           },
    {"CMP",            IR_CMP           },
    {"MATH_OPER",      IR_MATH_OPER     },
    {"JMP",            IR_JMP           },
    {"LABEL",          IR_LABEL         },
    {"GET_ARG",        IR_GET_ARG       },
    {"FREE_ARG",       IR_FREE_ARG    },
    {"PUT_ARG_FUNC",   IR_PUT_ARG_FUNC  }
};

const int NUM_COMMAND_IR = sizeof (array_command_ir) / sizeof (array_command_ir[0]);

struct Arg
{
    ListCommand_t com;
    int num_int;
    char* num_str;
    char stack;
    char memory;
    char reg;
};

struct Node_IR 
{
    Name num;
    Arg arg1;
    Arg arg2;
};

struct ListIR_t
{
    FILE* input;
    FILE* output;
    char* data;
    Node_IR* list;
    int size_list;
    
    uint8_t* byte;
    int size_byte;

    const char* name_output_file;
    uint8_t* asm_code;
    int size_asm_code;
    char* buffer_for_label;
    int pointer_label;
};

struct pair
{
    char* str;
    int index;
};

#endif