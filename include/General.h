#ifndef GENERAL_H
#define GENERAL_H

constexpr Command_t array_command[] = {
    {"cos", F_COS  , OP},
    {"sin", F_SIN  , OP},
    {"tg" , F_TAN  , OP},
    {"ctg", F_CTG  , OP},
    {"ln" , F_LN   , OP},
    {"+"  , F_ADD  , OP},
    {"-"  , F_SUB  , OP},
    {"*"  , F_MUL  , OP},
    {"/"  , F_DIV  , OP},
    {"^"  , F_DEG  , OP},
    {"("  , F_BRACE_OPEN , OP},
    {")"  , F_BRACE_CLOSE, OP},
    {"="  , F_ASSIGNMENT  , OP},
    {"if" , F_IF   , OP},
    {"main", F_FUNC, FUNC},
    {"{"  , F_CURLY_BRACE_OPEN, OP},
    {"}"  , F_CURLY_BRACE_CLOSE, OP},
    {";"  , F_INTERRUPT, OP},
    {"==" , F_JE , OP},
    {"else", F_ELSE, OP},
    {"while", F_WHILE, OP},
    {"<"    , F_JB,   OP},
    {"<="   , F_JBE,   OP},
    {">"    , F_JA,   OP},
    {">="    , F_JAE,   OP},
    {"!="    , F_JNE,   OP},
    {"int"  , F_INT, OP},
    {"double", F_DOUBLE, OP}
};

const int NUM_COMMAND = sizeof (array_command) / sizeof (array_command[0]);

#endif