    typedef struct {
        long offset; // offset of the reference to relocate
        long type:32, // relocation type
            symbol:32;
        long addend; // constant part of relocation expression
    } Elf64_Rela;