#include <stdio.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"

int main(int argc, char* argv[]){
    lexer_T* lexer = init_lexer(
        "var name = \"John Doe\";\n"
        "var noname = \"3\";\n"
        "print(name);\n"
        "print(noname);\n"
    );

    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse(parser);
    visitor_T* visitor = init_visitor();
    visitor_visit(visitor, root);



    return 0;
}