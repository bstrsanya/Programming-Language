#ifndef ELF64_H
#define ELF64_H

#include <stdint.h>

#define PUSH_REG(ARG, ADDR, PTR)  \
    do {                          \
        ADDR[PTR++] = 0x50 + ARG; \
    } while (0)

#define POP_REG(ARG, ADDR, PTR)   \
    do {                          \
        ADDR[PTR++] = 0x58 + ARG; \
    } while (0)

#define CALLING_FUNCTION(ADDR, PTR) \
    do {                            \
        ADDR[PTR++] = 0xe8;         \
        ADDR[PTR++] = 0x00;         \
        ADDR[PTR++] = 0x00;         \
        ADDR[PTR++] = 0x00;         \
        ADDR[PTR++] = 0x00;         \
    } while (0) 

#define CMP_REG_REG(REG1, REG2, ADDR, PTR) \
    do {                                   \
        ADDR[PTR++] = 0x48;                \
        ADDR[PTR++] = 0x39;                \
        uint8_t temp = 0;                  \
        temp |= REG2;                      \
        temp = (uint8_t)(temp << 3);       \
        temp |= REG1;                      \
        temp |= 0b11000000;                \
        ADDR[PTR++] = temp;                \
    } while (0)

#define MOV_REG_REG(REG1, REG2, ADDR, PTR) \
    do {                                   \
        ADDR[PTR++] = 0x48;                \
        ADDR[PTR++] = 0x89;                \
        uint8_t temp = 0;                  \
        temp |= REG2;                      \
        temp = (uint8_t)(temp << 3);       \
        temp |= REG1;                      \
        temp |= 0b11000000;                \
        ADDR[PTR++] = temp;                \
    } while (0)

#define MOV_REG_MEM(REG1, REG2, SHIFT, ADDR, PTR) \
    do {                                          \
        ADDR[PTR++] = 0x48;                       \
        ADDR[PTR++] = 0x8b;                       \
        uint8_t temp = 0;                         \
        temp |= REG1;                             \
        temp = (uint8_t)(temp << 3);              \
        temp |= REG2;                             \
        temp |= 0b01000000;                       \
        ADDR[PTR++] = temp;                       \
        ADDR[PTR++] = SHIFT;                      \
    } while (0)

#define MOV_MEM_REG(REG1, SHIFT, REG2, ADDR, PTR) \
    do {                                          \
        ADDR[PTR++] = 0x48;                       \
        ADDR[PTR++] = 0x89;                       \
        uint8_t temp = 0;                         \
        temp |= REG2;                             \
        temp = (uint8_t)(temp << 3);              \
        temp |= REG1;                             \
        temp |= 0b01000000;                       \
        ADDR[PTR++] = temp;                       \
        ADDR[PTR++] = SHIFT;                      \
    } while (0)

#define MOV_REG_NUM(REG, NUM, ADDR, PTR)               \
    do {                                               \
        ADDR[PTR++] = 0xB8 + REG;                      \
        for (int shift = 0; shift < 4; shift++)        \
            ADDR[PTR++] = (NUM >> (8 * shift)) & 0xFF; \
    } while (0) 

#define MOV_MEM_NUM(REG, SHIFT, NUM, ADDR, PTR)        \
    do {                                               \
        ADDR[PTR++] = 0x48;                            \
        ADDR[PTR++] = 0xc7;                            \
        uint8_t temp = 0;                              \
        temp |= REG;                                   \
        temp |= 0b01000000;                            \
        ADDR[PTR++] = temp;                            \
        ADDR[PTR++] = SHIFT;                           \
        for (int shift = 0; shift < 4; shift++)        \
            ADDR[PTR++] = (NUM >> (8 * shift)) & 0xFF; \
    } while (0)


#define PUSH_NUM(NUM, ADDR, PTR)                       \
    do {                                               \
        ADDR[PTR++] = 0x68;                            \
        for (int shift = 0; shift < 4; shift++)        \
            ADDR[PTR++] = (NUM >> (8 * shift)) & 0xFF; \
    } while (0) 

#define POP_MEM(REG, SHIFT, ADDR, PTR) \
    do {                               \
        ADDR[PTR++] = 0x8f;            \
        uint8_t temp = 0;              \
        temp |= REG;                   \
        temp |= 0b01000000;            \
        ADDR[PTR++] = temp;            \
        ADDR[PTR++] = SHIFT;           \
    } while (0)

#define PUSH_MEM(REG, SHIFT, ADDR, PTR) \
    do {                                \
        ADDR[PTR++] = 0xff;             \
        uint8_t temp = 0;               \
        temp |= REG;                    \
        temp |= 0b01110000;             \
        ADDR[PTR++] = temp;             \
        ADDR[PTR++] = SHIFT;            \
    } while (0)

#define RET_COM(ADDR, PTR)  \
    do {                    \
        ADDR[PTR++] = 0xc3; \
    } while (0)


#define MATH_OP_REG_NUM(OP, REG, NUM, ADDR, PTR) \
    do {                                         \
        ADDR[PTR++] = 0x48;                      \
        ADDR[PTR++] = 0x83;                      \
        uint8_t temp = 0;                        \
        temp |= OP;                              \
        temp = (uint8_t) (temp << 3);            \
        temp |= REG;                             \
        temp |= 0b11000000;                      \
        ADDR[PTR++] = temp;                      \
        ADDR[PTR++] = NUM;                       \
    } while (0)

