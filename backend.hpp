#include "differ-tree.hpp"

void asm_operator (FILE* stream, CalcTree::Node_t *node);

void asm_cond_operator (FILE* stream, CalcTree::Node_t *node);

void write_to_asm (const CalcTree &code);

void asm_undertree (FILE* stream, CalcTree::Node_t *node);

void asm_un_function (FILE* stream, CalcTree::Node_t *node);

bool return_value = false;

int ARYPHM_LABLES = 0;

void write_to_asm (const CalcTree &code)
{
    FILE* stream = fopen (OUTPUT_CODE, "w");


    
    fprintf (stream, ";==========INCLUDE DREW LIB==========\n\n"
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
                    ";==========INCLUDE DREW LIB==========\n\n");

    fprintf (stream,"\nglobal _start\n_start:\n");
            for (int i = 0; i <= GLOBAL_VARIABLES_NUM; ++i) //СОЗДАЛИ МЕСТО ПОД ПЕРЕМЕННЫЕ ДЛЯ МЕЙНА И ФИКТИВНЫЙ АДРЕС ВОЗВРАТА
                fprintf (stream, "\tpush 0\n");

    fprintf (stream,"\tcall MAIN\n" 
                    "\tmov rax, 0x3C\n\txor rdi, rdi\n\tsyscall\n");
    asm_undertree (stream, code.head);
    
    fclose (stream);
}



void asm_undertree (FILE* stream, CalcTree::Node_t *node)
{
    switch (node -> node_data.type)
    {
        case OPERATOR:

            asm_operator (stream, node);
            break;

        case QUANTITY:

            fprintf (stream, "\tmov r10, %lld\t\t\t\t\t\t\t\t;Actual value: %lf\n"
                             "\tpush r10\n",
                             static_cast<long long> (node -> node_data.data.value * 1024), node -> node_data.data.value);
            break;

        case BLOCK:

            asm_undertree (stream, node -> left);
            if (node -> right) 
                asm_undertree (stream, node -> right);
            break;

        case MAIN_DEF:
            fprintf (stream, "MAIN:\n"
                             "\tpush rbp\n"
                             "\tmov rbp, rsp\n");
            asm_undertree (stream, node -> right);
            fprintf (stream, "\n\tpop rbp\nret\n");
            break;

        case VARIABLE:

            fprintf (stream, "push qword [rbp + 16 + 8*%d]\n", node -> node_data.data.code);
            break;

        case UN_FUNCTION:

            asm_un_function (stream, node);
            break;

        case COND_OPERATOR:
    
            asm_cond_operator (stream, node);
            break;

        case DEF:
            
            fprintf (stream, "\nFUNCTION%d:\n", node->node_data.data.code);
            fprintf (stream, "\tpush rbp\n"
                             "\tmov rbp, rsp\n");

            asm_undertree (stream, node -> right);
            fprintf (stream, "\tpop rbp\n"
                             "\tret\n\n");
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
                asm_undertree (stream, curr_node -> left);
            }
            fprintf (stream, "\n\tcall FUNCTION%d\n", node->node_data.data.code);
            for (int i = 0; i < local_counter; ++i)
                fprintf (stream, "\tpop r8\n");
            if (return_value)
                fprintf (stream, "\tpush rax\n");
                return_value = false;
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

            fprintf (stream, "\tpush qword [rbp + 16 + 8*%d]\n", node -> node_data.data.code);
            break;
        }

        throw "CANNOT ASM THIS TYPE";
    } 
}

