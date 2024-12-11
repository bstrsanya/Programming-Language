#ifndef GENERAL_H
#define GENERAL_H

constexpr Command_t array_command[] = {
    {"cos", F_COS  , OP},
    {"sin", F_SIN  , OP},
    {"tg" , F_TAN  , OP},
    {"ctg", F_CTG  , OP},
    {"ln" , F_LN  , OP},
    {"+"  , F_ADD  , OP},
    {"-"  , F_SUB  , OP},
    {"*"  , F_MUL  , OP},
    {"/"  , F_DIV  , OP},
    {"^"  , F_DEG  , OP},
    {"x"  , F_VAR  , VAR},
    {"("  , F_OPEN , OP},
    {")"  , F_CLOSE, OP},
    {"e"  , F_E    , MATH_CONST}
};

#endif