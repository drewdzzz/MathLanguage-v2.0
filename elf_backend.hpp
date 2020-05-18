#include <vector>
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

std::vector<uint64_t> calls;
std::vector<uint32_t> function_addr;
uint32_t main_addr = 0;

void write_to_elf (const CalcTree &code)
{
    FILE* stream = fopen (OUTPUT_BYTES, "w");


    
    /*fprintf (stream, ";==========INCLUDE DREW LIB==========\n\n"
                     "segment .data\n"
                     "BUFFER_SIZE equ 100\n"
                     "buffer times BUFFER_SIZE db 0\n"
                     "new_line db 0x0a\n"
                     );

    fprintf (stream, "segment .text\n\n");

    fprintf (stream,";==============================================================\n"
                    ";Entry:		    requires buffer and const BUFFER_SIZE\n"
                    ";				R8	- long long value\n"
                    ";Exit: 		    RDX - length of buffer\n"
                    ";				RSI - ptr to number in buffer\n"
                    ";				ES  = DS\n"
                    ";\n"
                    ";Destr:		    RCX RAX	R8D	R10B R9B\n"		
                    ";==============================================================\n"
                    "\tdec_format:\n"
                    "\tmov rsi, buffer\n"
                    "\tadd rsi, BUFFER_SIZE\n"
                    "\txor r10b, r10b\n"
                    "\tmov r9, 10\n"
                    "\tmov rcx, 19\n"
                    "\tmov rax, r8\n"
                    "\n"
                    "\t.dec_format_loop:\n"
                    "\tdec rsi\n"
                    "\tinc r10b\n"
                    "\n" 
                    "\txor rdx, rdx\n"
                    "\tdiv r9\n"
                    "\n"
                    "\tadd rdx, '0'\n"
                    "\tmov [rsi], dl \n"
                    "\tLOOP .dec_format_loop\n"
                    "\n"
                    "\txor rdx, rdx\n"
                    "\tmov dl, r10b\n"
                    "\n"
                    "\tmov rax, ds\n"
                    "\tmov es, rax\n"
                    "\tmov al, '0'\n"
                    "\tmov rcx, rdx\n"
                    "\tdec rcx\n"
                    "\t.skipping:\n"
                    "\tcmp al, [rsi]\n"
                    "\tjnz .break\n"
                    "\tinc rsi\n"
                    "\tdec rdx\n"
                    "\tloop .skipping\n"
                    "\t.break:\n"
                    "\tret\n\n\n"

                    "PRINT:\n"
                    "\txor rax, rax\n"
                    "\tcmp r8, rax\n"
                    "\tjge .positive\n"
                    "\n"
                    "\tmov rsi, buffer\n"
                    "\tmov rdx, 1\n"
                    "\tmov byte [rsi], '-'\n"
                    "\txor rax, rax\n"
                    "\tinc rax\n"
                    "\txor rdi, rdi\n"
                    "\tsyscall\n"
                    "\n"
                    "\tneg r8\n"
                    ".positive:\n"
                    "\tpush r8\n"
                    "\tshr r8, 10\n"
                    "\tcall dec_format\n"
                    "\txor rax, rax\n"
                    "\tinc rax\n"
                    "\txor rdi, rdi\n"
                    "\tsyscall\n"
                    "\tpop r8\n"
                    "\n"
                    "\tmov rsi, buffer\n"
                    "\tmov rdx, 1\n"
                    "\tmov byte [rsi], '.'\n"
                    "\txor rax, rax\n"
                    "\tinc rax\n"
                    "\txor rdi, rdi\n"
                    "\tsyscall\n"
                    "\n"
                    "\tand r8, 0x3FF\n"
                    "\tshl r8, 10\n"
                    "\tmov rax, r8\n"
                    "\tcqo\n"
                    "\tmov r8, 1000\n"
                    "\tidiv r8\n"
                    "\tmov r8, rax\n"
                    "\tcall dec_format\n"
                    "\txor rax, rax\n"
                    "\tinc rax\n"
                    "\txor rdi, rdi\n"
                    "\tinc rdx\n"
                    "\tsyscall\n"
                    "\tret\n\n\n"
                    

                    ";==============================================================================\n"
                    "; In:  rsi - buf to input\n"
                    "; Out: rdx - string len\n"
                    "; Destr: rdi, rax\n"
                    ";==============================================================================\n"
                    "gets:\n"
                    "\tpush rsi\n\n"
                        
                    "\tmov rdi, 0\n"
                    "\tmov rdx, 1\n\n"
                    
                    ".continue:\n"
                    "\tmov rax, 0\n"
                    "\tsyscall\n\n"

                    "\tcmp byte [rsi], 10d\n"
                    "\tje .exit\n\n"
                        
                    "\tinc rsi\n"
                    "\tjmp .continue\n\n"
                    
                    ".exit:\n"
                    "\tmov rdx, rsi\n"
                    "\tpop rsi\n"
                    "\tsub rdx, rsi\n\n"
                    
                    "\tret\n\n"
                    "\n"
                    "\n"
                    "toInt:\n"
                    "\txor r8, r8\n"
                    "\tmov rdi, rsi\n"
                    "\tadd rdi, rdx\n"
                    "\tmov rcx, 1\n"
                    "\tmov rbx, 10\n\n"
                    ".loop:\n"
                    "\tdec rdi\n"
                    "\tcmp byte [rdi], '-'\n"
                    "\tje .end\n"
                    "\tmov bl, [rdi]\n"
                    "\tsub bl, '0'\n"
                    "\tmov rax, rcx\n"
                    "\tmul bl\n"
                    "\tadd r8, rax\n"
                    "\tmov rax, rcx\n"
                    "\tmul rbx\n"
                    "\tmov rcx, rax\n"
                    "\tcmp rdi, rsi\n"
                    "\tjne .loop\n"
                    
                    ".end:\n"
                    "\tcmp byte [rdi], '-'\n"
                    "\tjne .positive\n"
                    "\tshl r8, 10\n"
                    "\tneg r8\n"
                    "\tjmp .neg_end\n"
                    ".positive:\n"
                    "\tshl r8, 10\n"
                    ".neg_end:\n"
                    "\tmov rax, r8\n"
                    "\tret\n\n\n"

                    "SCAN:\n"
                    "\tmov rsi, buffer\n"
                    "\tcall gets\n"
                    "\tmov rsi, buffer\n"
                    "\tcall toInt\n"
                    "\tret\n"
                    "\n"
                    "\n"
                    "\n"
                    "SQRT: \n"
                    "\txor rbx, rbx\n"
                    "\tbsr rcx, rax\n"
                    "\tand cl,  0x0fe\n"
                    "\tmov rdx, 1\n"
                    "\tshl rdx, cl\n"
                    ".refine:\n"
                    "\tmov rsi, rbx\n"
                    "\tadd rsi, rdx\n"
                    "\tcmp rsi, rax\n"
                    "\tja .@f\n"
                    "\tsub rax, rsi\n"
                    "\tshr rbx, 1\n"
                    "\tadd rbx, rdx\n"
                    "\tjmp .next\n"
                    ".@f:\n"
                    "\tshr rbx, 1\n"
                    ".next :\n"
                    "\tshr rdx, 2\n"
                    "\tjnz .refine\n"
                    "\tmov rax, rbx\n"
                    "\tshl rax, 5\n"
                    "\tret\n"
                    ";==========INCLUDE DREW LIB==========\n\n");*/

    for (int i = 0; i <= GLOBAL_VARIABLES_NUM; ++i) {
        mov_r10_const(PROGRAMM_CODE, 0);
        push_r10(PROGRAMM_CODE);
    } //СОЗДАЛИ МЕСТО ПОД ПЕРЕМЕННЫЕ ДЛЯ МЕЙНА И ФИКТИВНЫЙ АДРЕС ВОЗВРАТА


    calls.push_back(CODE_POS);
    empty_call(PROGRAMM_CODE, 0xff); 
    exit_syscall(PROGRAMM_CODE);
    elf_undertree (stream, code.head);

    DEBUG_PRINT(TRANSLATED WITHOUT CALL ADDRESSES);

    set_call_addr(&CODE_BUF[0]);

    DEBUG_PRINT(CALL ADDRESSES WERE SET);

    ElfHeader elf_h = {};
    ProgHeader prog_h = {};

    elf_h.entry_addr = prog_h.P_VADDR + sizeof(ElfHeader) + sizeof(ProgHeader);
    elf_h.ph_size = sizeof(ProgHeader);

    prog_h.p_filesz = sizeof(ElfHeader) + sizeof(ProgHeader) + CODE_POS;
    prog_h.p_memsz = prog_h.p_filesz;

    std::cout<<sizeof(ElfHeader) + sizeof(ProgHeader)<<'\n';

    char* elf_h_bytes = reinterpret_cast<char*>(&elf_h);
    fwrite(elf_h_bytes, 1, sizeof(ElfHeader), stream);
    char* prog_h_bytes = reinterpret_cast<char*>(&prog_h);
    fwrite(prog_h_bytes, 1, sizeof(ProgHeader), stream);
    fwrite(CODE_BUF, 1, CODE_POS, stream);

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
    char code = node->node_data.data.code;

    if (is_this_operator (code, ADD))
    {
        elf_undertree (stream, node -> left);
        elf_undertree (stream, node -> right);
        
        pop_r10(PROGRAMM_CODE);
        pop_r11(PROGRAMM_CODE);
        add_r11_r10(PROGRAMM_CODE);
        push_r11(PROGRAMM_CODE);
        return;
    }

    if (is_this_operator (code, SUB))
    {
        elf_undertree (stream, node -> left);
        elf_undertree (stream, node -> right);
        
        pop_r10(PROGRAMM_CODE);
        pop_r11(PROGRAMM_CODE);
        sub_r11_r10(PROGRAMM_CODE);
        push_r11(PROGRAMM_CODE);
        return;
    }

    if (is_this_operator (code, DIV))
    {
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
        return;
    }

    if (is_this_operator (code, MUL))
    { 
        elf_undertree (stream, node -> left);
        elf_undertree (stream, node -> right);
        pop_rax(PROGRAMM_CODE);
        pop_rcx(PROGRAMM_CODE);
        imul_rcx(PROGRAMM_CODE);
        shr_rax(PROGRAMM_CODE, 10);
        shl_rdx(PROGRAMM_CODE, 54);
        or_rax_rdx(PROGRAMM_CODE);
        push_rax(PROGRAMM_CODE);
        return;
    }

    if (is_this_operator (code, ASSGN))
    {
        elf_undertree (stream, node -> right);
        if ( node->left->node_data.type != VARIABLE && node->left->node_data.type != ARGUMENT)
            throw "Могу присвоить только переменной!";
        pop_rbp_addr_plus(PROGRAMM_CODE, 16 + 8*node->left->node_data.data.code);
        return;   
    }

    if (is_this_operator (code, NOT_EQUAL))
    {
        elf_undertree (stream, node -> left);
        elf_undertree (stream, node -> right);

        pop_r10(PROGRAMM_CODE);
        pop_r11(PROGRAMM_CODE);
        cmp_r10_r11(PROGRAMM_CODE);

        jne_byte(PROGRAMM_CODE, 0x04);
        push_byte(PROGRAMM_CODE, 0);
        jmp_byte(PROGRAMM_CODE, 0x02);
        push_byte(PROGRAMM_CODE, 1);

        return;
    }

    if (is_this_operator (code, EQUAL))
    {
        elf_undertree (stream, node -> left);
        elf_undertree (stream, node -> right);

        pop_r10(PROGRAMM_CODE);
        pop_r11(PROGRAMM_CODE);
        cmp_r10_r11(PROGRAMM_CODE);
        je_byte(PROGRAMM_CODE, 0x04);
        push_byte(PROGRAMM_CODE, 0);
        jmp_byte(PROGRAMM_CODE, 0x02);
        push_byte(PROGRAMM_CODE, 1);

        return; 
    }

    if (is_this_operator (code, LESS))
    {
        elf_undertree (stream, node -> left);
        elf_undertree (stream, node -> right);
        pop_r10(PROGRAMM_CODE);
        pop_r11(PROGRAMM_CODE);
        cmp_r11_r10(PROGRAMM_CODE);
        jl_byte(PROGRAMM_CODE, 0x04);
        push_byte(PROGRAMM_CODE, 0);
        jmp_byte(PROGRAMM_CODE, 0x02);
        push_byte(PROGRAMM_CODE, 1);
        return; 
    }

    if (is_this_operator (code, MORE))
    {
        elf_undertree (stream, node -> left);
        elf_undertree (stream, node -> right);
        pop_r10(PROGRAMM_CODE);
        pop_r11(PROGRAMM_CODE);
        cmp_r11_r10(PROGRAMM_CODE);
        jg_byte(PROGRAMM_CODE, 0x04);
        push_byte(PROGRAMM_CODE, 0);
        jmp_byte(PROGRAMM_CODE, 0x02);
        push_byte(PROGRAMM_CODE, 1);
        return; 
    }

    DEBUG_CODE(printf ("%s\n", operators[node->node_data.data.code]))
    throw "CANNOT ELF THIS OPERATOR";

}


