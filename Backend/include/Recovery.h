#ifndef RECOVERY_H
#define RECOVERY_H

#include "General.h"
#include "ReadFile.h"
#include "SizeFile.h"

const char INPUT_FILE[] = "tree.txt";
const char ASM_FILE[]   = "asm.txt";
const int SIZE_ARRAY = 1000;

void TreeCtor (Tree_t* tree, const char* name_file);
Node_t* NodeCtor (int type, double value, Node_t* left, Node_t* right);
void NodeDtor (Node_t* node);
void TreeDtor (Tree_t* tree);
void ReadDataBase (Tree_t* tree);
Node_t** CreateArrayTokens ();
void Tokenization (Tree_t* tree);
Node_t* GetP (int* pointer, Node_t** array);
void CreateAsmFile (Node_t* node, Tree_t* tree);

#endif