#define JMP_COM(CONDITION, ADDR, PTR) \
    do {                              \
        if (CONDITION != ALWAYS)      \
            ADDR[PTR++] = 0x0f;       \
        ADDR[PTR++] = CONDITION;      \
        ADDR[PTR++] = 0x00;           \
        ADDR[PTR++] = 0x00;           \
        ADDR[PTR++] = 0x00;           \
        ADDR[PTR++] = 0x00;           \
    } while (0)        

#define MATH_OP_REG_REG(OP, REG1, REG2, ADDR, PTR) \
    do {                                           \
        if (OP == OP_ADD || OP == OP_SUB)          \
        {                                          \
            ADDR[PTR++] = 0x48;                    \
            ADDR[PTR++] = OP;                      \
            uint8_t temp = 0;                      \
            temp |= REG2;                          \
            temp = (uint8_t)(temp << 3);           \
            temp |= REG1;                          \
            temp |= 0b11000000;                    \
            ADDR[PTR++] = temp ;                   \
        }                                          \
        else                                       \
        {                                          \
            MOV_REG_REG (RAX, RBX, ADDR, PTR);     \
            ADDR[PTR++] = 0x48;                    \
            ADDR[PTR++] = 0xf7;                    \
            uint8_t temp = 0;                      \
            temp |= OP;                            \
            temp = (uint8_t)(temp << 3);           \
            temp |= REG2;                          \
            temp |= 0b11000000;                    \
            ADDR[PTR++] = temp;                    \
            MOV_REG_REG (RBX, RAX, ADDR, PTR);     \
        }                                          \
    } while (0)

enum REGISTERS_JUMP_OPERATIONS
{
    RAX = 0,
    RCX = 1,
    RDX = 2,
    RBX = 3,
    RSP = 4,
    RBP = 5,
    RSI = 6,
    RDI = 7,

    JB  =    0x8C, // jl (82)
    JAE =    0x8D, // jge (83)
    JE  =    0x84,
    JNE =    0x85,
    JBE =    0x8E, // jle
    JA  =    0x8F, // jg (87)
    ALWAYS = 0x9e,
    
    OP_ADD = 0x01,
    OP_SUB = 0x29,
    OP_MUL = 0x05, // было 0x04
    OP_DIV = 0x06
};

enum MATH_OPER_BYTE_CODE 
{
    ADD = 0,
    OR = 1,
    AND = 4,
    SUB = 5,
    XOR = 6,
    // CMP = 7
};

struct Elf64Header
    {
        uint8_t  ei_MAG0;           // magic number equ 0x7f
        uint8_t  ei_MAG3[3];        // ELF in ASCII (0x45 0x4c 0x46)
        uint8_t  ei_CLASS;          // 32-bit (1) or 64-bit (2) format
        uint8_t  ei_DATA;           // little (1) or big (2) endianness
        uint8_t  ei_VERSION;        // version of ELF (need 1 for original and current version)
        uint8_t  ei_OSABI;          // target operating system
        uint8_t  ei_ABIVERSION;     // it is ignored for statically linked executables
        uint8_t  ei_PAD[7];         // reserved padding bytes. should be filled with zeros and ignored when read
        uint16_t e_type;            // identifies object file type (ET_EXEC = 0x02)
        uint16_t e_machine;         // 0x3E AMD x86-64
        uint32_t e_version;         // set to 1 for the original version of ELF
        uint64_t e_entry;           // this is the memory address of the entry point from where the process starts executing
        uint64_t e_phoff;           // points to the start of the program header table
        uint64_t e_shoff;           // points to the start of the section header table, 0 if not present
        uint32_t e_flags;           // processor-specific flags (must be 0)
        uint16_t e_ehsize;          // contains the size of this header (64 bytes)
        uint16_t e_phentsize;       // contains the size of a program header table entry (0x38 bytes)
        uint16_t e_phnum;           // contains the number of entries in the program header table
        uint16_t e_shentsize;       // contains the size of a section header table entry (0x40 bytes) 
        uint16_t e_shnum;           // contains the number of entries in the section header table
        uint16_t e_shstrndx;        // Contains index of the section header table entry that contains the section names
    };

    struct Elf64_PhtEntry 
    {
        uint32_t p_type;            // Identifies the type of the segment (PT_LOAD = 1, PT_DYNAMIC = 2)
        uint32_t p_flags;           // Segment-dependent flags
        uint64_t p_offset;          // Offset of the segment in the file image
        uint64_t p_vaddr;           // Virtual address of the segment in memory
        uint64_t p_paddr; // on systems where the physical address is relevant, reserved for the physical address of the segment
        uint64_t p_filesz;          // Size in bytes of the segment in the file image
        uint64_t p_memsz;           // Size in bytes of the segment in memory
        uint64_t p_align;           // 0 and 1 specify no alignment. Otherwise should be a positive, integral power of 2, with 'vaddr' equating 'offset' modulus 'p_align'
    };


#endif // ELF64_H