void elf_cond_operator (FILE* stream, CalcTree::Node_t *node) 
{
    if ( is_this_cond_op (node->node_data.data.code, "Если" ) )
    {

        elf_undertree (stream, node->left);
        pop_r10(PROGRAMM_CODE);
        xor_r11_r11(PROGRAMM_CODE);
        cmp_r10_r11(PROGRAMM_CODE);
        uint64_t jmp_pos = CODE_POS + 1;
        je_byte(PROGRAMM_CODE, 0);
        elf_undertree (stream, node->right);
        CODE_BUF[jmp_pos] = CODE_POS - (jmp_pos+1);
    }
}

void elf_un_function (FILE* stream, CalcTree::Node_t *node)
{
    if ( is_this_un_func (node->node_data.data.code, PRINT) )
    {
        elf_undertree (stream, node->right);
        fprintf (stream,"\tpop r8\t\t\t\t\t\t\t\t; PRINT function\n" 
                        "\tcall PRINT\n\n");
        return;
    }
    
    else if ( is_this_un_func (node->node_data.data.code, SQRT) )
    {
        elf_undertree (stream, node->right);
        fprintf (stream, "\tpop rax\n"
                         "\tcall SQRT\n"
                         "\tpush rax\n");
        return;
    }
    else if ( is_this_un_func (node->node_data.data.code, RETURN) )
    {
        elf_undertree (stream, node->right);
        if (node->right) {
            pop_rax(PROGRAMM_CODE);
            elf_return_value = true;
        }   
        pop_rbp(PROGRAMM_CODE);
        ret(PROGRAMM_CODE);
        return;
    }
    else if ( is_this_un_func (node->node_data.data.code, INPUT) )
    {
        fprintf (stream, "\tcall SCAN\t\t\t\t\t\t\t\t; function INPUT\n");
        fprintf (stream, "\tmov qword [rbp + 16 + 8*%d], rax\n", node->right->node_data.data.code);
        if (node->right->node_data.type != VARIABLE)
            throw "Могу считать только в переменную!";
        return;
    }
    else
        throw "Не могу ELF эту унарную функцию";
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
            std::cout<<addr<<'\n';
        } else {
            def_num = buffer[ptr];
            addr = function_addr[def_num];
        }
        addr = addr - (ptr + 4);
        std::cout<<addr<<'\n';
        char* value = (char*)&addr;
        for (int i = 0; i < 4; ++i)
            buffer[ptr++] = *value++;         
    }
}