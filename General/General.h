#ifndef GENERAL_H
#define GENERAL_H

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

    F_FUNC   = 51,
    F_INT    = 52,
    F_DOUBLE = 53,
    F_BEGIN_FUNC = 54,
    F_PRINT = 55
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
    Node_t* expression_diff;
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


constexpr Command_t array_command[] = {
    {"cos"   , F_COS},
    {"sin"   , F_SIN},
    {"tg"    , F_TAN},
    {"ctg"   , F_CTG},
    {"ln"    , F_LN},
    {"+"     , F_ADD},
    {"-"     , F_SUB},
    {"*"     , F_MUL},
    {"/"     , F_DIV},
    {"^"     , F_DEG},
    {"("     , F_BRACE_OPEN},
    {")"     , F_BRACE_CLOSE},
    {"="     , F_ASSIGNMENT},
    {"if"    , F_IF},
    {"{"     , F_CURLY_BRACE_OPEN},
    {"}"     , F_CURLY_BRACE_CLOSE},
    {";"     , F_INTERRUPT},
    {"=="    , F_JE},
    {"else"  , F_ELSE},
    {"while" , F_WHILE},
    {"<"     , F_JB},
    {"<="    , F_JBE},
    {">"     , F_JA},
    {">="    , F_JAE},
    {"!="    , F_JNE},
    {"int"   , F_INT},
    {"double", F_DOUBLE},
    {"()"    , F_BEGIN_FUNC},
    {"print" , F_PRINT}
};

const int NUM_COMMAND = sizeof (array_command) / sizeof (array_command[0]);

#endif