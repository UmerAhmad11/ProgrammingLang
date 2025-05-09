#ifndef AST_H
#define AST_H
#include <stdlib.h>
#include <ctype.h>
typedef struct AST_STRUCT
{
    enum{
        AST_VARIABLE_DEFINITION, //0
        AST_VARIABLE, //1
        AST_FUNCTION_CALL, //2
        AST_STRING,  //3
        AST_COMPOUND,  //4
        AST_NOOP //5
    }type;

    /* AST VARIABLE DEFINITION */
    char* variable_definition_variable_name;
    struct AST_STRUCT* variable_definition_value;

    /* AST VARIABLE */
    char* variable_name;


    /* AST FUNCTION CALL */
    char* function_call_name;
    struct AST_STRUCT** function_call_arguments;
    size_t function_call_arguments_size;

    /* AST STRING */
    char* string_value;

    /* AST COMPOUND*/
    struct AST_STRUCT** compound_value;
    size_t compound_size;

}AST_T;

AST_T* init_ast(int type);
#endif