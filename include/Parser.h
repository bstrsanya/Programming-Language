#ifndef PARSER_H
#define PARSER_H

#include "ReadFile.h"
#include "SizeFile.h"

const int SIZE_ARRAY = 100;
const int LEN_STR    = 10;
const char FILE_EXPRESSION[] = "test_example.txt";
const char FILE_LATEX[]      = "INPUT";
const double SMALL    = 10e-8;
const int NUM_COMMAND = 17;
const int NUM_PHRASES = 5;

enum type_com
{
    NUM = 1,
    VAR = 2,
    OP  = 3,
    MATH_CONST = 4,
    FUNC = 5
};

struct Node_t 
{
    type_com type;
    double value;
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
};

enum command
{
    F_COS   = 1,
    F_SIN   = 2,
    F_TAN   = 3,
    F_CTG   = 4,
    F_LN    = 5,
    F_ADD   = 43,
    F_SUB   = 45,
    F_MUL   = 42,
    F_DIV   = 47,
    F_DEG   = 94,
    F_VAR   = 120,
    F_OPEN  = 40,
    F_CLOSE = 41,
    F_E     = 101,
    F_EQU   = 61,
    F_IF    = 6,
    F_FUNC  = 7,
    F_CURLY_BRACE_OPEN  = 123,
    F_CURLY_BRACE_CLOSE = 125
};

struct Command_t
{
    const char* name;
    command n_com;
    type_com t_com;
};

void ReadDataBase (Tree_t* tree);
Node_t** CreateTokens (char* str);
Node_t* GetG (int* pointer, Node_t** array);
Node_t* GetN (int* pointer, Node_t** array);
Node_t* GetE (int* pointer, Node_t** array);
Node_t* GetT (int* pointer, Node_t** array);
Node_t* GetP (int* pointer, Node_t** array);
Node_t* GetS (int* pointer, Node_t** array);
void FindCommand (char* com, type_com* com_type, int* com_value);
Node_t* GetEqu (int* pointer, Node_t** array);
Node_t* GetFunc (int* pointer, Node_t** array);

Node_t* NodeCtor (int type, double value, Node_t* left, Node_t* right);
void NodeDtor (Node_t* node);
void TreeCtor (Tree_t* tree, const char* name_file);
void TreeDtor (Tree_t* tree);

void CreateDot (Node_t* node, FILE* file_dot);
void CreateDotUSER (Node_t* node, FILE* file_dot);
void PrintDot (Node_t* node, const char* file_input);

#endif