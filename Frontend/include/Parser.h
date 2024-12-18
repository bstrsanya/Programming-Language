#ifndef PARSER_H
#define PARSER_H

#include "ReadFile.h"
#include "SizeFile.h"
#include "General.h"

const int SIZE_ARRAY = 1000;
const int SIZE_TABLE_VAR = 100;
const char FILE_EXPRESSION[] = "test_example.txt";
const char FILE_LATEX[]      = "tree.txt";
const double SMALL    = 10e-8;

void ReadDataBase (Tree_t* tree);
void Tokenization (Tree_t* tree, char* buffer);
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
void GetProg (int* pointer, Node_t** array, Node_t* main_value);
Node_t* GetF (int* pointer, Node_t** array);

Node_t** CreateArrayTokens ();

Node_t* NodeCtor (int type, double value, Node_t* left, Node_t* right);
void NodeDtor (Node_t* node);
void TreeCtor (Tree_t* tree, const char* name_file);
void TreeDtor (Tree_t* tree);

// void CreateDot (Node_t* node, FILE* file_dot);

void CreateTreeTxt (Tree_t* tree);
void PrintTxt (Node_t* node, Tree_t* tree);

char* CreateStr (char* main_str, int len, Tree_t* tree);

#endif