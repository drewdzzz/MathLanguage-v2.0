uint64_t CODE_POS = 0;

#define WRITE_BUFFER(A) {*buffer++ = A; ++CODE_POS;}

void push_rax(char*& buffer) {
    WRITE_BUFFER(0x50);
}

void push_rbp(char*& buffer) {
    WRITE_BUFFER(0x55);  
}

void push_r10(char*& buffer) {
    WRITE_BUFFER(0x41);
    WRITE_BUFFER(0x52);
}

void push_r11(char*& buffer) {
    WRITE_BUFFER(0x41);
    WRITE_BUFFER(0x53);
}

void pop_rax(char*& buffer) {
    WRITE_BUFFER(0x58);
}

void pop_rbx(char*& buffer) {
    WRITE_BUFFER(0x5B);
}

void pop_rcx(char*& buffer) {
    WRITE_BUFFER(0x59);
}

void pop_rdx(char*& buffer) {
    WRITE_BUFFER(0x5A);
}

void pop_rbp(char*& buffer) {
    WRITE_BUFFER(0x5d);
}

void pop_r8(char*& buffer) {
    WRITE_BUFFER(0x41);
    WRITE_BUFFER(0x58);
}

void pop_r9(char*& buffer) {
    WRITE_BUFFER(0x41);
    WRITE_BUFFER(0x59);
}

void pop_r10(char*& buffer) {
    WRITE_BUFFER(0x41);
    WRITE_BUFFER(0x5a);
}

void pop_r11(char*& buffer) {
    WRITE_BUFFER(0x41);
    WRITE_BUFFER(0x5b); 
}

void pop_r12(char*& buffer) {
    WRITE_BUFFER(0x41);
    WRITE_BUFFER(0x5c);
}

void inc_rax(char*& buffer) {
    *buffer++ = 0x48;
    ++CODE_POS;
    *buffer++ = 0xff;
    ++CODE_POS;
    *buffer++ = 0xc0;
    ++CODE_POS;
}

void inc_rbx(char*& buffer) {
    *buffer++ = 0x48;
    ++CODE_POS;
    *buffer++ = 0xff;
    ++CODE_POS;
    *buffer++ = 0xc3;
    ++CODE_POS;
}

void inc_rcx(char*& buffer) {
    *buffer++ = 0x48;
    ++CODE_POS;
    *buffer++ = 0xff;
    ++CODE_POS;
    *buffer++ = 0xc1;
    ++CODE_POS;
}

void inc_rdx(char*& buffer) {
    *buffer++ = 0x48;
    ++CODE_POS;
    *buffer++ = 0xff;
    ++CODE_POS;
    *buffer++ = 0xc2;
    ++CODE_POS;
}

void mov_r10_const(char*& buffer, uint64_t val) {
    *buffer++ = 0x49; 
    ++CODE_POS;
    *buffer++ = 0xba;
    ++CODE_POS;
    
    union {
        uint64_t quantity;
        char bytes[8];
    } value;
    value.quantity = val;

    for (int i = 0; i < 8; ++i) {
        *buffer++ = value.bytes[i];
        ++CODE_POS;
    }
}

void mov_rsi_const(char*& buffer, uint64_t val) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0xbe);

    union {
        uint64_t quantity;
        char bytes[8];
    } value;
    value.quantity = val;

    for (int i = 0; i < 8; ++i) {
        WRITE_BUFFER(value.bytes[i]);
    }
}

void mov_rax_r9(char*& buffer) {
    WRITE_BUFFER(0x4c);
    WRITE_BUFFER(0x89);
    WRITE_BUFFER(0xc8);
}

void mov_rcx_rdx(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0x89);
    WRITE_BUFFER(0xd1);
}

void mov_r9_rdx(char*& buffer) {
    WRITE_BUFFER(0x49);
    WRITE_BUFFER(0x89);
    WRITE_BUFFER(0xd1);
}

void mov_r11_rax(char*& buffer) {
    WRITE_BUFFER(0x49);
    WRITE_BUFFER(0x89);
    WRITE_BUFFER(0xc3);
}

void mov_rbp_rsp(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0x89);
    WRITE_BUFFER(0xe5);
}

