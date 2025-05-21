#ifndef RECOVERY_H
#define RECOVERY_H

#include "General.h"
#include "ReadFile.h"
#include "SizeFile.h"

#include "Elf64.h"

const int SIZE_ARRAY    = 10000;
const int SIZE_TABLE_VAR = 100;

void ReadFileIR (ListIR_t* list_ir);
void WriteNasm (ListIR_t* list);
void CreateElf (ListIR_t* list);
void WriteElf (ListIR_t* list);
void ListIRCtor (ListIR_t* list_ir, const char* name_file_input, const char* name_file_output);
void ListIRDtor (ListIR_t* list_ir);

#endif