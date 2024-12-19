#ifndef RECOVERY_H
#define RECOVERY_H

#include "General.h"
#include "ReadFile.h"
#include "SizeFile.h"

const int SIZE_ARRAY    = 10000;
const int SIZE_TABLE_VAR = 100;

void TreeCtor (Tree_t* tree, const char* name_file_input, const char* name_file_output);
Node_t* NodeCtor (int type, double value, Node_t* left, Node_t* right);
void TreeDtor (Tree_t* tree);
void ReadDataBase (Tree_t* tree);
Node_t** CreateArrayTokens ();
void ReadBuffer (Tree_t* tree);
Node_t* RecoveryTree (int* pointer, Node_t** array);
void WritingAsmFile (Node_t* node, Tree_t* tree);
void ChangeSign (Node_t* node, Tree_t* tree);
void CreateAsmFile (Tree_t* tree);


#endif