void push_rbp_addr_plus(char*& buffer, char plus) {
    WRITE_BUFFER(0xff);
    WRITE_BUFFER(0x75);
    WRITE_BUFFER(plus);
}

void pop_rbp_addr_plus(char*& buffer, char plus) {
    WRITE_BUFFER(0x8f);
    WRITE_BUFFER(0x45);
    WRITE_BUFFER(plus);
}


void xor_rdx_rdx(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0x31);
    WRITE_BUFFER(0xd2);
}

void xor_r8_r8(char*& buffer) {
    WRITE_BUFFER(0x4d);
    WRITE_BUFFER(0x31);
    WRITE_BUFFER(0xc0);   
}

void xor_r11_r11(char*& buffer) {
    WRITE_BUFFER(0x4d);
    WRITE_BUFFER(0x31);
    WRITE_BUFFER(0xdb);
}

void or_rax_rdx(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0x09);
    WRITE_BUFFER(0xd0);
}

void ret(char*& buffer) {
    WRITE_BUFFER(0xc3);
}

void cqo(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0x99);
}

void idiv_r8(char*& buffer) {
    WRITE_BUFFER(0x49);
    WRITE_BUFFER(0xf7);
    WRITE_BUFFER(0xf8);
}

void imul_rcx(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0xf7);
    WRITE_BUFFER(0xe9);
}

void imul_r10(char*& buffer) {
    WRITE_BUFFER(0x49);
    WRITE_BUFFER(0xf7);
    WRITE_BUFFER(0xea);
}

void dec_rcx(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0xff);
    WRITE_BUFFER(0xc9);
}

void dec_rsi(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0xff);
    WRITE_BUFFER(0xce);
}

void add_rdx_byte(char*& buffer, char val) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0x83);
    WRITE_BUFFER(0xc2);
    WRITE_BUFFER(val);
}

void add_rsi_byte(char*& buffer, char val) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0x83);
    WRITE_BUFFER(0xc6);
    WRITE_BUFFER(val);
}

void add_r9_r10(char*& buffer) {
    WRITE_BUFFER(0x4d);
    WRITE_BUFFER(0x01);
    WRITE_BUFFER(0xd1);
}

void add_rax_r11(char*& buffer) {
    WRITE_BUFFER(0x4c);
    WRITE_BUFFER(0x01);
    WRITE_BUFFER(0xd8);
}

void add_r11_r10(char*& buffer) {
    WRITE_BUFFER(0x4d);
    WRITE_BUFFER(0x01);
    WRITE_BUFFER(0xd3);
}

void sub_r10_r9(char*& buffer) {
    WRITE_BUFFER(0x4d);
    WRITE_BUFFER(0x29);
    WRITE_BUFFER(0xca);
}

void sub_r11_r10(char*& buffer) {
    WRITE_BUFFER(0x4d);
    WRITE_BUFFER(0x29);
    WRITE_BUFFER(0xd3);
}


void shr_rax(char*& buffer, char val) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0xc1);
    WRITE_BUFFER(0xe8);
    WRITE_BUFFER(val);
}

void shr_r8(char*& buffer, char val) {
    WRITE_BUFFER(0x49);
    WRITE_BUFFER(0xc1);
    WRITE_BUFFER(0xe8);
    WRITE_BUFFER(val);
}

void shl_rdx(char*& buffer, char val) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0xc1);
    WRITE_BUFFER(0xe2);
    WRITE_BUFFER(val);
}

void syscall(char*& buffer) {
    WRITE_BUFFER(0x0f);
    WRITE_BUFFER(0x05);
}

void exit_syscall(char*& buffer) {
    WRITE_BUFFER(0xb8);
    WRITE_BUFFER(0x3c);
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);

    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0x31);
    WRITE_BUFFER(0xff);

    syscall(buffer);
}

void empty_call(char*& buffer, char def_num) {
    WRITE_BUFFER(0xe8);
    WRITE_BUFFER(def_num);
    for (int i = 0; i < 3; ++i)
        WRITE_BUFFER(0x00);
}



void push_byte(char*& buffer, char val) {
    WRITE_BUFFER(0x6a);
    WRITE_BUFFER(val);
}



void cmp_r10_r11(char*& buffer) {
    WRITE_BUFFER(0x4d);
    WRITE_BUFFER(0x39);
    WRITE_BUFFER(0xda);
}