void asm_operator (FILE* stream, CalcTree::Node_t *node)
{
    char code = node->node_data.data.code;

    if (is_this_operator (code, ADD))
    {
        asm_undertree (stream, node -> left);
        asm_undertree (stream, node -> right);
        fprintf (stream,"\tpop r10\n"
                        "\tpop r11\n"
                        "\tadd r11, r10\n"
                        "\tpush r11\n");
        return;
    }

    if (is_this_operator (code, SUB))
    {
        asm_undertree (stream, node -> left);
        asm_undertree (stream, node -> right);
        fprintf (stream,"\tpop r10\n"
                        "\tpop r11\n"
                        "\tsub r11, r10\n"
                        "\tpush r11\n");
        return;
    }

    if (is_this_operator (code, DIV))
    {
        asm_undertree (stream, node -> left);
        asm_undertree (stream, node -> right);
        fprintf (stream,"\tpop r8\n"
                        "\tpop rax\n"
                        "\tcqo\n"
                        "\tidiv r8\n"
                        "\tmov r9, rdx\n"
                        "\txor rdx, rdx\n"
                        "\tmov r10, 1024\n"
                        "\timul r10\n"
                        "\tmov r11, rax\n"
                        "\tmov rax, r9\n"
                        "\tshr r8, 10\n"
                        "\tcqo\n"
                        "\tidiv r8\n"
                        "\tadd rax, r11\n"
                        "\tpush rax\n");       
        return;
    }

    if (is_this_operator (code, MUL))
    { 
        asm_undertree (stream, node -> left);
        asm_undertree (stream, node -> right);
        fprintf (stream,"\tpop rax\n"
                        "\tpop rcx\n"
                        "\timul rcx\n"
                        "\tshr rax, 10\n"
                        "\tshl rdx, 54\n"
                        "\tor rax, rdx\n"
                        "\tpush rax\n");
        return;
    }

    if (is_this_operator (code, ASSGN))
    {
        asm_undertree (stream, node -> right);
        if ( node->left->node_data.type != VARIABLE)
            throw "Могу присвоить только переменной!";
        fprintf (stream, "\tpop qword [rbp + 16 + 8*%d]\n", node->left->node_data.data.code);
        return;   
    }

    if (is_this_operator (code, NOT_EQUAL))
    {
        asm_undertree (stream, node -> left);
        asm_undertree (stream, node -> right);

        fprintf(stream, "\n"
                        "\tpop r10\t\t\t\t\t\t\t\t;Conditional operator - a != b\n"
                        "\tpop r11\n"
                        "\tcmp r10, r11\n"
                        "\tjne COND_T%d\n"
                        "\tpush 0\n"
                        "\tjmp COND_F%d\n"
                        "COND_T%d:\n"
                        "\tpush 1\n"
                        "COND_F%d:\n\n", LABEL_COUNTER, LABEL_COUNTER, LABEL_COUNTER, LABEL_COUNTER);
        LABEL_COUNTER++;
        return;
    }

    if (is_this_operator (code, EQUAL))
    {
        asm_undertree (stream, node -> left);
        asm_undertree (stream, node -> right);

        fprintf(stream, "\n"
                        "\tpop r10\t\t\t\t\t\t\t\t;Conditional operator - a == b\n"
                        "\tpop r11\n"
                        "\tcmp r10, r11\n"
                        "\tje COND_T%d\n"
                        "\tpush 0\n"
                        "\tjmp COND_F%d\n"
                        "COND_T%d:\n"
                        "\tpush 1\n"
                        "COND_F%d:\n\n", LABEL_COUNTER, LABEL_COUNTER, LABEL_COUNTER, LABEL_COUNTER);
        LABEL_COUNTER++;
        return; 
    }

    if (is_this_operator (code, LESS))
    {
        asm_undertree (stream, node -> left);
        asm_undertree (stream, node -> right);
        fprintf(stream, "\n"
                        "\tpop r10\t\t\t\t\t\t\t\t;Conditional operator - a < b\n"
                        "\tpop r11\n"
                        "\tcmp r11, r10\n"
                        "\tjl COND_T%d\n"
                        "\tpush 0\n"
                        "\tjmp COND_F%d\n"
                        "COND_T%d:\n"
                        "\tpush 1\n"
                        "COND_F%d:\n\n", LABEL_COUNTER, LABEL_COUNTER, LABEL_COUNTER, LABEL_COUNTER);
        LABEL_COUNTER++;
        return; 
    }

    if (is_this_operator (code, MORE))
    {
        asm_undertree (stream, node -> left);
        asm_undertree (stream, node -> right);
        fprintf(stream, "\n"
                        "\tpop r10\t\t\t\t\t\t\t\t;Conditional operator - a > b\n"
                        "\tpop r11\n"
                        "\tcmp r11, r10\n"
                        "\tjg COND_T%d\n"
                        "\tpush 0\n"
                        "\tjmp COND_F%d\n"
                        "COND_T%d:\n"
                        "\tpush 1\n"
                        "COND_F%d:\n\n", LABEL_COUNTER, LABEL_COUNTER, LABEL_COUNTER, LABEL_COUNTER);
        LABEL_COUNTER++;
        return; 
    }

    printf ("%s\n", operators[node->node_data.data.code]);
    throw "CANNOT ASM THIS OPERATOR";

}


void asm_cond_operator (FILE* stream, CalcTree::Node_t *node) //mb is ready
{
    if ( is_this_cond_op (node->node_data.data.code, "Если" ) )
    {
        int label = LABEL_COUNTER;
        LABEL_COUNTER++;

        asm_undertree (stream, node->left);
        fprintf (stream, "\tpop r10\n"
                         "\txor r11, r11\n"
                         "\tcmp r10, r11\n"
                         "\tje COND_F%d\n", label);
        asm_undertree (stream, node->right);
        fprintf (stream, "COND_F%d:\n", label);
    }
}

void asm_un_function (FILE* stream, CalcTree::Node_t *node)
{
    if ( is_this_un_func (node->node_data.data.code, PRINT) )
    {
        asm_undertree (stream, node->right);
        fprintf (stream,"\tpop r8\t\t\t\t\t\t\t\t; PRINT function\n" 
                        "\tcall PRINT\n\n");
        return;
    }
    
    else if ( is_this_un_func (node->node_data.data.code, SQRT) )
    {
        asm_undertree (stream, node->right);
        fprintf (stream, "\tpop rax\n"
                         "\tcall SQRT\n"
                         "\tpush rax\n");
        return;
    }
    else if ( is_this_un_func (node->node_data.data.code, RETURN) )
    {
        asm_undertree (stream, node->right);
        if (node->right) {
            fprintf(stream, "\tpop rax\n");
            return_value = true;
        }   
        fprintf (stream,"\tpop rbp\n" 
                        "\tret\n" );
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
        throw "Не могу ASM эту унарную функцию";
}