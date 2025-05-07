#include <stdio.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"

int main(int argc, char* argv[]){
    lexer_T* lexer = init_lexer(
        "var name = \"John Doe\";\n"
        "print(name);\n"
    );

    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse(parser);
    visitor_visit(root);



    return 0;
}