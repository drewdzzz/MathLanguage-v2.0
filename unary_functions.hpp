#ifndef CALC_TREE_UNARY_FUNCTIONS_INCLUDED
#define CALC_TREE_UNARY_FUNCTIONS_INCLUDED

#include <math.h>
#include <string.h>
#include <cassert>

static const char* SQRT   = "Корень";
static const char* SIN    = "Синус";
static const char* COS    = "Косинус";
static const char* LN     = "Логарифмыч";
static const char* PRINT  = "Выписать";
static const char* RETURN = "Держи_Уд";
static const char* INPUT  = "Определить";

static const char* un_functions[] = { SIN, COS, LN, PRINT, RETURN, INPUT, SQRT };

bool is_this_un_func (char code, const char* func)
{   
    if ( ! strcmp ( un_functions [code], func ) )
        return true;
    else
        return false;  
}

bool is_un_function (const char* potential_func)
{
    if ( strlen (potential_func) == 1)
        return false;
        
    for (char i = 0; i < sizeof (un_functions) / sizeof (un_functions[0]); i++)
        if ( ! strcmp (un_functions[i], potential_func) )
            return true;
    
    return false;
}

char get_un_function_code (const char* func)
{
    for (char i = 0; i < sizeof (un_functions)  / sizeof (un_functions[0]) ; i++)
    {
        if ( ! strcmp (un_functions[i], func) )
        {
            return i;
        }
    }

    assert (false/*USE bool is_function*/);
    return 0;
}

char* get_un_func_by_code (char code)
{
    assert ( code < sizeof (un_functions) );
    char* res = strdup (un_functions[code]);
    return res;
}

double use_un_func (char func_code, double value)
{

    if ( is_this_un_func (func_code, SIN) )
        return sin (value);
    if ( is_this_un_func (func_code, COS) )
        return cos (value);
    if ( is_this_un_func (func_code, LN) )
        return log (value);
        
    assert (false);
    return 0;
}

#endif