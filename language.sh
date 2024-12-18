#!/bin/bash
TREE_NAME=tree.txt
ASM_FILE=asm.txt
BIN_FILE=asm.bin

./frontend $1 ${TREE_NAME}
./backend ${TREE_NAME} ${ASM_FILE}
./Processor/asm ${ASM_FILE} ${BIN_FILE}
./Processor/pro ${BIN_FILE}