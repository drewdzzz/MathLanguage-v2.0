///@file

#define DEBUG

const char* OUTPUT_CODE = "asm_code.asm";

const char* OUTPUT_BYTES = "elf_code";

const char* INPUT_PROG = "programm.drew";

#include "frontend.hpp"

#include "differ-tree.hpp"

#include "backend.hpp"

#include "elf_backend.hpp"

#include <ctime>

#include <stdexcept>


#ifdef DEBUG

    #define DEBUG_CODE(code) code;
    #define DEBUG_PRINT(print) printf ("\\\\debug: %s\n", #print);
#else
    #define DEBUG_CODE(code)
    #define DEBUG_PRINT(print) 
#endif

typedef CalcTree::Node_t Node;

class COMPILATOR_ERROR
{
    char* error;
    int line_number;

public:

    COMPILATOR_ERROR (const char* error, int line_number)
    {
        this -> error = strdup (error);
        this -> line_number = line_number;
    }
};


int main ()
{
    setlocale (LC_ALL, "rus");

    FILE* stream = open_file (INPUT_PROG);
    assert (stream);

    long file_size = size_of_file (stream);
    char* input = (char*)calloc(file_size + 2, sizeof(char));
	if (!input)
	{
		printf("Memory can't be allocated\n");
		assert(input);
	}
    file_size = fread (input, sizeof (char), file_size, stream);

    CalcTree tree;


    try
    {
        tree.head = Get_G (input);
    }
    catch(const char* error)
    {
        std::cerr << "ОШИБКА КОМПИЛЯЦИИ: " << error << '\n';
        std::cerr << "Строка: " << LINE_COUNTER << '\n';
        return 15;
    }

    std::cout <<"SUCCESSFULLY COMPILED!"<<'\n';

    try
    {
        // DEBUG_CODE ( tree.dump("open"); )
        write_to_elf (tree);
        DEBUG_PRINT (WROTE TO ELF);
        write_to_asm (tree);
        DEBUG_PRINT (WROTE TO ASM);
    }
    catch(const char* error)
    {
        std::cerr << error << '\n';
        return 1;
    }

    DEBUG_CODE (printf("GLOBAL_VAR_NUM = %d\n", GLOBAL_VARIABLES_NUM));

    // system ("g++ make.cpp -o make");
    // system ("clear");
    // system ("./make");

    return 0;
}
