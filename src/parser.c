#include "include/parser.h"
#include <stdio.h>
#include <string.h>


parser_T* init_parser(lexer_T* lexer)
{
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    parser->prev_token = parser->current_token;

    return parser;
}

void parser_eat(parser_T* parser, int token_type)
{
    if (parser->current_token->type == token_type)
    {
        parser->prev_token = parser->current_token;
        parser->current_token = lexer_get_next_token(parser->lexer);
    }
    else
    {
        printf(
            "Unexpected Token '%s', with type %d",
            parser->current_token->value,
            parser->current_token->type
        );
        exit(1);
    }
}

AST_T* parser_parse(parser_T* parser)
{
    return parser_parse_statements(parser);
}

AST_T* parser_parse_statement(parser_T* parser)
{
    switch (parser->current_token->type)
    {
        case TOKEN_ID: return parser_parse_id(parser);
    }

    return init_ast(AST_NOOP);
}

AST_T* parser_parse_statements(parser_T* parser)
{
    AST_T* compound_node = init_ast(AST_COMPOUND); // compound_size = 0, compound_value = NULL

    // Handle empty program or if the first token isn't part of a valid statement
    if (parser->current_token->type == TOKEN_EOF) {
        return compound_node; // Return empty compound
    }

    AST_T* first_statement = parser_parse_statement(parser);
    
    // If the first statement is NOOP and we're at EOF (e.g. empty input or just comments),
    // we can return an empty compound or the NOOP itself.
    // For simplicity, we'll add it.
    compound_node->compound_value = calloc(1, sizeof(struct AST_STRUCT*));
    if (!compound_node->compound_value) {
        perror("parser_parse_statements: calloc failed for the first statement");
        exit(1);
    }
    compound_node->compound_value[0] = first_statement;
    compound_node->compound_size = 1; // Correctly account for the first statement

    // Loop for subsequent statements separated by semicolons
    while (parser->current_token->type == TOKEN_SEMI)
    {
        parser_eat(parser, TOKEN_SEMI);

        // If after eating a SEMI, we are at EOF, there's no next statement.
        if (parser->current_token->type == TOKEN_EOF) {
            // Optionally, you could add a NOOP here if strict about semicolons always preceding statements
            // or just break if a trailing semicolon is allowed without a following statement.
            break; 
        }

        AST_T* subsequent_statement = parser_parse_statement(parser);
        // If parser_parse_statement returns NOOP because there's nothing valid after a semicolon
        // (e.g. "stmt1;;stmt2"), you might choose not to add the NOOP.
        // For now, we add whatever is parsed.
        // if (subsequent_statement->type == AST_NOOP && parser->current_token->type == TOKEN_EOF) break; // Avoid trailing NOOPs

        compound_node->compound_size++; // Increment size for the new statement
        AST_T** temp_ptr = realloc(compound_node->compound_value, compound_node->compound_size * sizeof(struct AST_STRUCT*));
        if (!temp_ptr) {
            perror("parser_parse_statements: realloc failed for subsequent statements");
            exit(1);
        }
        compound_node->compound_value = temp_ptr;
        compound_node->compound_value[compound_node->compound_size - 1] = subsequent_statement;
    }

    return compound_node;
}

AST_T* parser_parse_expr(parser_T* parser)
{
    switch (parser->current_token->type)
    {
        case TOKEN_STRING: return parser_parse_string(parser);
        case TOKEN_ID: return parser_parse_id(parser);
    }
    return init_ast(AST_NOOP);
} // E.g. 1 + 3 * 7 THE WHOLE THING GENERALLY

AST_T* parser_parse_factor(parser_T* parser)
{

} // E.g. 1*2*(3+7) Things you multiply

AST_T* parser_parse_term(parser_T* parser)
{

} // E.g. 1 + 2 + (3*4) Things you add

AST_T* parser_parse_function_call(parser_T* parser)
{
    printf("func name: %s\n", parser->prev_token->value);

    AST_T* function_call = init_ast(AST_FUNCTION_CALL);
    parser_eat(parser, TOKEN_LPAREN);
    function_call->function_call_name = parser->prev_token->value;


    function_call->function_call_arguments = calloc(1, sizeof(struct AST_STRUCT*));
    AST_T* ast_expr = parser_parse_expr(parser);
    function_call->function_call_arguments[0] = ast_expr;
    

    while (parser->current_token->type == TOKEN_COMMA)
    {
        parser_eat(parser, TOKEN_COMMA);
        AST_T* ast_expr = parser_parse_expr(parser);
        function_call->function_call_arguments_size += 1;
        function_call->function_call_arguments = realloc(
            function_call->function_call_arguments, 
            function_call->function_call_arguments_size * sizeof(struct AST_STRUCT*)
            );
        function_call->function_call_arguments[function_call->function_call_arguments_size-1] = ast_expr;
        
    }
    parser_eat(parser, TOKEN_RPAREN);

    return function_call;
}

AST_T* parser_parse_variable(parser_T* parser)
{
    char* token_value = parser->current_token->value;
    parser_eat(parser, TOKEN_ID);
    if (parser->current_token->type == TOKEN_LPAREN)
        return parser_parse_function_call(parser);
    
    AST_T* ast_variable = init_ast(AST_VARIABLE);
    ast_variable->variable_name = token_value;
    return ast_variable;;
    
}

AST_T* parser_parse_variable_definition(parser_T* parser)
{
    parser_eat(parser, TOKEN_ID); //var
    char* variable_definition_variable_name = parser->current_token->value;
    parser_eat(parser, TOKEN_ID); //name
    parser_eat(parser, TOKEN_EQUALS); //=
    AST_T* variable__definition_value = parser_parse_expr(parser);

    AST_T* variable_definition = init_ast(AST_VARIABLE_DEFINITION);
    variable_definition->variable_definition_variable_name = variable_definition_variable_name;
    variable_definition->variable_definition_value = variable__definition_value;

    return variable_definition;

}

AST_T* parser_parse_string(parser_T* parser)
{
    AST_T* ast_string = init_ast(AST_STRING);
    ast_string->string_value = parser->current_token->value;
    parser_eat(parser, TOKEN_STRING);
    return ast_string;
}

AST_T* parser_parse_id(parser_T* parser)
{
    if (strcmp(parser->current_token->value, "var") == 0){
        return parser_parse_variable_definition(parser);
    }
    else{
        return parser_parse_variable(parser);
    }

}
