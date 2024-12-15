#ifndef PARSER_H
#define PARSER_H

#include "ReadFile.h"
#include "SizeFile.h"

const int SIZE_ARRAY = 100;
const int SIZE_TABLE_VAR = 10;
const char FILE_EXPRESSION[] = "test_example.txt";
const char FILE_LATEX[]      = "INPUT.txt";
const double SMALL    = 10e-8;

enum TypeCommand_t
{
    NUM = 1,
    VAR = 2,
    OP  = 3,
    FUNC = 5,
    BLOCK = 6,
    INVALID_TYPE = -1
};

enum ListCommand_t
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
    F_BRACE_OPEN  = 40,
    F_BRACE_CLOSE = 41,
    F_E     = 101,
    F_ASSIGNMENT   = 61,
    F_IF    = 6,
    F_FUNC  = 7,
    F_CURLY_BRACE_OPEN  = 123,
    F_CURLY_BRACE_CLOSE = 125,
    F_INTERRUPT = 59,
    F_EQUAL = 8,
    F_ELSE  = 9,
    F_INVALID = -1,
    F_WHILE = 10
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
    TypeCommand_t t_com;
};

void ReadDataBase (Tree_t* tree);
void Tokenization (Tree_t* tree);
Node_t* GetG (int* pointer, Node_t** array);
Node_t* GetN (int* pointer, Node_t** array);
Node_t* GetE (int* pointer, Node_t** array);
Node_t* GetT (int* pointer, Node_t** array);
Node_t* GetP (int* pointer, Node_t** array);
Node_t* GetS (int* pointer, Node_t** array);
void FindCommand (char* com, TypeCommand_t* com_type, int* com_value, Tree_t* tree);
Node_t* GetEqu (int* pointer, Node_t** array);
Node_t* GetFunc (int* pointer, Node_t** array);
Node_t* GetIf (int* pointer, Node_t** array);
void GetStop (int* pointer, Node_t** array, Node_t* main_value);
Node_t* GetWhile (int* pointer, Node_t** array);

Node_t** CreateArrayTokens ();

Node_t* NodeCtor (int type, double value, Node_t* left, Node_t* right);
void NodeDtor (Node_t* node);
void TreeCtor (Tree_t* tree, const char* name_file);
void TreeDtor (Tree_t* tree);

// void CreateDot (Node_t* node, FILE* file_dot);
void CreateDotUSER (Node_t* node, FILE* file_dot, Tree_t* tree);
void PrintDot (Node_t* node, const char* file_input, Tree_t* tree);

#endif