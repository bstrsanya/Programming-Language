#!/bin/bash
TREE_NAME=tree.txt
ASM_FILE=asm.txt
BIN_FILE=asm.bin
IR_FILE=IR.txt
NASM_FILE=new_asm.s
ELF_FILE=my_elf

# ./frontend $1 ${TREE_NAME}
# ./backend ${TREE_NAME} ${ASM_FILE}
# ./Processor/asm ${ASM_FILE} ${BIN_FILE}
# ./Processor/pro ${BIN_FILE}

./frontend $1 ${TREE_NAME}
./back ${TREE_NAME} ${IR_FILE}
./ir "elf64" ${IR_FILE} ${ELF_FILE}
# nasm -f elf64 -o asm.o ${NASM_FILE}
# ld -o elf_asm asm.o
# ./elf_asm


