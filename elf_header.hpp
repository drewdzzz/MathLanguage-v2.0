#ifndef ELF_HEADER
#define ELF_HEADER

const int BUFFER_SIZE = 100;

const uint64_t LOAD_ADDR = 0x400000;

#pragma pack(push, 1)
struct ElfHeader {
    const              uint8_t  ELF_FORMAT[4]        = {0x7F, 'E','L','F'};  // format specification

    const              uint8_t  CLASS                = 2;                    // ELF64 
    const              uint8_t  NUMBER_FORMAT        = 1;                    // little endian

    const              uint8_t  ELF_HEADER_VER       = 1;                    // 
    const              uint8_t  OS_ABI               = 0;                    // System V

    const              uint64_t PADDING              = 0;                    // unused

    const              uint16_t  FILE_TYPE           = 2;                    // executable file
    const              uint16_t  INSTRUCTION_SET     = 0x3E;                 // x86_64 instruction set
    const              uint32_t  ELF_VERSION         = 1;                    

    /*MUST BE FILLED*/ uint64_t entry_addr           = 0;                    // initial rip
    const              uint64_t ph_offset            = sizeof(ElfHeader);    //  
    /*MUST BE FILLED*/ uint64_t SH_OFFSET            = 0;                    // no segment headers

    const              uint32_t FLAGS                = 0;                    // no flags 
    
    const              uint16_t  HEADER_SIZE         = sizeof(ElfHeader);   
    /*MUST BE FILLED*/ uint16_t  ph_size             = 0;
    const              uint16_t  PH_NUM              = 1;
    /*MUST BE FILLED*/ uint16_t  SH_SIZE             = 0;
    const              uint16_t  SH_NUM              = 3;
    const              uint16_t  SH_TABLE_IDX        = 0;                
};

struct ProgHeader {
    const              uint32_t SEGMENT_TYPE         = 1;                    // LOAD
    const              uint32_t FLAGS                = 7;                    // Readable and executable
    const              uint64_t P_OFFSET             = 0;                    //
    const              uint64_t P_VADDR              = LOAD_ADDR;            // load to LOAD_ADDR
    const              uint64_t P_RADDR              = 0;                    // real memory addr is unused
    /*MUST BE FILLED*/ uint64_t p_filesz             = 0;
    /*MUST BE FILLED*/ uint64_t p_memsz              = 0;
    const              uint64_t ALIGNMENT            = 0x1000;
};

struct SecHeader {
                       uint64_t null_sec[8]          = {};
                       
                       uint32_t data_name            = 0;
                       uint32_t data_type            = 1;  
                       uint64_t data_flags           = 3;
                       uint64_t data_addr            = LOAD_ADDR + sizeof(ElfHeader) + sizeof(ProgHeader);
                       uint64_t data_offset          = sizeof(ElfHeader) + sizeof(ProgHeader);
                       uint64_t data_size            = BUFFER_SIZE + 1;
                       uint64_t data_space[3]        = {};

                       uint32_t text_name            = 0;
                       uint32_t text_type            = 1;  
                       uint64_t text_flags           = 6;
                       uint64_t text_addr            = LOAD_ADDR + BUFFER_SIZE + sizeof(ElfHeader) + sizeof(ProgHeader) + 1; 
                       uint64_t text_offset          = BUFFER_SIZE + sizeof(ElfHeader) + sizeof(ProgHeader) + 1;
    /*MUST BE FILLED*/ uint64_t text_size            = 0;
                       uint64_t text_space[3]        = {};
};


#pragma pack(pop)

#endif 