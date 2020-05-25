#include "FreeAccessStack.hpp"
#include "differ-tree.hpp"
#include "nasm_commands.hpp"
#include "elf_header.hpp"


void elf_operator (FILE* stream, CalcTree::Node_t *node);

void elf_cond_operator (FILE* stream, CalcTree::Node_t *node);

void write_to_elf (const CalcTree &code);

void elf_undertree (FILE* stream, CalcTree::Node_t *node);

void elf_un_function (FILE* stream, CalcTree::Node_t *node);

void set_call_addr(char* buffer);

bool elf_return_value = false;

const int PROG_MAX_SIZE = 100000;

char CODE_BUF[PROG_MAX_SIZE];
char* PROGRAMM_CODE = &CODE_BUF[0];

FreeAccessStack<uint64_t> calls;
FreeAccessStack<uint32_t> function_addr;


uint32_t main_addr = 0;
uint32_t SQRT_ADDR = 0;
uint32_t SCAN_ADDR = 0;
uint32_t PRINT_ADDR = 0;
uint32_t BUFFER_ADDR = 0;

void write_to_elf (const CalcTree &code)
{
    FILE* stream = fopen (OUTPUT_BYTES, "w");

    BUFFER_ADDR = CODE_POS;
    for (int i = 0; i < BUFFER_SIZE; ++i) {   //ВЫДЕЛИЛИ БУФФЕР
        *PROGRAMM_CODE++ = 0x00;
        ++CODE_POS;
    }

    *PROGRAMM_CODE++ = 0x0a;
    ++CODE_POS;

    for (int i = 0; i <= GLOBAL_VARIABLES_NUM; ++i) {
        mov_r10_const(PROGRAMM_CODE, 0);
        push_r10(PROGRAMM_CODE);
    } //СОЗДАЛИ МЕСТО ПОД ПЕРЕМЕННЫЕ ДЛЯ МЕЙНА И ФИКТИВНЫЙ АДРЕС ВОЗВРАТА


    calls.push_back(CODE_POS);
    empty_call(PROGRAMM_CODE, 0xff); 
    exit_syscall(PROGRAMM_CODE);


    PRINT_ADDR = PRINT_FUNC(PROGRAMM_CODE, BUFFER_ADDR + sizeof(ProgHeader) + sizeof(ElfHeader) + LOAD_ADDR);
    SCAN_ADDR  = SCAN_FUNC(PROGRAMM_CODE, BUFFER_ADDR + sizeof(ProgHeader) + sizeof(ElfHeader) + LOAD_ADDR);
    SQRT_ADDR = CODE_POS;
    SQRT_FUNC(PROGRAMM_CODE);

    elf_undertree (stream, code.head);

    DEBUG_PRINT(TRANSLATED WITHOUT CALL ADDRESSES);

    set_call_addr(&CODE_BUF[0]);

    DEBUG_PRINT(CALL ADDRESSES WERE SET);

    ElfHeader elf_h = {};
    ProgHeader prog_h = {};
    SecHeader sec_h = {};

    elf_h.entry_addr = prog_h.P_VADDR + sizeof(ElfHeader) + sizeof(ProgHeader) + BUFFER_SIZE + 1;
    elf_h.ph_size = sizeof(ProgHeader);
    elf_h.SH_OFFSET = CODE_POS + sizeof(ElfHeader) + sizeof(ProgHeader);
    elf_h.SH_SIZE = sizeof(SecHeader) / elf_h.SH_NUM;

    prog_h.p_filesz = sizeof(ElfHeader) + sizeof(ProgHeader) + CODE_POS + sizeof(SecHeader);
    prog_h.p_memsz = prog_h.p_filesz;

    sec_h.text_size = CODE_POS - (sec_h.text_offset - sizeof(ElfHeader) - sizeof(ProgHeader));

    char* elf_h_bytes = reinterpret_cast<char*>(&elf_h);
    fwrite(elf_h_bytes, 1, sizeof(ElfHeader), stream);
    char* prog_h_bytes = reinterpret_cast<char*>(&prog_h);
    fwrite(prog_h_bytes, 1, sizeof(ProgHeader), stream);
    fwrite(CODE_BUF, 1, CODE_POS, stream);

    char* sec_h_bytes = reinterpret_cast<char*>(&sec_h);
    fwrite(sec_h_bytes, 1, sizeof(SecHeader), stream);

    system("chmod +x elf_code");
    
    fclose (stream);
}