void cmp_r11_r10(char*& buffer) {
    WRITE_BUFFER(0x4d);
    WRITE_BUFFER(0x39);
    WRITE_BUFFER(0xd3);
}


void je_byte(char*& buffer, char val) {
    WRITE_BUFFER(0x74);
    WRITE_BUFFER(val);
}

void jne_byte(char*& buffer, char val) {
    WRITE_BUFFER(0x75);
    WRITE_BUFFER(val);
}

void jl_byte(char*& buffer, char val) {
    WRITE_BUFFER(0x7c);
    WRITE_BUFFER(val);
}

void jg_byte(char*& buffer, char val) {
    WRITE_BUFFER(0x7f);
    WRITE_BUFFER(val);
}

void jmp_byte(char*& buffer, char val) {
    WRITE_BUFFER(0xeb);
    WRITE_BUFFER(val);
}

void push_r8(char*& buffer) {
    WRITE_BUFFER(0x41);
    WRITE_BUFFER(0x50);
}

void neg_r8(char*& buffer) {
    WRITE_BUFFER(0x49);
    WRITE_BUFFER(0xf7);
    WRITE_BUFFER(0xd8);
}

void inc_rsi(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0xff);
    WRITE_BUFFER(0xc6);
}

void dec_rdx(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0xff);
    WRITE_BUFFER(0xca);
}

void xor_rax_rax(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0x31);
    WRITE_BUFFER(0xc0);
}

void xor_rdi_rdi(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0x31);
    WRITE_BUFFER(0xff);
}

void mov_rax_rcx(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0x89);
    WRITE_BUFFER(0xc8);
}

void dec_rdi(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0xff);
    WRITE_BUFFER(0xcf);
}

void add_rdi_rdx(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0x01);
    WRITE_BUFFER(0xd7);
}

void mov_rdi_rsi(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0x89);
    WRITE_BUFFER(0xf7);
}

void sub_rdx_rsi(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0x29);
    WRITE_BUFFER(0xf2);
}

void mov_rdx_rsi(char*& buffer) {
    WRITE_BUFFER(0x48);
    WRITE_BUFFER(0x89);
    WRITE_BUFFER(0xf2);
}

void pop_rsi(char*& buffer) {
    WRITE_BUFFER(0x5e);
}

void push_rsi(char*& buffer) {
    WRITE_BUFFER(0x56);
}

