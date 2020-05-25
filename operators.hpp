#ifndef CALC_TREE_OPERATORS_INCLUDED
#define CALC_TREE_OPERATORS_INCLUDED

#include <math.h>

namespace OPE //Operator errors
{
    enum ERR
    {
        OK,
        NOT_EXIST,
        HAS_VARIABLE,
        DIVIDE_TO_ZERO
    } LAST_ERR;
};

const char* NOT_EQUAL = "НеРавномощен";
const char* EQUAL     = "Равномощен";
const char* MORE      = "Больше";
const char* LESS      = "Меньше";
const char* SUB       = "Минус";
const char* ADD       = "Плюс";
const char* MUL       = "Помножен";
const char* DIV       = "Поделен";
const char* POW       = "Пау";
const char* ASSGN     = "ПревратилсяВ";

static const char* operators[] = {NOT_EQUAL, EQUAL, MORE, LESS, ADD, SUB, MUL, DIV, POW, ASSGN};

enum OPERATOR{
    NOT_EQUAL_OP = 0,
    EQUAL_OP = 1,
    MORE_OP = 2,
    LESS_OP = 3,
    ADD_OP = 4,
    SUB_OP = 5,
    MUL_OP = 6,
    DIV_OP = 7,
    POW_OP = 8,
    ASSGN_OP = 9
};

bool is_operator (char* oper)
{
    for (int i = 0; i < sizeof (operators) / sizeof (operators[0]); i++)
        if ( ! strcmp (oper, operators[i]) )
            return true;
        
    return false; 
}

bool is_this_operator(char code, const char* op)
{   
    if ( ! strcmp ( operators [code], op ) )
        return true;
    else
        return false;  
}

char get_operator_code (const char* op)
{
    for (char i = 0; i < sizeof (operators)  / sizeof (operators[0]) ; i++)
    {
        if ( ! strcmp (operators[i], op) )
        {
            return i;
        }
    }

    assert (false/*USE bool is_function*/);
    return 0;
}

OPE::ERR use_operator (double a, double b, char op_code, double &res)
{
    
        if ( ! strcmp ( operators[op_code], ADD ) )
        { 
            res = a + b; return OPE::OK;
        }
        if ( ! strcmp ( operators[op_code], SUB ) )
        {
            res = a - b; return OPE::OK;
        }
        if ( ! strcmp ( operators[op_code], DIV ) )
        {
            res = a / b; return OPE::OK;
        }
        if ( ! strcmp ( operators[op_code], MUL ) )
        {
            res = a * b; return OPE::OK;
        }
        if ( ! strcmp ( operators[op_code], POW ) )
        {
            res = pow (a, b); return OPE::OK;
        }
        
        assert (false);
}

#endif