void elf_undertree (FILE* stream, CalcTree::Node_t *node)
{
    switch (node -> node_data.type)
    {
        case OPERATOR:

            elf_operator (stream, node);
            break;

        case QUANTITY:

            mov_r10_const(PROGRAMM_CODE, static_cast<long long> (node -> node_data.data.value * 1024));
            push_r10(PROGRAMM_CODE);
            break;

        case BLOCK:

            elf_undertree (stream, node -> left);
            if (node -> right) 
                elf_undertree (stream, node -> right);
            break;

        case MAIN_DEF:
            main_addr = CODE_POS;
            push_rbp(PROGRAMM_CODE);
            mov_rbp_rsp(PROGRAMM_CODE);

            elf_undertree (stream, node -> right);
            
            pop_rbp(PROGRAMM_CODE);
            ret(PROGRAMM_CODE);
            break;

        case VARIABLE:
            push_rbp_addr_plus(PROGRAMM_CODE, 16 + 8 * node -> node_data.data.code);
            break;

        case UN_FUNCTION:

            elf_un_function (stream, node);
            break;

        case COND_OPERATOR:
    
            elf_cond_operator (stream, node);
            break;

        case DEF:
            if (function_addr.size() <= node->node_data.data.code)
                function_addr.resize(node->node_data.data.code + 1);
            function_addr[node->node_data.data.code] = CODE_POS;

            push_rbp(PROGRAMM_CODE);
            mov_rbp_rsp(PROGRAMM_CODE);

            elf_undertree (stream, node -> right);

            pop_rbp(PROGRAMM_CODE);
            ret(PROGRAMM_CODE);
            break;

        case CALL:
        {
            DEBUG_PRINT (Вызываю функцию);

            Node* curr_node = node;
            int local_counter = 0;
            while (curr_node -> right)
            {
                ++local_counter;
                curr_node = curr_node -> right;
                elf_undertree (stream, curr_node -> left);
            }
            calls.push_back(CODE_POS);
            empty_call(PROGRAMM_CODE, node->node_data.data.code);
            for (int i = 0; i < local_counter; ++i)
                pop_r8(PROGRAMM_CODE);
            if (elf_return_value)
                push_rax(PROGRAMM_CODE);
                elf_return_value = false;
            break;
        }

        case ARGUMENT:
        {
            DEBUG_PRINT (Пишу использование аргумента);
            int number_of_def = -1;

            Node* parent_search = node;
            while (parent_search->node_data.type != DEF)
                parent_search = parent_search -> father;

            number_of_def = parent_search->node_data.data.code;

            push_rbp_addr_plus(PROGRAMM_CODE, 16 + 8*node -> node_data.data.code);
            break;
        }

        throw "CANNOT ASM THIS TYPE";
    } 
}

