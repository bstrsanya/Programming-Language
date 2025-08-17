#ifndef RECOVERY_H
#define RECOVERY_H

#include "General.h"
#include "ReadFile.h"
#include "SizeFile.h"

#include "Elf64.h"

const int SIZE_ARRAY    = 10000;
const int SIZE_TABLE_VAR = 100;
const int SIZE_ARRAY_PAIR = 100;
const int BUFFER = 30;
const int ADDR_BEGIN_SIZE_CODE = 0x60;
const int SIZE_BYTE = 8;

void ReadFileIR (ListIR_t* list_ir);
void WriteNasm (ListIR_t* list);
void CreateElf (ListIR_t* list);
void WriteElf (ListIR_t* list);
void ListIRCtor (ListIR_t* list_ir, const char* name_file_input, const char* name_file_output);
void ListIRDtor (ListIR_t* list_ir);
void CopyLib (ListIR_t* list, const char* name_file);

void TreeCtor (Tree_t* tree, const char* name_file_input, const char* name_file_output);
Node_t* NodeCtor (int type, double value, Node_t* left, Node_t* right);
void TreeDtor (Tree_t* tree);
void ReadDataBase (Tree_t* tree);
Node_t** CreateArrayTokens ();
void ReadBuffer (Tree_t* tree);
Node_t* RecoveryTree (int* pointer, Node_t** array);
void WritingIR (Node_t* node, Tree_t* tree, ListIR_t* list_ir);
void ChangeSign (Node_t* node, Tree_t* tree, ListIR_t* list_ir);
void CreateIR (Tree_t* tree, ListIR_t* list_ir);

void Dump (ListIR_t* list_ir);

#endif