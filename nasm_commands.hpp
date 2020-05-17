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
    *buffer++ = 0xc1;
    ++CODE_POS;
}

void inc_rcx(char*& buffer) {
    *buffer++ = 0x48;
    ++CODE_POS;
    *buffer++ = 0xff;
    ++CODE_POS;
    *buffer++ = 0xc2;
    ++CODE_POS;
}

void inc_rdx(char*& buffer) {
    *buffer++ = 0x48;
    ++CODE_POS;
    *buffer++ = 0xff;
    ++CODE_POS;
    *buffer++ = 0xc3;
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

void mov_addr(char*& buffer);

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

void imul_r10(char*& buffer) {
    WRITE_BUFFER(0x49);
    WRITE_BUFFER(0xf7);
    WRITE_BUFFER(0xea);
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







void cmp_r10_r11(char*& buffer) {
    WRITE_BUFFER(0x4d);
    WRITE_BUFFER(0x39);
    WRITE_BUFFER(0xda);
}