void elf_operator (FILE* stream, CalcTree::Node_t *node)
{
    switch (node->node_data.data.code) {
        case OPERATOR::ADD_OP: 
            elf_undertree (stream, node -> left);
            elf_undertree (stream, node -> right);
            
            pop_r10(PROGRAMM_CODE);
            pop_r11(PROGRAMM_CODE);
            add_r11_r10(PROGRAMM_CODE);
            push_r11(PROGRAMM_CODE);
            break;
        
        case OPERATOR::SUB_OP:
            elf_undertree (stream, node -> left);
            elf_undertree (stream, node -> right);
            
            pop_r10(PROGRAMM_CODE);
            pop_r11(PROGRAMM_CODE);
            sub_r11_r10(PROGRAMM_CODE);
            push_r11(PROGRAMM_CODE);
            break;
        
        case OPERATOR::DIV_OP:
            elf_undertree (stream, node -> left);
            elf_undertree (stream, node -> right);
            pop_r8(PROGRAMM_CODE);
            pop_rax(PROGRAMM_CODE);
            cqo(PROGRAMM_CODE);
            idiv_r8(PROGRAMM_CODE);
            mov_r9_rdx(PROGRAMM_CODE);
            xor_rdx_rdx(PROGRAMM_CODE);
            mov_r10_const(PROGRAMM_CODE, 1024);
            imul_r10(PROGRAMM_CODE);
            mov_r11_rax(PROGRAMM_CODE);
            mov_rax_r9(PROGRAMM_CODE);
            shr_r8(PROGRAMM_CODE, 10);
            cqo(PROGRAMM_CODE);
            idiv_r8(PROGRAMM_CODE);
            add_rax_r11(PROGRAMM_CODE);
            push_rax(PROGRAMM_CODE); 
            break;

        case OPERATOR::MUL_OP:
            elf_undertree (stream, node -> left);
            elf_undertree (stream, node -> right);
            pop_rax(PROGRAMM_CODE);
            pop_rcx(PROGRAMM_CODE);
            imul_rcx(PROGRAMM_CODE);
            shr_rax(PROGRAMM_CODE, 10);
            shl_rdx(PROGRAMM_CODE, 54);
            or_rax_rdx(PROGRAMM_CODE);
            push_rax(PROGRAMM_CODE);
            break;

        case OPERATOR::ASSGN_OP:
            elf_undertree (stream, node -> right);
            if ( node->left->node_data.type != VARIABLE && node->left->node_data.type != ARGUMENT)
                throw "Могу присвоить только переменной!";
            pop_rbp_addr_plus(PROGRAMM_CODE, 16 + 8*node->left->node_data.data.code);
            break; 

        case OPERATOR::NOT_EQUAL_OP:
            elf_undertree (stream, node -> left);
            elf_undertree (stream, node -> right);

            pop_r10(PROGRAMM_CODE);
            pop_r11(PROGRAMM_CODE);
            cmp_r10_r11(PROGRAMM_CODE);

            jne_byte(PROGRAMM_CODE, 0x04);
            push_byte(PROGRAMM_CODE, 0);
            jmp_byte(PROGRAMM_CODE, 0x02);
            push_byte(PROGRAMM_CODE, 1);
            break;

        case OPERATOR::EQUAL_OP:
            elf_undertree (stream, node -> left);
            elf_undertree (stream, node -> right);

            pop_r10(PROGRAMM_CODE);
            pop_r11(PROGRAMM_CODE);
            cmp_r10_r11(PROGRAMM_CODE);
            je_byte(PROGRAMM_CODE, 0x04);
            push_byte(PROGRAMM_CODE, 0);
            jmp_byte(PROGRAMM_CODE, 0x02);
            push_byte(PROGRAMM_CODE, 1);
            break;

        case OPERATOR::LESS_OP:
            elf_undertree (stream, node -> left);
            elf_undertree (stream, node -> right);
            pop_r10(PROGRAMM_CODE);
            pop_r11(PROGRAMM_CODE);
            cmp_r11_r10(PROGRAMM_CODE);
            jl_byte(PROGRAMM_CODE, 0x04);
            push_byte(PROGRAMM_CODE, 0);
            jmp_byte(PROGRAMM_CODE, 0x02);
            push_byte(PROGRAMM_CODE, 1);
            break; 

        
        case OPERATOR::MORE_OP:
            elf_undertree (stream, node -> left);
            elf_undertree (stream, node -> right);
            pop_r10(PROGRAMM_CODE);
            pop_r11(PROGRAMM_CODE);
            cmp_r11_r10(PROGRAMM_CODE);
            jg_byte(PROGRAMM_CODE, 0x04);
            push_byte(PROGRAMM_CODE, 0);
            jmp_byte(PROGRAMM_CODE, 0x02);
            push_byte(PROGRAMM_CODE, 1);
            break; 

        default:
            throw "CANNOT ELF THIS OPERATOR";
            break;
    }

    DEBUG_CODE(printf ("%s\n", operators[node->node_data.data.code]));

}