uint32_t PRINT_FUNC(char*& buffer, uint32_t BUFFER_ADDR) {

    mov_rsi_const(buffer, BUFFER_ADDR);
    add_rsi_byte(buffer, 0x64);

    WRITE_BUFFER(0x45);
    WRITE_BUFFER(0x30);  //xor r10b r10b
    WRITE_BUFFER(0xd2);

    WRITE_BUFFER(0x41);
    WRITE_BUFFER(0xb9);
    WRITE_BUFFER(0x0a);
    WRITE_BUFFER(0x00);  //mov r9d, 0xa
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);

    WRITE_BUFFER(0xb9);
    WRITE_BUFFER(0x13);
    WRITE_BUFFER(0x00);  //mov ecx, 0x13
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);

    WRITE_BUFFER(0x4c);
    WRITE_BUFFER(0x89);  //mov rax, r8
    WRITE_BUFFER(0xc0);

    dec_rsi(buffer);

    WRITE_BUFFER(0x41);
    WRITE_BUFFER(0xfe);  //inc r10b
    WRITE_BUFFER(0xc2);

    xor_rdx_rdx(buffer);

    WRITE_BUFFER(0x49);
    WRITE_BUFFER(0xf7);  //div r9
    WRITE_BUFFER(0xf1);

    add_rdx_byte(buffer, 0x30);

    WRITE_BUFFER(0x88);  //mov [rsi], dl
    WRITE_BUFFER(0x16);

    WRITE_BUFFER(0xe2);  //LOOP
    WRITE_BUFFER(0xec);

    xor_rdx_rdx(buffer);

    WRITE_BUFFER(0x44);
    WRITE_BUFFER(0x88);  //mov dl, r10b
    WRITE_BUFFER(0xd2);
    
    WRITE_BUFFER(0x8c); //mov eax, ds
    WRITE_BUFFER(0xd8);

    WRITE_BUFFER(0x8e);  //mov es, eax
    WRITE_BUFFER(0xc0);

    WRITE_BUFFER(0xb0);  //mov al, 0x30
    WRITE_BUFFER(0x30);

    mov_rcx_rdx(buffer);

    dec_rcx(buffer);

    WRITE_BUFFER(0x3a);  //cmp al, [rsi]
    WRITE_BUFFER(0x06);

    jne_byte(buffer, 0x08);  //jne dec_format.break

    inc_rsi(buffer);

    dec_rdx(buffer);

    WRITE_BUFFER(0xe2);  //loop
    WRITE_BUFFER(0xf4);

    ret(buffer);

    uint32_t pos = CODE_POS;

    xor_rax_rax(buffer);

    WRITE_BUFFER(0x49);  //cmp r8 rax
    WRITE_BUFFER(0x39);
    WRITE_BUFFER(0xc0);

    WRITE_BUFFER(0x7d);  //lge PRINT.positive
    WRITE_BUFFER(0x20); 

    mov_rsi_const(buffer, BUFFER_ADDR);

    WRITE_BUFFER(0xba);  //mov edx, 1
    WRITE_BUFFER(0x01);  
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);

    WRITE_BUFFER(0xc6);  //mov byte [rsi], 0x2d
    WRITE_BUFFER(0x06);
    WRITE_BUFFER(0x2d);

    xor_rax_rax(buffer);

    inc_rax(buffer);

    xor_rdi_rdi(buffer);

    syscall(buffer);

    neg_r8(buffer);

    push_r8(buffer);

    shr_r8(buffer, 0x0a);

    WRITE_BUFFER(0xe8);  //call dec_format
    WRITE_BUFFER(0x7b);
    WRITE_BUFFER(0xff);
    WRITE_BUFFER(0xff);
    WRITE_BUFFER(0xff);

    xor_rax_rax(buffer);
    inc_rax(buffer);
    xor_rdi_rdi(buffer);

    syscall(buffer);

    pop_r8(buffer);

    mov_rsi_const(buffer, BUFFER_ADDR);

    WRITE_BUFFER(0xba);  //mov edx, 1
    WRITE_BUFFER(0x01);  
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);

    WRITE_BUFFER(0xc6);  //mov byte [rsi], 0x2e
    WRITE_BUFFER(0x06);
    WRITE_BUFFER(0x2e);

    xor_rax_rax(buffer);
    inc_rax(buffer);
    xor_rdi_rdi(buffer);
    syscall(buffer);
    
    WRITE_BUFFER(0x49);  //and r8 0x3ff
    WRITE_BUFFER(0x81);
    WRITE_BUFFER(0xe0);
    WRITE_BUFFER(0xff);
    WRITE_BUFFER(0x03);
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);

    WRITE_BUFFER(0x49); //shl r8, 0xa
    WRITE_BUFFER(0xc1);
    WRITE_BUFFER(0xe0);
    WRITE_BUFFER(0x0a);

    WRITE_BUFFER(0x4c); //mov rax, r8
    WRITE_BUFFER(0x89);
    WRITE_BUFFER(0xc0);

    cqo(buffer);

    WRITE_BUFFER(0x41);  //mov r8d, 0x3e8
    WRITE_BUFFER(0xb8);
    WRITE_BUFFER(0xe8);
    WRITE_BUFFER(0x03);
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);

    idiv_r8(buffer);

    WRITE_BUFFER(0x49);  //mov r8 rax
    WRITE_BUFFER(0x89);
    WRITE_BUFFER(0xc0);

    WRITE_BUFFER(0xe8);  //call dec_format
    WRITE_BUFFER(0x30);
    WRITE_BUFFER(0xff);
    WRITE_BUFFER(0xff);
    WRITE_BUFFER(0xff);

    xor_rax_rax(buffer);
    inc_rax(buffer);
    xor_rdi_rdi(buffer);
    inc_rdx(buffer);
    syscall(buffer);

    ret(buffer);

    return pos;
}

