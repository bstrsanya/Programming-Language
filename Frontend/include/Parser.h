#ifndef PARSER_H
#define PARSER_H

#include "ReadFile.h"
#include "SizeFile.h"

const int SIZE_ARRAY = 100;
const int SIZE_TABLE_VAR = 10;
const char FILE_EXPRESSION[] = "test_example.txt";
const char FILE_LATEX[]      = "output.txt";
const double SMALL    = 10e-8;

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
Node_t* GetV (int* pointer, Node_t** array);
Node_t* GetO (int* pointer, Node_t** array);

Node_t** CreateArrayTokens ();

Node_t* NodeCtor (int type, double value, Node_t* left, Node_t* right);
void NodeDtor (Node_t* node);
void TreeCtor (Tree_t* tree, const char* name_file);
void TreeDtor (Tree_t* tree);

// void CreateDot (Node_t* node, FILE* file_dot);
void CreateDotUSER (Node_t* node, FILE* file_dot, Tree_t* tree);
void PrintDot (Node_t* node, const char* file_input, Tree_t* tree);

void CreateTreeTxt (Tree_t* tree);
void PrintTxt (Node_t* node, Tree_t* tree);

#endif