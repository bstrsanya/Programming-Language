#ifndef GENERAL_H
#define GENERAL_H

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