void elf_cond_operator (FILE* stream, CalcTree::Node_t *node) 
{
    switch (node->node_data.data.code) {

        case COND_OPERATOR::IF_COND_OP:
        {
            elf_undertree (stream, node->left);
            pop_r10(PROGRAMM_CODE);
            xor_r11_r11(PROGRAMM_CODE);
            cmp_r10_r11(PROGRAMM_CODE);
            uint64_t jmp_pos = CODE_POS + 1;
            je_byte(PROGRAMM_CODE, 0);
            elf_undertree (stream, node->right);
            CODE_BUF[jmp_pos] = CODE_POS - (jmp_pos+1);
            break;
        }

        case COND_OPERATOR::WHILE_COND_OP:
        {
            uint64_t while_begin = CODE_POS;
            elf_undertree (stream, node->left);
            pop_r10(PROGRAMM_CODE);
            xor_r11_r11(PROGRAMM_CODE);
            cmp_r10_r11(PROGRAMM_CODE);
            uint64_t jmp_pos = CODE_POS + 1;
            je_byte(PROGRAMM_CODE, 0);
            elf_undertree (stream, node->right);
            jmp_byte(PROGRAMM_CODE, while_begin);
            CODE_BUF[jmp_pos] = CODE_POS - (jmp_pos+1);
            break;
        }
        default:
            throw "Cannot elf this conditional operator"; 
    } 
}




void elf_un_function (FILE* stream, CalcTree::Node_t *node)
{

    switch (node->node_data.data.code) {
        case UN_FUNCTION::PRINT_UN_FUNC:
            elf_undertree (stream, node->right);
            pop_r8(PROGRAMM_CODE);

            union{
                uint32_t val;
                char bytes[4];
            } print_add;

            print_add.val = PRINT_ADDR - (CODE_POS + 5) ;
            *PROGRAMM_CODE++ = 0xe8;
            ++CODE_POS;
            for (int i = 0; i < 4; ++i) {
                *PROGRAMM_CODE++ = print_add.bytes[i];
                ++CODE_POS;
            }
            break;

        case UN_FUNCTION::SQRT_UN_FUNC:
            elf_undertree (stream, node->right);
            pop_rax(PROGRAMM_CODE);

            union{
                uint32_t val;
                char bytes[4];
            } sqrt_add;

            sqrt_add.val = SQRT_ADDR - (CODE_POS + 5) ;
            *PROGRAMM_CODE++ = 0xe8;
            ++CODE_POS;
            for (int i = 0; i < 4; ++i) {
                *PROGRAMM_CODE++ = sqrt_add.bytes[i];
                ++CODE_POS;
            }
            
            push_rax(PROGRAMM_CODE);
            break;  

        case UN_FUNCTION::RETURN_UN_FUNC:
            elf_undertree (stream, node->right);
            if (node->right) {
                pop_rax(PROGRAMM_CODE);
                elf_return_value = true;
            }   
            pop_rbp(PROGRAMM_CODE);
            ret(PROGRAMM_CODE);
            break;

        case UN_FUNCTION::INPUT_UN_FUNC:
            union{
                uint32_t val;
                char bytes[4];
            } scan_add;

            scan_add.val = SCAN_ADDR - (CODE_POS + 5) ;
            *PROGRAMM_CODE++ = 0xe8;
            ++CODE_POS;
            for (int i = 0; i < 4; ++i) {
                *PROGRAMM_CODE++ = scan_add.bytes[i];
                ++CODE_POS;
            }
            
            *PROGRAMM_CODE++ = 0x48;
            *PROGRAMM_CODE++ = 0x89;
            *PROGRAMM_CODE++ = 0x45;
            *PROGRAMM_CODE++ = 16 + 8*node->right->node_data.data.code;

            CODE_POS += 4;

            if (node->right->node_data.type != VARIABLE)
                throw "Могу считать только в переменную!";
            break;

        default:
            throw "Не могу ELF эту унарную функцию";

    }
}




void set_call_addr(char* buffer) {
    int call_num = calls.size();
    uint32_t ptr = 0;
    int32_t addr = 0;
    char* value = nullptr;
    char def_num = 0;


    for(int j = 0; j < call_num; ++j) {
        ptr = calls[j];
        ++ptr;
        if (buffer[ptr] == static_cast<char>(0xff)) {
            addr = main_addr;
        } else {
            def_num = buffer[ptr];
            addr = function_addr[def_num];
        }
        addr = addr - (ptr + 4);
        char* value = (char*)&addr;
        for (int i = 0; i < 4; ++i)
            buffer[ptr++] = *value++;         
    }
}