uint32_t SCAN_FUNC(char*& buffer, uint32_t BUFFER_ADDR) {

    push_rsi(buffer);

    WRITE_BUFFER(0xbf); //mov edi 0
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);

    WRITE_BUFFER(0xba); //mov edx, 1
    WRITE_BUFFER(0x01);
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);

    WRITE_BUFFER(0xb8); //mov eax, 0
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);

    syscall(buffer);

    WRITE_BUFFER(0x80); //cmp byte[rsi], 0xa
    WRITE_BUFFER(0x3e);
    WRITE_BUFFER(0x0a);

    je_byte(buffer, 0x05); //je gets.exit

    inc_rsi(buffer);

    jmp_byte(buffer, 0xef); //jmp gets.continue

    mov_rdx_rsi(buffer);

    pop_rsi(buffer);

    sub_rdx_rsi(buffer);

    ret(buffer);

    xor_r8_r8(buffer);

    mov_rdi_rsi(buffer);

    add_rdi_rdx(buffer);

    WRITE_BUFFER(0xb9); //mov ecx, 1
    WRITE_BUFFER(0x01);
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);

    WRITE_BUFFER(0xbb); //mov ebx, 0xa
    WRITE_BUFFER(0x0a);
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);
    WRITE_BUFFER(0x00);

    dec_rdi(buffer);

    WRITE_BUFFER(0x80); //cmp byte [rdi], 0x2d
    WRITE_BUFFER(0x3f);
    WRITE_BUFFER(0x2d);

    je_byte(buffer, 0x1b); //je toInt.end

    WRITE_BUFFER(0x8a); //mov bl, [rdi]
    WRITE_BUFFER(0x1f);

    WRITE_BUFFER(0x80); //sub bl, 0x30
    WRITE_BUFFER(0xeb);
    WRITE_BUFFER(0x30);

    mov_rax_rcx(buffer);

    WRITE_BUFFER(0xf6); //mul bl
    WRITE_BUFFER(0xe3);

    WRITE_BUFFER(0x49); //add r8 rax
    WRITE_BUFFER(0x01);
    WRITE_BUFFER(0xc0);

    mov_rax_rcx(buffer);

    WRITE_BUFFER(0x48); //mul rbx
    WRITE_BUFFER(0xf7);
    WRITE_BUFFER(0xe3);

    WRITE_BUFFER(0x48); //mov rcx, rax
    WRITE_BUFFER(0x89);
    WRITE_BUFFER(0xc1);

    WRITE_BUFFER(0x48); //cmp rdi, rsi
    WRITE_BUFFER(0x39);
    WRITE_BUFFER(0xf7);

    jne_byte(buffer, 0xdd); //jne toInt.loop

    WRITE_BUFFER(0x80); //cmp byte [rdi], 0x2d
    WRITE_BUFFER(0x3f);
    WRITE_BUFFER(0x2d);

    jne_byte(buffer, 0x09); //jne toInt.positive

    WRITE_BUFFER(0x49); //shl r8, 0xa
    WRITE_BUFFER(0xc1);
    WRITE_BUFFER(0xe0);
    WRITE_BUFFER(0x0a);

    WRITE_BUFFER(0x49); //neg r8
    WRITE_BUFFER(0xf7);
    WRITE_BUFFER(0xd8);

    jmp_byte(buffer, 0x04); //jmp toInt.neg_end

    WRITE_BUFFER(0x49); //shl r8, 0xa
    WRITE_BUFFER(0xc1);
    WRITE_BUFFER(0xe0);
    WRITE_BUFFER(0x0a);

    WRITE_BUFFER(0x4c); //mov rax r8
    WRITE_BUFFER(0x89);
    WRITE_BUFFER(0xc0);

    ret(buffer);

    uint32_t pos = CODE_POS;

    mov_rsi_const(buffer, BUFFER_ADDR);

    WRITE_BUFFER(0xe8); //call gets
    WRITE_BUFFER(0x81);
    WRITE_BUFFER(0xff);
    WRITE_BUFFER(0xff);
    WRITE_BUFFER(0xff);

    mov_rsi_const(buffer, BUFFER_ADDR);

    WRITE_BUFFER(0xe8); //call toInt
    WRITE_BUFFER(0x96);
    WRITE_BUFFER(0xff);
    WRITE_BUFFER(0xff);
    WRITE_BUFFER(0xff);

    ret(buffer);

    return pos;
}
